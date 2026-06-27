#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"
#include "../Client/Module.hpp"
#include "PopupBase.hpp"
#include <BetterInputNode.hpp>
#include <PulsingCircle.hpp>

namespace qolmod
{
    class SetupRangeUI : public PopupBase
    {
        protected:
            Module* module = nullptr;
            qolmod::BackgroundSprite* bg;
            cocos2d::CCClippingNode* redClip = nullptr;
            cocos2d::CCClippingNode* greenClip = nullptr;
            PulsingCircle* pulsing = nullptr;
            BetterInputNode* min = nullptr;
            BetterInputNode* max = nullptr;

            CCMenu* createInfo();
            CCSprite* createProgress();
            void updateProgress();

        public:
            static SetupRangeUI* create(Module* mod);

            void onInfo(CCObject* sender);
            void onClear(CCObject* sender);
            void onAdd(CCObject* sender);

            virtual bool setup();
    };
};