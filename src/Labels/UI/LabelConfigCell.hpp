#pragma once

#include <Geode/Geode.hpp>
#include "../LabelConfig.hpp"

using namespace geode::prelude;

#if GEODE_COMP_GD_VERSION < 22081
using NineSlice = CCScale9Sprite;
#endif

class LabelConfigCell : public CCNode
{
    protected:
        CCSize size;
        LabelConfig config;
        int index;
        NineSlice* bg;
        CCMenuItemToggler* visibleToggler;

    public:
        static LabelConfigCell* create(CCSize size, LabelConfig config, int index);

        void onOptions(CCObject* sender);
        void onDelete(CCObject* sender);
        void onToggleVisible(CCObject* sender);

        bool init(CCSize size, LabelConfig config, int index);
};