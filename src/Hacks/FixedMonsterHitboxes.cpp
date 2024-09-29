#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify(GJBaseGameLayer)
{
    virtual void updateDebugDraw()
    {
        GJBaseGameLayer::updateDebugDraw();

        for (auto obj : CCArrayExt<GameObject*>(m_objects))
        {
            if (obj->m_objectID == 918 || obj->m_objectID == 2012)
            {
                m_debugDrawNode->drawCircle(obj->getPosition(), obj->m_objectRadius, ccc4f(1, 0, 0, 1), 0.25f, ccc4f(1, 0, 0, 1), 120);
            }
        }
    }
};