#include <Geode/Geode.hpp>
#include <Geode/modify/CommentCell.hpp>
#include <Geode/modify/LevelListCell.hpp>
#include <Geode/modify/LevelCell.hpp>
#include <Geode/modify/MapPackCell.hpp>
#include <Geode/modify/CCLayerColor.hpp>
#include <Geode/modify/GJCommentListLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (CCLayerColor)
{
    virtual bool initWithColor(const ccColor4B& color, GLfloat width, GLfloat height)
    {
        if (!CCLayerColor::initWithColor(color, width, height))
            return false;

        if (typeinfo_cast<GJListLayer*>(this))
        {
            if (color == ccc4(191, 114, 62, 255))
                this->setOpacity(0);
        }

        return true;
    }

    QOLMOD_MOD_ALL_HOOKS("trans-lists")
};

class $modify (CommentCell)
{
    void loadFromComment(GJComment* p0)
    {
        CommentCell::loadFromComment(p0);

        as<CCNode*>(this->getChildren()->objectAtIndex(0))->setVisible(false);
    }
    
    QOLMOD_MOD_ALL_HOOKS("trans-lists")
};

class $modify (LevelListCell)
{
    void loadFromList(GJLevelList* p0)
    {
        LevelListCell::loadFromList(p0);

        if (!Client::GetModuleEnabled("trans-lists"))
            return;

        as<CCNode*>(this->getChildren()->objectAtIndex(0))->setVisible(false);
    }

    QOLMOD_MOD_ALL_HOOKS("trans-lists")
};

class $modify (LevelCell)
{
    void loadFromLevel(GJGameLevel* p0)
    {
        LevelCell::loadFromLevel(p0);

        if (!Client::GetModuleEnabled("trans-lists"))
            return;

        as<CCNode*>(this->getChildren()->objectAtIndex(0))->setVisible(false);
    }

    QOLMOD_MOD_ALL_HOOKS("trans-lists")
};

class $modify (MapPackCell)
{
    void loadFromMapPack(GJMapPack* p0)
    {
        MapPackCell::loadFromMapPack(p0);

        if (!Client::GetModuleEnabled("trans-lists"))
            return;

        as<CCNode*>(this->getChildren()->objectAtIndex(0))->setVisible(false);
    }

    QOLMOD_MOD_ALL_HOOKS("trans-lists")
};

class $modify (GJCommentListLayer)
{
    bool init(BoomListView* p0, char const* p1, cocos2d::ccColor4B p2, float p3, float p4, int p5)
    {
        return GJCommentListLayer::init(p0, p1, ccc4(0, 0, 0, 0), p3, p4, p5);
    }

    QOLMOD_MOD_ALL_HOOKS("trans-lists")
};