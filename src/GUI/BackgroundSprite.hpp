#pragma once

#include <Geode/Geode.hpp>
#include <ThemeManager.hpp>
#include "BoundingBoxClipNode.hpp"

using namespace geode::prelude;

namespace qolmod
{
    class BackgroundSprite : public CCNode
    {
        protected:
            qolmod::BackgroundType theme = (qolmod::BackgroundType)0;

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

            void updateCustomSprite();

            CREATE_FUNC(BackgroundSprite);
    };
};