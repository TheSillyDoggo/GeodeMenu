#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class BackgroundSprite : public CCNode
{
    protected:
        int theme = 0;

        CCScale9Sprite* colouredBG = nullptr;
        CCScale9Sprite* outlineSpr = nullptr;

        CCLayerGradient* gradientBG = nullptr;
        CCScale9Sprite* gradientOutline = nullptr;
        CCScale9Sprite* gradientDarken = nullptr;
        CCClippingNode* clipping = nullptr;
        CCScale9Sprite* clippingStencil = nullptr;

        bool gradientDarkenVisible = true;
        
    public:
        virtual bool init();
        virtual void setContentSize(const CCSize& contentSize);

        void setGradientDarkenVisible(bool visible);
        void setTheme(int theme);

        CREATE_FUNC(BackgroundSprite);
};