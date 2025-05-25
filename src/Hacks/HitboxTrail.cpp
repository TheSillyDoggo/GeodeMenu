#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

std::vector<CCPoint> points;
std::vector<CCPoint> sizes;
Module* hitboxTrail = nullptr;
CCPoint lastPos = CCPointZero;
CCPoint lastPos2 = CCPointZero;

class $modify (GJBaseGameLayerExt, GJBaseGameLayer)
{
    virtual bool init()
    {
        if (!GJBaseGameLayer::init())
            return false;

        points.clear();
        sizes.clear();

        lastPos = CCPointZero;
        lastPos2 = CCPointZero;

        hitboxTrail = Module::get("show-hitboxes")->options[6];
        hitboxTrail->onToggle = [](bool enabled){
            points.clear();
            sizes.clear();
        };

        return true;
    }

    virtual void update(float dt)
    {
        GJBaseGameLayer::update(dt);

        if (hitboxTrail->enabled && m_debugDrawNode->isVisible())
            drawTrail();
    }

    void drawTrail()
    {
        if (m_player1)
        {
            if (lastPos != m_player1->getPosition())
            {
                lastPos = m_player1->getPosition();

                points.push_back(lastPos);
                sizes.push_back(m_player1->getObjectRect().size);
            }
        }

        if (m_player2)
        {
            if (lastPos2 != m_player2->getPosition())
            {
                lastPos2 = m_player2->getPosition();

                points.push_back(lastPos2);
                sizes.push_back(m_player2->getObjectRect().size);
            }
        }

        if (m_debugDrawNode)
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

                m_debugDrawNode->drawPolygon(squareVertices, 4, ccc4f(0, 0, 0, 0), 0.35f, ccc4f(1, 0, 0, 1));

                i++;
            }
        }

        if (points.size() > 200)
        {
            points.erase(points.begin());
            sizes.erase(sizes.begin());
        }
    }
};

class $modify (PlayLayer)
{
    void resetLevel()
    {
        PlayLayer::resetLevel();

        points.clear();
        sizes.clear();
    }
};

class $modify (LevelEditorLayer)
{
    virtual void updateVisibility(float p0)
    {
        LevelEditorLayer::updateVisibility(p0);

        if (hitboxTrail && hitboxTrail->enabled)
            reinterpret_cast<GJBaseGameLayerExt*>(this)->drawTrail();
    }

    void onPlaytest()
    {
        LevelEditorLayer::onPlaytest();

        points.clear();
        sizes.clear();
    }
};