#include <Geode/Geode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/PlayerObject.hpp>
#include <../../../build/bindings/bindings/Geode/modify/CCNode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/MenuLayer.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

class $modify (CCNode)
{
    CCAction* runAction(CCAction* action)
    {
        if (PlayLayer::get() && (PlayLayer::get()->m_player1 == as<CCNode*>(this) || PlayLayer::get()->m_player2 == as<CCNode*>(this)) && action->getTag() == 11)
            return action;

        return CCNode::runAction(action);
    }

    static void onModify(auto& self) {
        auto hook = self.getHook("CCNode::runAction");

        Loader::get()->queueInMainThread([hook]
        {
            auto modu = Client::GetModule("no-blink");
            modu->addHookRaw(hook);
        });
    }
};