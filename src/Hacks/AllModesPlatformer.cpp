#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

#ifdef QOLMOD_ALL_MODES_PLATFORMER

using namespace geode::prelude;

class $modify (GJBaseGameLayer)
{
    void collisionCheckObjects(PlayerObject* p0, gd::vector<GameObject*>* p1, int p2, float p3)
    {
        if (p0 && p1)
        {
            for (size_t i = 0; i < p1->size(); i++)
            {
                if (auto obj = p1->at(i))
                {
if (obj->m_objectType == GameObjectType::WavePortal || obj->m_objectType == GameObjectType::SwingPortal)
                        {
                    if (p0->getObjectRect().intersectsRect(obj->getObjectRect()))
                    {
                            if(this->canBeActivatedByPlayer(p0, as<EffectGameObject*>(obj)))
                            {
                                this->playerWillSwitchMode(p0, obj);
                                #ifdef GEODE_IS_WINDOWS
                                p0->switchedToMode(obj->m_objectType);

                                if (obj->m_objectType == GameObjectType::SwingPortal)
                                    p0->toggleSwingMode(true, false);
                                else
                                    p0->toggleDartMode(true, false);
                                
                                #else
                                this->switchToFlyMode(p0, obj, false, as<int>(obj->m_objectType));
                                #endif
                                obj->playShineEffect();
                            }
                        }
                    }
                }
            }
        }

        GJBaseGameLayer::collisionCheckObjects(p0, p1, p2, p3);
    }

    QOLMOD_MOD_HOOK("all-plat", "GJBaseGameLayer::collisionCheckObjects")
};

#endif