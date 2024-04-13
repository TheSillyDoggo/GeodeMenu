#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GJBaseGameLayer)
{
    void collisionCheckObjects(PlayerObject* p0, gd::vector<GameObject*>* p1, int p2, float p3)
    {    
        for (size_t i = 0; i < p2; i++)
        {
            auto obj = p1->at(i);

            if (p0->getObjectRect().intersectsRect(obj->getObjectRect()))
            {
                if (obj->m_objectType == GameObjectType::WavePortal || obj->m_objectType == GameObjectType::SwingPortal)
                {
                    if(this->canBeActivatedByPlayer(p0, as<EffectGameObject*>(obj)))
                    {
                        this->playerWillSwitchMode(p0, obj);
                        this->switchToFlyMode(p0, obj, false, as<int>(obj->m_objectType));
                        obj->playShineEffect();
                    }
                }
            }
        }

        GJBaseGameLayer::collisionCheckObjects(p0, p1, p2, p3);
    }

    static void onModify(auto& self) {
        std::vector<geode::Hook*> hooks;

        hooks.push_back(self.getHook("GJBaseGameLayer::collisionCheckObjects").unwrap());

        Loader::get()->queueInMainThread([hooks] 
        {
            auto modu = Client::GetModule("all-plat");

            for (auto hook : hooks)
            {
                hook->setAutoEnable(false);

                if (!modu->enabled)
                    hook->disable();

                modu->hooks.push_back(hook);
            }
        });
    }
};