#include "HidePause.hpp"
#include "ZoomInterceptNode.hpp"
#include <Geode/modify/CCBlockLayer.hpp>

class $modify (ZoomBlockLayer, CCBlockLayer)
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

    virtual void registerWithTouchDispatcher()
    {
        CCBlockLayer::registerWithTouchDispatcher();

        if (typeinfo_cast<PauseLayer*>(this))
        {
            // m_fields->node = ZoomInterceptNode::create();

            // this->addChild(m_fields->node);
            this->schedule(schedule_selector(ZoomBlockLayer::updateVisibility));
            updateVisibility(0);
        }
    }
};