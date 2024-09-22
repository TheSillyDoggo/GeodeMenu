#include <Geode/Geode.hpp>
#include <Geode/modify/SecretLayer2.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

class $modify (SecretLayer2)
{
    static void onModify(auto& self) {
        self.setHookPriority("SecretLayer2::onSecretLevel", 99999999);
        auto hook = self.getHook("SecretLayer2::onSecretLevel");
        Loader::get()->queueInMainThread([hook]{
            Client::GetModule("the-challenge-bypass")->addHookRaw(hook);
        });
    }

    void onSecretLevel(cocos2d::CCObject* sender)
    {
        GameManager::get()->m_sceneEnum = 12;

        auto scene = CCTransitionFade::create(0.5f, PlayLayer::scene(GameLevelManager::get()->getMainLevel(3001, false), false, false));
        
        CCDirector::get()->replaceScene(scene);
    }
};