#pragma once

#include <Geode/Geode.hpp>
#include "../Utils/Utils.hpp"

using namespace geode::prelude;

class QOLModMenuBG : public CCNode
{
    public:
        int theme;

        CCScale9Sprite* colouredBG;
        CCScale9Sprite* outlineSpr;

        CCLayerGradient* gradientBG = nullptr;
        CCScale9Sprite* gradientOutline;
        CCScale9Sprite* gradientDarken;

        bool gradientDarkenVisible = true;

        virtual bool init();
        virtual void setContentSize(const CCSize& contentSize);

        void setGradientDarkenVisible(bool visible);
        void updateTheme(int theme);

        CREATE_FUNC(QOLModMenuBG);
};