#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class HBox : public CCObject
{
    public:
        CCPoint point;
        float scale;
};

std::vector<CCPoint> points;
std::vector<CCPoint> sizes;
Module* hitboxTrail = nullptr;

class $modify (GJBaseGameLayer)
{
    CCPoint lastPos = CCPointZero;

    virtual bool init()
    {
        if (!GJBaseGameLayer::init())
            return false;

        points.clear();
        sizes.clear();

        hitboxTrail = Client::GetModule("show-hitbox-trail");

        return true;
    }

    virtual void update(float dt)
    {
        GJBaseGameLayer::update(dt);

        if (m_player1 && m_debugDrawNode && hitboxTrail->enabled)
        {
            if (m_fields->lastPos != m_player1->getPosition())
            {
                m_fields->lastPos = m_player1->getPosition();

                points.push_back(m_fields->lastPos);
                sizes.push_back(m_player1->getObjectRect().size);
            }

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

        if (points.size() > 1500)
        {
            points.erase(points.begin());
            sizes.erase(sizes.begin());
        }
    }
};