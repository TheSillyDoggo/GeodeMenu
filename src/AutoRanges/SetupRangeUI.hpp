#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"
#include "../Client/Module.hpp"
#include "PopupBase.hpp"
#include <InputNode.hpp>
#include <PulsingCircle.hpp>
#include <Button.hpp>
#include "RangeManager.hpp"

namespace qolmod
{
    class SetupRangeUI : public PopupBase
    {
        protected:
            Module* module;
            qolmod::ModRangeConfig* m_config;
            std::unordered_map<qolmod::ModRangeMode, qolmod::Button*> m_modeBtns;
            CCMenuItemToggler* m_enabledToggler;
            CCMenuItemToggler* m_practiceToggler;
            CCMenuItemToggler* m_notifyToggler;
            AdvLabelBMFont* m_enabledLabel;
            AdvLabelBMFont* m_practiceLabel;
            AdvLabelBMFont* m_notifyLabel;
            cocos2d::CCSprite* m_redSprite;
            cocos2d::CCSprite* m_greenSprite;
            cocos2d::CCSprite* m_whiteSprite;
            cocos2d::CCSprite* m_outlineSprite;
            AdvLabelBMFont* m_perLevelError = nullptr;
            qolmod::BackgroundSprite* bg;
            cocos2d::CCClippingNode* redClip = nullptr;
            cocos2d::CCClippingNode* greenClip = nullptr;
            PulsingCircle* pulsing = nullptr;
            CCMenuItemToggler* toggler = nullptr;
            cocos2d::CCMenu* deleteMenu = nullptr;
            qolmod::InputNode* min = nullptr;
            qolmod::InputNode* max = nullptr;

            cocos2d::CCMenu* createModeMenu();
            CCMenu* createInfo();
            CCSprite* createProgress();
            void updateProgress();
            void updateButtons();
            void updateConfig();

        public:
            static SetupRangeUI* create(Module* mod);

            void onInfo(CCObject* sender);
            void onClear(CCObject* sender);
            void onAdd(CCObject* sender);
            void onRemove(CCObject* sender);
            void onMode(CCObject* sender);
            void onNotify(CCObject* sender);
            void onEnabled(CCObject* sender);
            void onPractice(CCObject* sender);

            virtual bool setup();
    };
};