#pragma once

#include <Geode/Geode.hpp>
#include "BoundingBoxClipNode.hpp"

using namespace geode::prelude;

enum class BackgroundTheme
{
    Gradient = -1,
    Darken = -2,
    Geode1 = -4,
    Geode2 = -5,
    Geode3 = -6,
    Soggy = -7,
};

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
        CCScale9Sprite* clippingStencil2 = nullptr;
        CCClippingNode* clippingCustom = nullptr;
        CCSprite* customImg = nullptr;

        bool gradientDarkenVisible = true;
        
    public:
        virtual bool init();
        virtual void setContentSize(const CCSize& contentSize);
        virtual void update(float dt);

        void setGradientDarkenVisible(bool visible);
        void setTheme(int theme);

        void setOpacity(float opacity);
        void setColour(ccColor3B colour);

        CREATE_FUNC(BackgroundSprite);
};