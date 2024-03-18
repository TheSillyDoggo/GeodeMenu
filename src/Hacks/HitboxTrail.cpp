/*#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

std::vector<CCPoint> points;

class $modify (GJBaseGameLayer)
{
    CCPoint lastPos = ccp(0, 0);

    Module* mod = nullptr;
    Module* mod2 = nullptr;

    CCDrawNode* dn = nullptr;

    virtual TodoReturn updateDebugDraw()
    {
        GJBaseGameLayer::updateDebugDraw();

        //if (!m_fields->mod)
            //m_fields->mod = Client::GetModule("hitbox-trail");

        if (/*m_fields->mod->enabled*//*true && m_player1 && m_debugDrawNode)
        {
            if (m_fields->lastPos != m_player1->getPosition())
            {
                m_fields->lastPos = m_player1->getPosition();

                points.push_back(m_player1->getPosition());
            }

            if (points.size() != 0)
            {
                for (size_t i = 0; i < points.size(); i++)
                {
                    m_debugDrawNode->drawRect(points[i], points[i] + ccp(100, 100), ccc4FFromccc4B(ccc4(0, 0, 0, 0)), 1, ccc4FFromccc4B(ccc4(255, 0, 0, 255)));
                }
            }            
        }
    }
};*/