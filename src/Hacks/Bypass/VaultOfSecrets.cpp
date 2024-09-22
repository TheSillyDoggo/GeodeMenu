#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

class $modify (CreatorLayer)
{
    static void onModify(auto& self) {
        self.setHookPriority("CreatorLayer::onSecretVault", 99999999);
        auto hook = self.getHook("CreatorLayer::onSecretVault");
        Loader::get()->queueInMainThread([hook]{
            Client::GetModule("vault-of-secrets-bypass")->addHookRaw(hook);
        });
    }

    void onSecretVault(cocos2d::CCObject* sender)
    {
        auto scene = CCTransitionFade::create(0.5f, SecretLayer2::scene());
        
        CCDirector::get()->replaceScene(scene);
    }
};