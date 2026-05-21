#pragma once

#include "ButtonModule.hpp"

class LayoutModule : public ButtonModule
{
    protected:
        std::function<cocos2d::CCNode*()> previewNode = nullptr;

        virtual void save();
        virtual void load();

        virtual void onClick();

        void setPreviewNode(std::function<cocos2d::CCNode*()> preview);

    public:
        cocos2d::CCNode* createPreviewNode();

        virtual ModuleNode* getNode();
};