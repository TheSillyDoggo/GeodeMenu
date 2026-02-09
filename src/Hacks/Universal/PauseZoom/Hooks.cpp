#include "HidePause.hpp"
#include "ZoomInterceptNode.hpp"
#include <Geode/modify/PauseLayer.hpp>

class $modify (ZoomPauseLayer, PauseLayer)
{
    struct Fields
    {
        ZoomInterceptNode* node = nullptr;
    };

    bool shouldHidePause()
    {
        if (HidePauseMenu::get()->getRealEnabled())
            return true;

        // if (m_fields->node->zoom != 0)
            // return true;

        return false;
    }

    void updateVisibility(float dt)
    {
        this->setVisible(!shouldHidePause());
    }

    virtual void customSetup()
    {
        PauseLayer::customSetup();

        // m_fields->node = ZoomInterceptNode::create();

        // this->addChild(m_fields->node);
        this->schedule(schedule_selector(ZoomPauseLayer::updateVisibility));
    }
};