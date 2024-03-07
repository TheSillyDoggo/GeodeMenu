/*#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GJBaseGameLayer)
{
    Module* mod = nullptr;
    Module* mod2 = nullptr;

    CCDrawNode* dn = nullptr;

    std::vector<CCPoint> points = {};

    virtual TodoReturn updateDebugDraw()
    {
        GJBaseGameLayer::updateDebugDraw();

        if (!m_fields->mod)
            m_fields->mod = Client::GetModule("hitbox-trail");

        if (m_fields->mod->enabled && m_player1)
        {
            if (m_fields->points.size() == 0 || (m_fields->points[m_fields->points.size() - 1].equals(m_player1->getPosition())))
            {
                m_fields->points.push_back(m_player1->getPosition());
            }

            for (size_t i = 0; i < m_fields->points.size(); i++)
            {
                m_debugDrawNode->drawRect(m_fields->points[i], m_fields->points[i] + ccp(100, 100), ccc4FFromccc4B(ccc4(0, 0, 0, 0)), 1, ccc4FFromccc4B(ccc4(255, 0, 0, 255)));
            }
            
        }
    }
};*/