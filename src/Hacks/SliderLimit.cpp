#include <Geode/Geode.hpp>
#include <Geode/modify/SliderTouchLogic.hpp>
#include <Geode/modify/SliderThumb.hpp>
#include <Geode/modify/GJScaleControl.hpp>
#include "../Client/Client.h"

class $modify (SliderTouchLogic)
{
    virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1)
    {
        if (this->m_thumb->m_vertical)
            this->m_thumb->setPositionY(this->convertTouchToNodeSpace(p0).y);
        else
            this->m_thumb->setPositionX(this->convertTouchToNodeSpace(p0).x);

        this->m_slider->updateBar();

        this->m_thumb->activate();
    }

    static void onModify(auto& self) {
        geode::Hook* hook = self.getHook("SliderTouchLogic::ccTouchMoved").unwrap();

        Loader::get()->queueInMainThread([hook] 
        {
            auto modu = Client::GetModule("slider-limit");

            hook->setAutoEnable(false);

            if (!modu->enabled)
                hook->disable();

            modu->hooks.push_back(hook);
        });
    }
};