#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../../Client/Client.h"
#include "../SafeMode/SafeMode.hpp"

using namespace geode::prelude;

class NonRemovableNode : public CCNode
{
    public:
        bool allowDeleting = false;

        virtual void removeChild(CCNode* child, bool cleanup)
        {
            if (allowDeleting)
                CCNode::removeChild(child, cleanup);
        }

        virtual void visit()
        {
            for (auto child : CCArrayExt<CCNode*>(getChildren()))
            {
                child->setVisible(true);
            }

            CCNode::visit();
        }

        CREATE_FUNC(NonRemovableNode)
};

std::vector<std::string> camera = 
{
    "edit_eCamModeBtn_001.png",
    "edit_eGPOffsetBtn_001.png",
    "edit_eEdgeBtn_001.png",
    "edit_eCamGuideBtn_001.png",
    "edit_eCamRotBtn_001.png",
    "edit_eOffsetBtn_001.png",
    "edit_eStaticBtn_001.png",
    "edit_eZoomBtn_001.png",
};

static inline std::vector<Hook*> showLayoutHooks = {};

class $modify (LayoutPlayLayer, PlayLayer)
{
    struct Fields
    {
        NonRemovableNode* node;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects);
    void onQuit();

    void addObject(GameObject* object);

    QOLMOD_MOD_HOOK("show-layout", "PlayLayer::addObject")
};

class $modify (LayoutBaseGameLayer, GJBaseGameLayer)
{
    static void onModify(auto& self)
    {
        showLayoutHooks.push_back(self.getHook("GJBaseGameLayer::updateColor").unwrapOr(nullptr));
        showLayoutHooks.push_back(self.getHook("GJBaseGameLayer::createBackground").unwrapOr(nullptr));
        showLayoutHooks.push_back(self.getHook("GJBaseGameLayer::createGroundLayer").unwrapOr(nullptr));
        showLayoutHooks.push_back(self.getHook("GJBaseGameLayer::createMiddleground").unwrapOr(nullptr));

        for (auto hook : showLayoutHooks)
            hook->setAutoEnable(false);
    }

    void createBackground(int p0);
    void createGroundLayer(int p0, int p1);
    void createMiddleground(int p0);

    virtual void updateColor(cocos2d::ccColor3B& color, float fadeTime, int colorID, bool blending, float opacity, cocos2d::ccHSVValue& copyHSV, int colorIDToCopy, bool copyOpacity, EffectGameObject* callerObject, int unk1, int unk2);
};