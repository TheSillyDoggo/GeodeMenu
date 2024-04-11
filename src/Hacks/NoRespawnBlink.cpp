#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCNode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (CCNode)
{
    CCAction* runAction(CCAction* action)
    {
        if (PlayLayer::get() && PlayLayer::get()->m_player1 == as<CCNode*>(this) && action->getTag() == 11)
            return action;

        return CCNode::runAction(action);
    }

    static void onModify(auto& self) {
        std::vector<geode::Hook*> hooks;

        hooks.push_back(self.getHook("CCNode::runAction").unwrap());

        Loader::get()->queueInMainThread([hooks] 
        {
            auto modu = Client::GetModule("no-blink");

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