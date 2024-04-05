#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* allMod = nullptr;

class $modify (GJBaseGameLayer)
{
    void collisionCheckObjects(PlayerObject* p0, gd::vector<GameObject*>* p1, int p2, float p3)
    {
        if (!allMod)
            allMod = Client::GetModule("all-plat");

        //if (allMod->enabled)
        if (true)
        {
            for (size_t i = 0; i < p2; i++)
            {
                auto obj = p1->at(i);

                if (p0->getObjectRect().intersectsRect(obj->getObjectRect()))
                {
                    if (obj->m_objectType == GameObjectType::WavePortal || obj->m_objectType == GameObjectType::SwingPortal)
                    {
                        if(canBeActivatedByPlayer(p0, as<EffectGameObject*>(obj)))
                        {
                            playerWillSwitchMode(p0, obj);
                            switchToFlyMode(p0, obj, false, as<int>(obj->m_objectType));
                            obj->playShineEffect();
                        }
                    }
                }
            }            

            return GJBaseGameLayer::collisionCheckObjects(p0, p1, p2, p3);
        }
        else
        {
            GJBaseGameLayer::collisionCheckObjects(p0, p1, p2, p3);
        }
    }
};