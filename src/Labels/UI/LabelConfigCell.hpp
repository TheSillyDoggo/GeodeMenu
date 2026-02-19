#pragma once

#include <Geode/Geode.hpp>
#include "../LabelConfig.hpp"

using namespace geode::prelude;

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