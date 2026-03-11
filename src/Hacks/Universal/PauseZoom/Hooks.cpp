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

        if (m_fields->node->isZooming())
            return true;

        return false;
    }

    void updateVisibility(float dt)
    {
        this->setVisible(!shouldHidePause());
    }

    virtual void customSetup()
    {
        PauseLayer::customSetup();

        m_fields->node = ZoomInterceptNode::create(PlayLayer::get());

        this->addChild(m_fields->node);
        this->schedule(schedule_selector(ZoomPauseLayer::updateVisibility));
        updateVisibility(0);
    }
};