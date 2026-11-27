#include "../../Client/Module.hpp"
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>

using namespace geode::prelude;

class LevelEdit : public Module
{
    public:
        MODULE_SETUP(LevelEdit)
        {
            setName("Level Edit");
            setID("level-edit");
            setCategory("Creator");
            setDescription("Allows you to edit any level");
        }
};

SUBMIT_HACK(LevelEdit);

class $modify (PauseLayer)
{
    virtual void customSetup()
    {
        PauseLayer::customSetup();

        if (!LevelEdit::get()->getRealEnabled())
            return;

        CCNode* menu = m_mainLayer->getChildByID("center-button-menu");

        if (!menu)
            menu = m_mainLayer->getChildByType<CCMenu>(0);

        if (!menu)
            return;

        if (!getChildBySpriteFrameName(menu, "GJ_editBtn_001.png"))
        {
            auto spr = CCSprite::createWithSpriteFrameName("GJ_editBtn_001.png");
            spr->setScale(CCSprite::createWithSpriteFrameName("GJ_menuBtn_001.png")->getContentWidth() / spr->getContentSize().width);

            auto btn = CCMenuItemSpriteExtra::create(spr, nullptr, this, menu_selector(PauseLayer::onTryEdit));
            btn->setID("edit-button");

            menu->addChild(btn, -1);
            menu->sortAllChildren();
            menu->updateLayout();
            btn->setZOrder(0);
        }
    }

    void onTryEdit(cocos2d::CCObject* sender)
    {
        if (LevelEdit::get()->getRealEnabled())
            return PauseLayer::goEdit();

        PauseLayer::onTryEdit(sender);
    }
};

class $modify (EditorPauseLayer)
{
    virtual void customSetup()
    {
        auto level = GJBaseGameLayer::get()->m_level;
        auto lType = level->m_levelType;

        if (LevelEdit::get()->getRealEnabled())
            level->m_levelType = GJLevelType::Editor;

        EditorPauseLayer::customSetup();

        level->m_levelType = lType;
    }
};