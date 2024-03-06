#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCNode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* mod = nullptr;

class $modify (CCNode)
{
    CCAction* runAction(CCAction* action)
    {
        if (!mod)
            mod = Client::GetModule("no-blink");

        if (mod->enabled && PlayLayer::get() && PlayLayer::get()->m_player1 == as<CCNode*>(this) && action->getTag() == 11)
            return action;

        //log::info("action: {} {}", action, action->getTag());

        return CCNode::runAction(action);
    }
};