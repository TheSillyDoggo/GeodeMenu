#include <Geode/Geode.hpp>
#include <Geode/modify/SliderTouchLogic.hpp>
#include <Geode/modify/SliderThumb.hpp>
#include <Geode/modify/GJScaleControl.hpp>
#include "../Client/Client.h"

class $modify (SliderTouchLogic)
{
    virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1)
    {
        this->m_thumb->setPositionX(this->convertTouchToNodeSpace(p0).x);

        //this->m_slider->setValue(value);

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

/*#ifdef GEODE_IS_ANDROID

class $modify (GJScaleControl)
{
    virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1)
    {
        GJScaleControl::ccTouchMoved(p0, p1);

        auto slider = getChildOfType<Slider>(this, -1);
        auto lbl = getChildOfType<CCLabelBMFont>(this, 2);

        if (slider && lbl && slider->isVisible())
        {
            slider->m_touchLogic->ccTouchMoved(p0, p1);

        }
    }
};

#else

#endif*/