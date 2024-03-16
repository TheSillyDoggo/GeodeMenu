/*#include <Geode/Geode.hpp>
#include <Geode/modify/SliderTouchLogic.hpp>
#include <Geode/modify/Slider.hpp>
#include "../Client/Client.h"

class $modify (SliderTouchLogic)
{
    virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1)
    {
        if (!Client::GetModuleEnabled("slider-limit"))
            return SliderTouchLogic::ccTouchMoved(p0, p1);
        
        float w = 100;
        float x = (this->convertTouchToNodeSpace(p0).x + 100) / 2;

        this->m_slider->setValue(x / w);

        this->m_slider->updateBar();

        CCObject *pTarget = m_thumb->m_pListener;
        SEL_MenuHandler pSelector = m_thumb->m_pfnSelector;
       
        if (pTarget && pSelector) 
        {
            (pTarget->*pSelector)(this);
        }
    }
};*/