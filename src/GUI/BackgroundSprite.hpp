#pragma once

#include <Geode/Geode.hpp>
#include "BoundingBoxClipNode.hpp"

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
        virtual void update(float dt);

        void setGradientDarkenVisible(bool visible);
        void setTheme(int theme);

        void setOpacity(float opacity);
        void setColour(ccColor3B colour);

        CREATE_FUNC(BackgroundSprite);
};