#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

std::vector<CCPoint> points;
std::vector<CCPoint> sizes;
Module* hitboxTrail = nullptr;

class $modify (GJBaseGameLayerExt, GJBaseGameLayer)
{
    CCPoint lastPos = CCPointZero;

    CCDrawNode* dn = nullptr;

    virtual bool init()
    {
        if (!GJBaseGameLayer::init())
            return false;

        points.clear();
        sizes.clear();

        hitboxTrail = Client::GetModule("show-hitbox-trail");

        m_fields->dn = getNode();

        return true;
    }

    CCDrawNode* getNode()
    {
        CCArrayExt<CCNode*> children = this->getChildren();

        for (auto node : children)
        {
            if (typeinfo_cast<CCNode*>(node) && node->getChildrenCount() == 1)
            {
                if (auto l = getChildOfType<CCLayer>(node, 0))
                {
                    if (auto n = getChildOfType<CCDrawNode>(l, 0))
                        return n;
                }
            }
        }

        return nullptr;
    }

    virtual void update(float dt)
    {
        GJBaseGameLayer::update(dt);

        #ifdef GEODE_IS_WINDOWS
        if (!m_fields->dn)
            m_fields->dn = m_debugDrawNode;
        #endif

        if (!m_fields->dn)
            m_fields->dn = getNode();

        if (hitboxTrail->enabled)
            drawTrail();
    }

    void drawTrail()
    {
        if (m_player1)
        {
            if (m_fields->lastPos != m_player1->getPosition())
            {
                m_fields->lastPos = m_player1->getPosition();

                points.push_back(m_fields->lastPos);
                sizes.push_back(m_player1->getObjectRect().size);
            }
        }

        if (m_fields->dn)
        {
            int i = 0;
            for (auto point : points)
            {
                auto squareSize = sizes[i];
                CCPoint squarePosition = point;

                CCPoint squareVertices[] = {
                    ccp(squarePosition.x - squareSize.x / 2, squarePosition.y - squareSize.y / 2), // Bottom-left
                    ccp(squarePosition.x + squareSize.x / 2, squarePosition.y - squareSize.y / 2), // Bottom-right
                    ccp(squarePosition.x + squareSize.x / 2, squarePosition.y + squareSize.y / 2), // Top-right
                    ccp(squarePosition.x - squareSize.x / 2, squarePosition.y + squareSize.y / 2)  // Top-left
                };

                m_fields->dn->drawPolygon(squareVertices, 4, ccc4f(0, 0, 0, 0), 0.35f, ccc4f(1, 0, 0, 1));

                i++;
            }
        }

        if (points.size() > 1500)
        {
            points.erase(points.begin());
            sizes.erase(sizes.begin());
        }
    }
};

class $modify (LevelEditorLayer)
{
    virtual TodoReturn updateVisibility(float p0)
    {
        LevelEditorLayer::updateVisibility(p0);

        if (hitboxTrail && hitboxTrail->enabled)
            reinterpret_cast<GJBaseGameLayerExt*>(this)->drawTrail();
    }
};