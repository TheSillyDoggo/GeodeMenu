#include <Geode/Geode.hpp>
#include <Geode/modify/CommentCell.hpp>
#include <Geode/modify/LevelListCell.hpp>
#include <Geode/modify/LevelCell.hpp>
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

        if (!Client::GetModuleEnabled("trans-lists"))
            return true;

        if (typeinfo_cast<GJListLayer*>(this))
        {
            if (color == ccc4(191, 114, 62, 255))
                this->setOpacity(0);
        }

        return true;
    }
};

class $modify (CommentCell)
{
    void loadFromComment(GJComment* p0)
    {
        CommentCell::loadFromComment(p0);

        if (!Client::GetModuleEnabled("trans-lists"))
            return;

        as<CCNode*>(this->getChildren()->objectAtIndex(0))->setVisible(false);
    }
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
};

class $modify (LevelCell)
{
    TodoReturn loadFromLevel(GJGameLevel* p0)
    {
        LevelCell::loadFromLevel(p0);

        if (!Client::GetModuleEnabled("trans-lists"))
            return;

        as<CCNode*>(this->getChildren()->objectAtIndex(0))->setVisible(false);
    }
};

class $modify (GJCommentListLayer)
{
    bool init(BoomListView* p0, char const* p1, cocos2d::ccColor4B p2, float p3, float p4, int p5)
    {
        return GJCommentListLayer::init(p0, p1, Client::GetModuleEnabled("trans-lists") ? ccc4(0, 0, 0, 0) : p2, p3, p4, p5);
    }
};