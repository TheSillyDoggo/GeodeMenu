#include "HitboxNode.hpp"
#include "HitboxColours.hpp"
#include "ShowHitboxes.hpp"
#include "../Noclip/Hooks.hpp"

using namespace geode::prelude;

bool HitboxNode::init()
{
    if (!CCDrawNode::init())
        return false;    

    if (!HitboxTrailNoLimit::get()->getRealEnabled())
        trailStates.reserve(HitboxTrailMaxPositions::get()->getStringInt() * sizeof(PlayerHitboxState));

    return true;
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

    if (onlyObject && obj != onlyObject)
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

cocos2d::CCRect HitboxNode::getObjectRect(GameObject* obj)
{
    if (obj->m_isObjectRectDirty)
    {
        auto save1 = obj->m_isObjectRectDirty;
        auto save2 = obj->m_boxOffsetCalculated;

        auto rec = obj->getObjectRect();

        obj->m_isObjectRectDirty = save1;
        obj->m_boxOffsetCalculated = save2;
        return rec;
    }
    else
    {
        return obj->m_objectRect;
    }
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

    auto r = getObjectRect(obj);

    if (obj->m_objectType == GameObjectType::Slope)
    {
        CCPoint vertices[] = {
            ccp(r.getMaxX(), r.getMinY()), // br
            ccp(r.getMinX(), r.getMaxY()), // tl
            ccp(r.getMinX(), r.getMinY()), // bl
        };

        CCPoint hypotenuse[] = { // i wish i was high on potenuse
            vertices[0],
            vertices[1],
            vertices[2],
        };

        switch (obj->m_slopeDirection)
        {
            case 0:
            case 7:
                vertices[1] = ccp(r.getMaxX(), r.getMaxY());
                hypotenuse[0] = vertices[1];
                hypotenuse[1] = vertices[2];
                hypotenuse[2] = vertices[0];
                break;

            case 3:
            case 6:
                vertices[0] = ccp(r.getMaxX(), r.getMaxY());
                hypotenuse[0] = vertices[2];
                hypotenuse[1] = vertices[0];
                hypotenuse[2] = vertices[1];
                break;

            case 1:
            case 5:
                vertices[2] = ccp(r.getMinX(), r.getMaxY());
                vertices[1] = ccp(r.getMaxX(), r.getMaxY());
                vertices[0] = ccp(r.getMaxX(), r.getMinY());
                hypotenuse[0] = vertices[0];
                hypotenuse[1] = vertices[2];
                hypotenuse[2] = vertices[1];
                break;
        }

        drawPolygon(vertices, 3, ccc4f(0, 0, 0, 0), getHitboxThickness(), col2, BorderAlignment::Inside);

        if (obj->m_slopeIsHazard)
        {
            drawLine(hypotenuse[0], hypotenuse[1], ccc4FFromccc3B(colourForType(HitboxColourType::Hazard)), getHitboxThickness(), hypotenuse[2]);
        }
        return;
    }

    CCPoint vertices[] = {
        ccp(r.getMinX(), r.getMinY()),
        ccp(r.getMaxX(), r.getMinY()),
        ccp(r.getMaxX(), r.getMaxY()),
        ccp(r.getMinX(), r.getMaxY()),
    };

    if (obj->m_orientedBox)
    {
        vertices[0] = obj->m_orientedBox->m_corners[0];
        vertices[1] = obj->m_orientedBox->m_corners[1];
        vertices[2] = obj->m_orientedBox->m_corners[2];
        vertices[3] = obj->m_orientedBox->m_corners[3];
    }

    float v = 0;

    if (shouldFillHitboxes())
        v = HitboxFillOpacity::get()->getValue();

    drawPolygon(vertices, 4, ccc4f(0, 0, 0, 0), getHitboxThickness(), col2, BorderAlignment::Inside);
}

bool HitboxNode::shouldRenderState(PlayerHitboxState* state)
{
    if (auto gjbgl = GJBaseGameLayer::get())
    {
        if (auto editor = typeinfo_cast<LevelEditorLayer*>(gjbgl))
        {
            if (editor->m_playbackMode != PlaybackMode::Playing)
                return true;
        }

        auto size = CCDirector::get()->getWinSize() / gjbgl->m_gameState.m_cameraZoom;
        size.height = std::max<float>(size.width, size.height);
        size.width = size.height;

        float outer = size.width * 0.4f; 

        if (state->rectReg.getMinX() > gjbgl->m_gameState.m_cameraPosition.x + size.width + outer)
            return false;

        if (state->rectReg.getMaxX() < gjbgl->m_gameState.m_cameraPosition.x - outer)
            return false;

        if (state->rectReg.getMinY() > gjbgl->m_gameState.m_cameraPosition.y + size.height + outer)
            return false;

        if (state->rectReg.getMaxY() < gjbgl->m_gameState.m_cameraPosition.y - outer)
            return false;
    }

    return true;
}

void HitboxNode::drawPlayerTrails()
{
    if (!HitboxTrail::get()->getRealEnabled())
        return;

    auto reg = ccc4FFromccc3B(colourForType(HitboxColourType::PlayerReg));
    auto mini = ccc4FFromccc3B(colourForType(HitboxColourType::Solid));
    auto max = HitboxTrailMaxPositions::get()->getStringInt();
    auto size = trailStates.size();

    float decrement = (1.0f / (float)max) * 0.75f;

    if (HitboxTrailNoLimit::get()->getRealEnabled())
    {
        decrement = (1.0f / (float)std::max<int>(max, 480)) * 0.75f;
    }

    int i = 0;
    for (auto& state : trailStates)
    {
        if (!shouldRenderState(&state))
        {
            i++;
            continue;
        }

        CCPoint vertices[] = {
            ccp(state.rectReg.getMinX(), state.rectReg.getMinY()),
            ccp(state.rectReg.getMaxX(), state.rectReg.getMinY()),
            ccp(state.rectReg.getMaxX(), state.rectReg.getMaxY()),
            ccp(state.rectReg.getMinX(), state.rectReg.getMaxY())
        };

        ccColor4F col = reg;

        if (HitboxTrailDoClickColours::get()->getRealEnabled())
        {
            switch (state.clickState)
            {
                case 1:
                    col = ccc4FFromccc3B(HitboxTrailStartClickCol::get()->getColour());
                    break;

                case 2:
                    col = ccc4FFromccc3B(HitboxTrailEndClickCol::get()->getColour());
                    break;

                case 3:
                    if (HitboxTrailHoldClickColours::get()->getRealEnabled())
                        col = ccc4FFromccc3B(HitboxTrailMidClickCol::get()->getColour());
                    break;

                default:
                    break;
            }
        }

        if (HitboxTrailDarkenByAge::get()->getRealEnabled())
        {
            float v = (float)(size - i) * decrement;

            col.r = std::clamp<float>(col.r - v, 0, 1);
            col.g = std::clamp<float>(col.g - v, 0, 1);
            col.b = std::clamp<float>(col.b - v, 0, 1);
        }

        drawPolygon(vertices, 4, ccc4f(0, 0, 0, 0), getHitboxThickness(), col, BorderAlignment::Inside);
        i++;
    }
    
    i = 0;
    for (auto& state : trailStates)
    {
        if (!shouldRenderState(&state))
        {
            i++;
            continue;
        }

        CCPoint vertices[] = {
            ccp(state.rectBlue.getMinX(), state.rectBlue.getMinY()),
            ccp(state.rectBlue.getMaxX(), state.rectBlue.getMinY()),
            ccp(state.rectBlue.getMaxX(), state.rectBlue.getMaxY()),
            ccp(state.rectBlue.getMinX(), state.rectBlue.getMaxY())
        };

        ccColor4F col = mini;

        if (HitboxTrailDarkenByAge::get()->getRealEnabled())
        {
            float v = (float)(size - i) * decrement;

            col.r = std::clamp<float>(col.r - v, 0, 1);
            col.g = std::clamp<float>(col.g - v, 0, 1);
            col.b = std::clamp<float>(col.b - v, 0, 1);
        }

        drawPolygon(vertices, 4, ccc4f(0, 0, 0, 0), getHitboxThickness(), col, BorderAlignment::Inside);
        i++;
    }
}

void HitboxNode::drawPlayerHitbox(PlayerObject* plr)
{
    auto reg = ccc4FFromccc3B(colourForType(HitboxColourType::PlayerReg));
    auto rot = ccc4FFromccc3B(colourForType(HitboxColourType::PlayerRot));
    auto mini = ccc4FFromccc3B(colourForType(HitboxColourType::Solid));

    CCPoint vertices[4];

    if (auto ob = plr->m_orientedBox)
    {
        vertices[0] = ob->m_corners[0];
        vertices[1] = ob->m_corners[1];
        vertices[2] = ob->m_corners[2];
        vertices[3] = ob->m_corners[3];

        drawPolygon(vertices, 4, ccc4f(0, 0, 0, 0), getHitboxThickness(), rot, BorderAlignment::Inside);
    }

    auto rect = plr->getObjectRect(plr->m_vehicleSize, plr->m_vehicleSize);
    vertices[0] = ccp(rect.getMinX(), rect.getMinY());
    vertices[1] = ccp(rect.getMaxX(), rect.getMinY());
    vertices[2] = ccp(rect.getMaxX(), rect.getMaxY());
    vertices[3] = ccp(rect.getMinX(), rect.getMaxY());

    drawPolygon(vertices, 4, ccc4f(0, 0, 0, 0), getHitboxThickness(), reg, BorderAlignment::Inside);

    rect = plr->getObjectRect(0.25f, 0.25f);
    vertices[0] = ccp(rect.getMinX(), rect.getMinY());
    vertices[1] = ccp(rect.getMaxX(), rect.getMinY());
    vertices[2] = ccp(rect.getMaxX(), rect.getMaxY());
    vertices[3] = ccp(rect.getMinX(), rect.getMaxY());

    drawPolygon(vertices, 4, ccc4f(0, 0, 0, 0), getHitboxThickness(), mini, BorderAlignment::Inside);
}

void HitboxNode::storePlayerTrail(PlayerObject* plr)
{
    char clickState = 0;
    bool jump = plr->m_holdingButtons[(int)PlayerButton::Jump];

    if (jump)
        clickState = 3;

    if (playerClicks.contains(plr))
    {
        if (playerClicks[plr] != jump)
        {
            if (jump)
                clickState = 1;
            else
                clickState = 2;
        }
    }
    
    playerClicks[plr] = jump;

    trailStates.push_back({ plr->getObjectRect(plr->m_vehicleSize, plr->m_vehicleSize), plr->getObjectRect(0.25f, 0.25f), clickState });
    
    if (!HitboxTrailNoLimit::get()->getRealEnabled())
    {
        if (trailStates.size() > HitboxTrailMaxPositions::get()->getStringInt())
            trailStates.erase(trailStates.begin());
            // trailStates.pop_front();
    }
}

void HitboxNode::resetTrails()
{
    trailStates.clear();
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

        drawPlayerTrails();
        drawPlayerHitbox(gjbgl->m_player1);

        if (gjbgl->m_player2 && gjbgl->m_player2->isRunning())
            drawPlayerHitbox(gjbgl->m_player2);
    }
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

void HitboxNode::drawLine(cocos2d::CCPoint point1, cocos2d::CCPoint point2, cocos2d::ccColor4F colour, float thickness, cocos2d::CCPoint towards)
{
    // this is good enough for now, i dont care that it doesnt align properly

    drawSegment(point1, point2, thickness, colour);
}

void HitboxNode::setOnlyObject(GameObject* go)
{
    this->onlyObject = go;
}