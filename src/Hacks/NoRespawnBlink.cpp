#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCNode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayerObject)
{
    Module* mod = nullptr;

    virtual void update(float p0)
    {
        if (!m_fields->mod)
            m_fields->mod = Client::GetModule("no-blink");

        PlayerObject::update(p0);

        if (m_fields->mod->enabled && numberOfRunningActions() != 0)
        {
            if (this->getActionByTag(11))
            {
                this->stopActionByTag(11);

                this->setVisible(!m_isHidden);
            }
        }
    }


};
/*
class $modify (CCNode)
{
    CCAction* runAction(CCAction* action)
    {
        log::info("action: {} {}", action, action->getTag());

        return CCNode::runAction(action);
    }
};*/