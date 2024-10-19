#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class BetterSlider : public CCLayer
{
    private:
        CCScale9Sprite* bg;
        CCSprite* fill;
    
    public:
        bool init(CCObject* sender, SEL_MenuHandler selector);

        static BetterSlider* create(CCObject* sender, SEL_MenuHandler selector);
};

/*class $modify (MenuLayer)
{
    bool init()
    {
        MenuLayer::init();

        auto slider = BetterSlider::create(nullptr, nullptr);
        slider->setPosition(getContentSize() / 2);
        this->addChild(slider, 99);

        return true;
    }
};*/