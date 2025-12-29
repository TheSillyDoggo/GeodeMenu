#pragma once

#include <Geode/Geode.hpp>
#include "../../GUI/BackgroundSprite.hpp"
#include "../../GUI/BlurLayer.hpp"
#include "../../GUI/BetterButtonSprite.hpp"
#include "../../Utils/AdvancedLabel/AdvLabelBMFont.hpp"

using namespace geode::prelude;

class CustomRewardSetupUI : public geode::Popup<>
{
    protected:
        BackgroundSprite* bg;
        std::unordered_map<std::string, TextInput*> textInputs = {};

    public:
        static CustomRewardSetupUI* create();

        CCNode* createCreateInput(std::string id, std::string str);

        void onCreateReward(CCObject* sender);

        virtual bool setup();
};