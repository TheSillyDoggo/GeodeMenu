#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* allMod = nullptr;

#ifdef GEODE_IS_WINDOWS

void hack(GJBaseGameLayer* self, PlayerObject* p0, gd::vector<GameObject*>* p1, int p2)
{
    for (size_t i = 0; i < p2; i++)
    {
        auto obj = p1->at(i);

        if (p0->getObjectRect().intersectsRect(obj->getObjectRect()))
        {
            if (obj->m_objectType == GameObjectType::WavePortal || obj->m_objectType == GameObjectType::SwingPortal)
            {
                if(self->canBeActivatedByPlayer(p0, as<EffectGameObject*>(obj)))
                {
                    self->playerWillSwitchMode(p0, obj);
                    self->switchToFlyMode(p0, obj, false, as<int>(obj->m_objectType));
                    obj->playShineEffect();
                }
            }
        }
    }
}

class $modify (GJBaseGameLayer)
{
    void collisionCheckObjects(PlayerObject* p0, gd::vector<GameObject*>* p1, int p2, float p3)
    {
        if (!allMod)
            allMod = Client::GetModule("all-plat");

        if (allMod && allMod->enabled)
        {
            hack(this, p0, p1, p2);

            return GJBaseGameLayer::collisionCheckObjects(p0, p1, p2, p3);
        }
        else
        {
            GJBaseGameLayer::collisionCheckObjects(p0, p1, p2, p3);
        }
    }
};

#endif

#ifdef GEODE_IS_ANDROID

void myCollisionCheck(GJBaseGameLayer* self, PlayerObject* p0, gd::vector<GameObject*>* p1, int p2, float p3)
{
    CCScene::get()->addChild(TextAlertPopup::create("test", 0.5f, 0.6f, 150, ""), 9999999);

    dlsym(dlopen("libcocos2dcpp.so", RTLD_NOW), "_ZN15GJBaseGameLayer21collisionCheckObjectsEP12PlayerObjectPSt6vectorIP10GameObjectSaIS4_EEif");
}

$execute
{
    Mod::get()->hook(
        dlsym(dlopen("libcocos2dcpp.so", RTLD_NOW), "_ZN15GJBaseGameLayer21collisionCheckObjectsEP12PlayerObjectPSt6vectorIP10GameObjectSaIS4_EEif"),
        &myCollisionCheck,
        "GJBaseGameLayer::collisionCheckObjects",
        tulip::hook::TulipConvention::Default
    );
}

#endif