#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/GJGameState.hpp>
#include <Geode/modify/EditorUI.hpp>
#include "HitboxNode.hpp"

class $modify (HitboxBaseGameLayer, GJBaseGameLayer)
{
    struct Fields
    {
        HitboxNode* node = nullptr;
    };

    void resetLevelVariables();
    virtual bool init();
};

class $modify (HitboxEditorLayer, LevelEditorLayer)
{
    void onPlaytest();
};

class $modify (HitboxEditorUI, EditorUI)
{
    void transformObjectCall(EditCommand command);

    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
};