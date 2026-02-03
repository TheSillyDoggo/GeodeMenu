#pragma once

#include <Geode/Geode.hpp>
#include "FallbackLabelProtocol.hpp"

class FallbackLabel : public cocos2d::CCLabelBMFont, public TTFFallbackLabelProtocol
{
    protected:
        geode::Ref<cocos2d::CCSprite> ttfSprite = nullptr;

    public:
        static FallbackLabel* create(const char* text, const char* fnt);

        virtual bool init(const char* text, const char* fnt);
        virtual void visit();
        virtual void updateLabel();
};