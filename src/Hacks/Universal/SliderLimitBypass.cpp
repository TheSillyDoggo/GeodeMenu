#include "../../Client/Module.hpp"
#include <Geode/modify/SliderTouchLogic.hpp>
#include <Geode/modify/GJScaleControl.hpp>
#include "../../Utils/Num.hpp"

using namespace geode::prelude;

class SliderLimitBypass : public Module
{
    public:
        MODULE_SETUP(SliderLimitBypass)
        {
            setName("Slider Limit Bypass");
            setID("slider-limit");
            setCategory("Universal");
            setDescription("Allows sliders to go beyond the limit of the slider");
        }
};

SUBMIT_HACK(SliderLimitBypass)

class $modify (SliderTouchLogic)
{
    virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1)
    {
        if (!SliderLimitBypass::get()->getRealEnabled())
            return SliderTouchLogic::ccTouchMoved(p0, p1);

        if (this->m_thumb->m_vertical)
            this->m_thumb->setPositionY(this->convertToNodeSpace(p0->getLocation()).y);
        else
            this->m_thumb->setPositionX(this->convertToNodeSpace(p0->getLocation()).x);

        this->m_slider->updateBar();

        this->m_thumb->activate();
    }
};

class $modify (GJScaleControl)
{
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
    {
        GJScaleControl::ccTouchMoved(touch, event);

        if (!SliderLimitBypass::get()->getRealEnabled())
            return;

        if (m_sliderXY && m_sliderXY->m_touchLogic->m_activateThumb)
        {
            m_sliderXY->getThumb()->setPositionX(this->convertToNodeSpace(touch->getLocation()).x);
            m_sliderXY->updateBar();

            float value = scaleFloat(m_sliderXY->getThumb()->getValue(), m_lowerBound, m_upperBound);

            updateLabelXY(value);
            // this->sliderChanged(m_sliderXY->getThumb());

            if (EditorUI::get())
            {
                EditorUI::get()->scaleXYChanged(value, value, m_scaleLocked);
            }
        }
    }
};