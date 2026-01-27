#include "HitboxNode.hpp"
#include "HitboxColours.hpp"

using namespace geode::prelude;

bool HitboxNode::init()
{
    if (!CCDrawNode::init())
        return false;

    return true;
}

void HitboxNode::updateNode()
{
    this->clear();

    if (auto gjbgl = GJBaseGameLayer::get())
    {
        forEachObject(gjbgl, [this](GameObject* obj)
        {
            if (shouldObjectDraw(obj))
                drawObjectHitbox(obj);
        });
    }
}

HitboxColourType HitboxNode::getObjectColour(GameObject* obj)
{
    if (objTypeMatchesAny(obj, { GameObjectType::Hazard, GameObjectType::AnimatedHazard }))
        return HitboxColourType::Hazard;

    if (objTypeMatchesAny(obj, { GameObjectType::Solid, GameObjectType::Slope }))
        return HitboxColourType::Solid;

    if (objTypeMatchesAny(obj, { GameObjectType::Breakable }) || obj->m_isPassable)
        return HitboxColourType::Passable;

    if (objTypeMatchesAny(obj, { GameObjectType::UserCoin, GameObjectType::Collectible, GameObjectType::SecretCoin }))
        return HitboxColourType::Interactable;

    if (objTypeMatchesAny(obj, {
            GameObjectType::InverseGravityPortal,
            GameObjectType::NormalGravityPortal,
            GameObjectType::ShipPortal,
            GameObjectType::CubePortal,
            GameObjectType::YellowJumpPad,
            GameObjectType::PinkJumpPad,
            GameObjectType::GravityPad,
            GameObjectType::YellowJumpRing,
            GameObjectType::PinkJumpRing,
            GameObjectType::GravityRing,
            GameObjectType::InverseMirrorPortal,
            GameObjectType::NormalMirrorPortal,
            GameObjectType::BallPortal,
            GameObjectType::RegularSizePortal,
            GameObjectType::MiniSizePortal,
            GameObjectType::UfoPortal,
            GameObjectType::DualPortal,
            GameObjectType::SoloPortal,
            GameObjectType::WavePortal,
            GameObjectType::RobotPortal,
            GameObjectType::TeleportPortal,
            GameObjectType::GreenRing,
            GameObjectType::DropRing,
            GameObjectType::SpiderPortal,
            GameObjectType::RedJumpPad,
            GameObjectType::RedJumpRing,
            GameObjectType::CustomRing,
            GameObjectType::DashRing,
            GameObjectType::GravityDashRing,
            GameObjectType::SwingPortal,
            GameObjectType::GravityTogglePortal,
            GameObjectType::SpiderOrb,
            GameObjectType::SpiderPad,
            GameObjectType::TeleportOrb,
        }))
        return HitboxColourType::Interactable;

    return HitboxColourType::Unk;
}

bool HitboxNode::objTypeMatchesAny(GameObject* obj, std::vector<GameObjectType> types)
{
    return std::find(types.begin(), types.end(), obj->m_objectType) != types.end();
}

cocos2d::ccColor3B HitboxNode::colourForType(HitboxColourType colour)
{
    switch (colour)
    {
        case HitboxColourType::Hazard:
            return HitboxHazard::get()->getColour();

        case HitboxColourType::Solid:
            return HitboxSolid::get()->getColour();

        case HitboxColourType::Passable:
            return HitboxPassable::get()->getColour();

        case HitboxColourType::Interactable:
            return HitboxInteractable::get()->getColour();

        case HitboxColourType::PlayerReg:
            return HitboxPlayer::get()->getColour();

        case HitboxColourType::PlayerRot:
            return HitboxPlayerRot::get()->getColour();

        default:
            return ccWHITE;
    }
}

bool HitboxNode::shouldObjectDraw(GameObject* obj)
{
    if (obj->m_isDecoration || obj->m_isDecoration2)
        return false;

    if (objTypeMatchesAny(obj, { GameObjectType::EnterEffectObject, GameObjectType::Modifier }))
        return false;

    if (obj->m_unk3ee)
        return false;

    if (getObjectColour(obj) == HitboxColourType::Unk)
        return false;

    return true;
}

bool HitboxNode::shouldFillHitboxes()
{
    return HitboxFill::get()->getRealEnabled();
}

float HitboxNode::getHitboxThickness()
{
    return 0.35f * (HitboxThickOutline::get()->getRealEnabled() ? 2 : 1);
}

void HitboxNode::drawObjectHitbox(GameObject* obj)
{
    auto col = colourForType(getObjectColour(obj));
    auto col2 = ccc4f(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, 1);

    if (obj->m_objectRadius != 0)
    {
        drawCircle(ccp(obj->m_positionX, obj->m_positionY), obj->m_objectRadius * std::max<float>(obj->m_scaleX, obj->m_scaleY), ccc4f(0, 0, 0, 0), getHitboxThickness(), col2, 69);
        return;
    }

    if (obj->m_isOrientedBoxDirty)
        obj->calculateOrientedBox();

    auto rect = obj->m_orientedBox;

    if (!rect)
        return;

    if (obj->m_objectType == GameObjectType::Slope)
    {
        auto r = obj->getObjectRect();

        CCPoint vertices[] = {
            ccp(r.getMaxX(), r.getMinY()), // br
            ccp(r.getMinX(), r.getMaxY()), // tl
            ccp(r.getMinX(), r.getMinY()), // bl
        };

        switch (obj->m_slopeDirection)
        {
            case 0:
            case 7:
                vertices[1] = ccp(r.getMaxX(), r.getMaxY());
                break;

            case 3:
            case 6:
                vertices[0] = ccp(r.getMaxX(), r.getMaxY());
                break;

            case 1:
            case 5:
                vertices[2] = ccp(r.getMinX(), r.getMaxY());
                vertices[1] = ccp(r.getMaxX(), r.getMaxY());
                vertices[0] = ccp(r.getMaxX(), r.getMinY());
                break;
        }

        drawPolygon(vertices, 3, ccc4f(0, 0, 0, 0), getHitboxThickness(), col2, BorderAlignment::Inside);
        return;
    }

    CCPoint vertices[] = {
        rect->m_corners[0],
        rect->m_corners[1],
        rect->m_corners[2],
        rect->m_corners[3]
    };

    // obj->m_isOrientedBoxDirty = true;

    float v = 0;

    if (shouldFillHitboxes())
        v = HitboxFillOpacity::get()->getValue();

    drawPolygon(vertices, 4, ccc4f(0, 0, 0, 0), getHitboxThickness(), col2, BorderAlignment::Inside);
}

void HitboxNode::forEachObject(GJBaseGameLayer* game, const std::function<void(GameObject*)>& callback)
{
    if (!callback || !game)
        return;

    int count = game->m_sections.empty() ? -1 : game->m_sections.size();
    for (int i = game->m_leftSectionIndex; i <= game->m_rightSectionIndex && i < count; ++i)
    {
        auto leftSection = game->m_sections[i];
        if (!leftSection) continue;

        auto leftSectionSize = leftSection->size();
        for (int j = game->m_bottomSectionIndex; j <= game->m_topSectionIndex && j < leftSectionSize; ++j)\
        {
            auto section = leftSection->at(j);
            if (!section) continue;

            auto sectionSize = game->m_sectionSizes[i]->at(j);
            for (int k = 0; k < sectionSize; ++k)
            {
                auto obj = section->at(k);
                if (!obj) continue;

                if (obj == game->m_player1CollisionBlock)
                    continue;

                if (obj == game->m_player2CollisionBlock)
                    continue;

                if (obj == game->m_anticheatSpike)
                    continue;

                callback(obj);
            }
        }
    }
}