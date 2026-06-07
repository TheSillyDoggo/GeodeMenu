#pragma once

#include "ButtonModule.hpp"

namespace qolmod
{
    class EditLayoutUI;
};

class LayoutModule : public ButtonModule
{
    protected:
        std::function<cocos2d::CCNode*()> previewNode = nullptr;
        geode::Anchor anchor = geode::Anchor::Center;
        cocos2d::CCPoint offset = ccp(0, 0);

        geode::Anchor defaultAnchor = geode::Anchor::Center;
        cocos2d::CCPoint defaultOffset = ccp(0, 0);

        virtual void save();
        virtual void load();

        virtual void onClick();

        void setPreviewNode(std::function<cocos2d::CCNode*()> preview);
        void setDefaults(geode::Anchor an, cocos2d::CCPoint off);

        void nodeUpdate(float);

    public:
        cocos2d::CCNode* createPreviewNode();

        void applyToNode(cocos2d::CCNode* node);

        void setPosition(cocos2d::CCPoint point);
        cocos2d::CCPoint getPosition();

        virtual ModuleNode* getNode();
};