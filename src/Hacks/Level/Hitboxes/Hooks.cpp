#include "ShowHitboxes.hpp"
#include "HitboxColours.hpp"
#include "../HitboxUtils.hpp"
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/CCDrawNode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include "../Noclip/Hooks.hpp"

SUBMIT_HACK(ShowHitboxes);
SUBMIT_HACK(ShowHitboxesOnDeath);
SUBMIT_HACK(HitboxTrail);

SUBMIT_OPTION(ShowHitboxes, HitboxSolid);
SUBMIT_OPTION(ShowHitboxes, HitboxHazard);
SUBMIT_OPTION(ShowHitboxes, HitboxPassable);
SUBMIT_OPTION(ShowHitboxes, HitboxInteractable);
SUBMIT_OPTION(ShowHitboxes, HitboxPlayer);
SUBMIT_OPTION(ShowHitboxes, HitboxPlayerRot);

SUBMIT_OPTION(ShowHitboxes, HitboxThickOutline);
SUBMIT_OPTION(ShowHitboxes, HitboxFill);
SUBMIT_OPTION(ShowHitboxes, HitboxFillOpacity);

SUBMIT_OPTION(HitboxTrail, HitboxTrailMaxPositions);
SUBMIT_OPTION(HitboxTrail, HitboxTrailResetOnDeath);

SUBMIT_OPTION(ShowHitboxesOnDeath, ShowHitboxesOnDeathDeathObjOnly);
SUBMIT_OPTION(ShowHitboxesOnDeath, ShowHitboxesOnDeathTrail);

void ShowHitboxes::onToggle()
{
    if (auto pl = PlayLayer::get())
    {
        pl->m_debugDrawNode->setVisible(HitboxUtils::shouldHitboxesBeVisible() || (ShowHitboxesOnDeath::get()->getRealEnabled() ? pl->m_player1->m_isDead : false) || ShowHitboxes::get()->getRealEnabled());
        pl->updateDebugDraw();
    }
}

struct HitboxTrailState
{
    CCPoint location;
    CCPoint size;
};

class $modify (HitboxBaseGameLayer, GJBaseGameLayer)
{
    struct Fields
    {
        std::vector<HitboxTrailState> states = {};
    };

    void drawForPlayer(PlayerObject* po)
    {
        CCPoint squareSize = po->getObjectRect(po->m_vehicleSize, po->m_vehicleSize).size;
        CCPoint squarePosition = po->getPosition();

        CCPoint squareVertices[] = {
            ccp(squarePosition.x - squareSize.x / 2, squarePosition.y - squareSize.y / 2), // Bottom-left
            ccp(squarePosition.x + squareSize.x / 2, squarePosition.y - squareSize.y / 2), // Bottom-right
            ccp(squarePosition.x + squareSize.x / 2, squarePosition.y + squareSize.y / 2), // Top-right
            ccp(squarePosition.x - squareSize.x / 2, squarePosition.y + squareSize.y / 2)  // Top-left
        };

        m_debugDrawNode->drawPolygon(squareVertices, 4, ccc4f(0, 0, 0, 0), 0.35f, ccc4f(-1, -1, -1, -1));

        // m_debugDrawNode->drawCircle(squarePosition, po->getObjectRect().size.width / 2, ccc4f(0, 0, 0, 0), 0.35f, ccc4f(-1, -1, -1, -1), 64);

        CCPoint squareSize2 = po->getObjectRect(0.25f, 0.25f).size;

        CCPoint squareVertices2[] = {
            ccp(squarePosition.x - squareSize2.x / 2, squarePosition.y - squareSize2.y / 2), // Bottom-left
            ccp(squarePosition.x + squareSize2.x / 2, squarePosition.y - squareSize2.y / 2), // Bottom-right
            ccp(squarePosition.x + squareSize2.x / 2, squarePosition.y + squareSize2.y / 2), // Top-right
            ccp(squarePosition.x - squareSize2.x / 2, squarePosition.y + squareSize2.y / 2)  // Top-left
        };

        m_debugDrawNode->drawPolygon(squareVertices2, 4, ccc4f(0, 0, 0, 0), 0.35f, ccc4f(0, 0.25f, 1, 1));
    }

    void drawForState(HitboxTrailState state)
    {
        CCPoint squareSize = state.size;
        CCPoint squarePosition = state.location;

        CCPoint squareVertices[] = {
            ccp(squarePosition.x - squareSize.x / 2, squarePosition.y - squareSize.y / 2),
            ccp(squarePosition.x + squareSize.x / 2, squarePosition.y - squareSize.y / 2),
            ccp(squarePosition.x + squareSize.x / 2, squarePosition.y + squareSize.y / 2),
            ccp(squarePosition.x - squareSize.x / 2, squarePosition.y + squareSize.y / 2)
        };

        m_debugDrawNode->drawPolygon(squareVertices, 4, ccc4f(0, 0, 0, 0), 0.35f, ccc4f(-1, -1, -1, -1));
    }

    virtual void updateDebugDraw()
    {
        if (typeinfo_cast<LevelEditorLayer*>(this))
            return GJBaseGameLayer::updateDebugDraw();

        std::unordered_map<GameObject*, std::pair<float, float>> hitboxes = {};
        auto array = CCArrayExt<GameObject*>(m_objects);

        if (m_player1 && m_player1->m_isDead)
        {
            if (!ShowHitboxes::get()->getRealEnabled())
            {
                if (ShowHitboxesOnDeath::get()->getRealEnabled() && ShowHitboxesOnDeathDeathObjOnly::get()->getRealEnabled())
                {
                    for (auto obj : array)
                    {
                        if (obj == reinterpret_cast<NoclipBaseGameLayer*>(this)->getDeathObject())
                            continue;

                        if (!obj->getParent() || obj->isVisible())
                            continue;
                        
                        hitboxes.emplace(obj, std::make_pair(obj->m_scaleX, obj->m_scaleY));
                        obj->m_scaleX = 0;
                        obj->m_scaleY = 0;
                        obj->dirtifyObjectRect();
                    }
                }
            }
        }

        GJBaseGameLayer::updateDebugDraw();

        for (auto obj : hitboxes)
        {
            obj.first->m_scaleX = obj.second.first;
            obj.first->m_scaleY = obj.second.second;
            obj.first->dirtifyObjectRect();
        }

        auto fields = m_fields.self();

        if (HitboxTrail::get()->getRealEnabled())
        {
            if (!m_player1->m_isDead)
                fields->states.insert(fields->states.begin(), { m_player1->m_position, m_player1->getObjectRect().size });

            if (m_player2 && m_player2->isRunning() && !m_player2->m_isDead)
                fields->states.insert(fields->states.begin(), { m_player2->m_position, m_player2->getObjectRect().size });

            while (fields->states.size() > HitboxTrailMaxPositions::get()->getStringInt())
            {
                fields->states.pop_back();
            }
            
            for (auto state : fields->states)
            {
                drawForState(state);
            }
        }

        drawForPlayer(m_player1);

        if (m_player2 && m_player2->isRunning())
            drawForPlayer(m_player2);
    }

    void resetLevelVariables()
    {
        GJBaseGameLayer::resetLevelVariables();

        if (!typeinfo_cast<LevelEditorLayer*>(this))
        {
            if (HitboxTrailResetOnDeath::get()->getRealEnabled())
                m_fields->states.clear();
        }
    }
};

class $modify (PlayLayer)
{
    virtual void postUpdate(float dt)
    {
        PlayLayer::postUpdate(dt);

        m_debugDrawNode->setVisible(HitboxUtils::shouldHitboxesBeVisible() || (ShowHitboxesOnDeath::get()->getRealEnabled() ? m_player1->m_isDead : false) || ShowHitboxes::get()->getRealEnabled());

        if (m_debugDrawNode->isVisible() != HitboxUtils::shouldHitboxesBeVisible())
            updateDebugDraw();
        else
        {
            if (ShowHitboxesOnDeath::get()->getRealEnabled() && ShowHitboxesOnDeathTrail::get()->getRealEnabled() && HitboxTrail::get()->getRealEnabled())
            {
                auto fields = base_cast<HitboxBaseGameLayer*>(this)->m_fields.self();

                if (!m_player1->m_isDead)
                    fields->states.insert(fields->states.begin(), { m_player1->m_position, m_player1->getObjectRect().size });

                if (m_player2 && m_player2->isRunning() && !m_player2->m_isDead)
                    fields->states.insert(fields->states.begin(), { m_player2->m_position, m_player2->getObjectRect().size });

                while (fields->states.size() > HitboxTrailMaxPositions::get()->getStringInt())
                {
                    fields->states.pop_back();
                }
            }
        }
    }
};

class $modify (LevelEditorLayer)
{
    void onPlaytest()
    {
        LevelEditorLayer::onPlaytest();

        auto hbgl = base_cast<HitboxBaseGameLayer*>(this);

        if (HitboxTrailResetOnDeath::get()->getRealEnabled())
            hbgl->m_fields->states.clear();
    }

    virtual void updateVisibility(float dt)
    {
        if (GameManager::get()->getGameVariable("0045") || ShowHitboxes::get()->getRealEnabled())
        {
            auto en = m_isDebugDrawEnabled;
            m_isDebugDrawEnabled = true;

            LevelEditorLayer::updateVisibility(dt);

            auto hbgl = base_cast<HitboxBaseGameLayer*>(this);

            auto fields = hbgl->m_fields.self();

            if (HitboxTrail::get()->getRealEnabled())
            {
                if (m_playbackMode == PlaybackMode::Playing)
                {
                    fields->states.insert(fields->states.begin(), { m_player1->m_position, m_player1->getObjectRect().size });

                    if (m_player2 && m_player2->isRunning())
                        fields->states.insert(fields->states.begin(), { m_player2->m_position, m_player2->getObjectRect().size });
                }

                while (fields->states.size() > HitboxTrailMaxPositions::get()->getStringInt())
                {
                    fields->states.pop_back();
                }
                
                for (auto state : fields->states)
                {
                    hbgl->drawForState(state);
                }
            }

            hbgl->drawForPlayer(m_player1);

            if (m_player2 && m_player2->isRunning())
                hbgl->drawForPlayer(m_player2);

            m_isDebugDrawEnabled = en;
        }
        else
        {
            LevelEditorLayer::updateVisibility(dt);
        }
    }
};

class $modify (CCDrawNode)
{
    bool drawPolygon(CCPoint *verts, unsigned int count, const ccColor4F &fillColor, float borderWidth, const ccColor4F &borderColor, cocos2d::BorderAlignment alignment)
    {
        if (GJBaseGameLayer::get() && GJBaseGameLayer::get()->m_debugDrawNode == this)
        {
            if (verts[0].x == verts[1].x && verts[1].x == verts[2].x && verts[2].x == verts[3].x)
                return false;

            auto border = borderColor;

            if (borderColor.r == 0 && borderColor.g == 0.25f && borderColor.b == 1)
                border = ccc4FFromccc3B(HitboxSolid::get()->getColour());

            else if (borderColor.r == 1 && borderColor.g == 0 && borderColor.b == 0)
                border = ccc4FFromccc3B(HitboxHazard::get()->getColour());

            else if (borderColor.r == 0 && borderColor.g == 1 && borderColor.b == 0)
                border = ccc4FFromccc3B(HitboxInteractable::get()->getColour());

            else if (borderColor.r == 0 && borderColor.g == 1 && borderColor.b == 1)
                border = ccc4FFromccc3B(HitboxPassable::get()->getColour());

            else if (borderColor.r == 1 && borderColor.g == 1 && borderColor.b == 0)
                border = ccc4FFromccc3B(HitboxPlayerRot::get()->getColour());

            else if (borderColor.r == -1 && borderColor.g == -1 && borderColor.b == -1)
                border = ccc4FFromccc3B(HitboxPlayer::get()->getColour());

            auto fill = border;
            fill.a = HitboxFillOpacity::get()->getValue();

            if (borderWidth == 0)
                borderWidth = 1;

            return CCDrawNode::drawPolygon(verts, count, HitboxFill::get()->getRealEnabled() ? fill : fillColor, borderWidth * (HitboxThickOutline::get()->getRealEnabled() ? 2 : 1), border, alignment);
        }

        return CCDrawNode::drawPolygon(verts, count, fillColor, borderWidth, borderColor, alignment);
    }
};