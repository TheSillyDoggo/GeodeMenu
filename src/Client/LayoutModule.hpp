#pragma once

#include "ButtonModule.hpp"

namespace qolmod
{
    class EditLayoutUI;
};

class LayoutModule : public ButtonModule
{
    friend class qolmod::EditLayoutUI;
    protected:
        std::function<cocos2d::CCNode*()> previewNode = nullptr;
        geode::Anchor anchor = geode::Anchor::Center;
        cocos2d::CCPoint offset = ccp(0, 0);

        geode::Anchor defaultAnchor = geode::Anchor::Center;
        cocos2d::CCPoint defaultOffset = ccp(0, 0);
        struct FloatOption
        {
            std::string display = "";
            float defaultV = 1.0f;
            float min = 0.0f;
            float max = 1.0f;
            float value = 0.0f;
        };
        std::unordered_map<std::string, FloatOption> floatOptions = {};

        virtual void save();
        virtual void load();

        virtual void onClick();

        void setPreviewNode(std::function<cocos2d::CCNode*()> preview);
        void setDefaults(geode::Anchor an, cocos2d::CCPoint off);

        void addOption(std::string key, std::string displayKey, float defaultV, float min, float max);

        void nodeUpdate(float);

    public:
        cocos2d::CCNode* createPreviewNode();

        void applyToNode(cocos2d::CCNode* node);

        void setPosition(cocos2d::CCPoint point);
        cocos2d::CCPoint getPosition();

        void setOption(std::string key, float value);
        float getOption(std::string key);

        virtual ModuleNode* getNode();
};