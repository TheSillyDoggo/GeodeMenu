#include <Geode/Geode.hpp>
#include <Geode/modify/SliderTouchLogic.hpp>
#include <Geode/modify/Slider.hpp>
#include "../Client/Client.h"

const char* get_node_name(CCNode* node) {
	// works because msvc's typeid().name() returns undecorated name
	// typeid(CCNode).name() == "class cocos2d::CCNode"
	// the + 6 gets rid of the class prefix
	// "class cocos2d::CCNode" + 6 == "cocos2d::CCNode"
	return typeid(*node).name() + 6;
}
class $modify (Slider)
{
    static Slider* create(cocos2d::CCNode* p0, cocos2d::SEL_MenuHandler p1, char const* p2, char const* p3, char const* p4, char const* p5, float p6)
    {

    }
};


class $modify (SliderTouchLogic)
{
    virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1)
    {
        if (!Client::GetModuleEnabled("slider-limit"))
        {
            SliderTouchLogic::ccTouchMoved(p0, p1);
            return;
        }
        
        float w = 100;
        float x = (this->convertTouchToNodeSpace(p0).x + 100) / 2;

        this->m_slider->setValue(x / w);
        this->ccTouchEnded();
    }
};