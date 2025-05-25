#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

class $modify (CreatorLayer)
{
    static void onModify(auto& self) {
        (void)self.setHookPriority("CreatorLayer::onTreasureRoom", 99999999);
        auto hook = self.getHook("CreatorLayer::onTreasureRoom");
        Loader::get()->queueInMainThread([hook]{
            Module::get("treasure-room-bypass")->addHookRaw(hook);
        });
    }

    void onTreasureRoom(cocos2d::CCObject* sender)
    {
        auto scene = CCTransitionFade::create(0.5f, SecretRewardsLayer::scene(false));
        
        CCDirector::get()->replaceScene(scene);
    }
};