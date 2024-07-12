#include "Module.h"

#include "../Layers/ModuleOptionsLayer.h"
#include "Dropdown.h"
#include "../UI/PCDrawUtils.hpp"

bool Module::touchBegan(CCPoint point, CCTouch* touch)
{
    if (CCRectMake(0, 0, Client::tileSize.x, Client::tileSize.y).containsPoint(point))
    {
        log::info("id: {}", id);
        mouseHeldDown = true;

        return true;
    }

    return false;
}

bool Module::touchMoved(CCPoint point, CCTouch* touch)
{
    return false;
}

bool Module::touchEndedOrCancelled(CCPoint point, CCTouch* touch, bool cancelled)
{
    if (mouseHeldDown)
    {
        enabled = !enabled;
        save();
        onChange();

        if (enabled)
            enableHooks();
        else
            disableHooks();

        mouseHeldDown = false;
    }

    return false;
}


void Module::drawModule(CCPoint pointTopLeft)
{
    PCDrawUtils::drawRect(pointTopLeft, Client::tileSize, ccc4(0, 0, 255, 255));
}


void Module::onOptionsAndroid(CCObject* sender)
{
    auto dat = static_cast<Module*>(static_cast<CCNode*>(sender)->getUserData());

    auto pop = ModuleOptionsLayer::addToScene(dat);

    if (dat->delegate)
        dat->delegate->initOptionsLayer(pop);
}

void Module::onInfoAndroid(CCObject* sender)
{
    auto dat = static_cast<Module*>(static_cast<CCNode*>(sender)->getUserData());

    if (!dat)
        return;

    auto al = FLAlertLayer::create(dat->name.c_str(), dat->description.c_str(), "OK");
    al->show();
}

void Module::onToggleAndroid(CCObject* sender)
{
    auto dat = static_cast<Module*>(static_cast<CCNode*>(sender)->getUserData());

    if (dat->isInComp)
    {
        as<CCMenuItemToggler*>(sender)->toggle(!as<CCMenuItemToggler*>(sender)->isToggled());
        return FLAlertLayer::create(dat->name.c_str(), dat->inCompAlert.c_str(), "OK")->show();
    }

    if (dat->useAlert && !dat->vAlert)
    {
        FLAlertLayer::create(dat->name.c_str(), dat->alert.c_str(), "OK")->show();
        dat->vAlert = true;
        as<CCMenuItemToggler*>(sender)->toggle(!as<CCMenuItemToggler*>(sender)->isToggled());

        return;
    }

    dat->enabled = !dat->enabled;
    dat->save();
    dat->onChange();

    if (dat->enabled)
        dat->enableHooks();
    else
        dat->disableHooks();
}

void Module::makeAndroid(CCNode* menu, CCPoint pos)
{
    if (inCompAlert.empty())
        this->isInComp = false;

    auto btn = CCMenuItemToggler::createWithStandardSprites(menu, menu_selector(Module::onToggleAndroid), 0.75f);
    btn->setUserData(this);
    btn->setID(id);
    btn->toggle(enabled);
    btn->setPosition(pos);

    auto label = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setScale(0.575f);
    label->setPosition(pos + ccp(15, 0));
    label->limitLabelWidth(110 - ((options.size() != 0) ? 3 : 0) + (isInComp ? 17.5f : 0), 0.575f, 0.1f);

    if (description.size() != 0 && options.size() == 0 && !isInComp)
    {
        auto info = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"), menu, menu_selector(Module::onInfoAndroid));
        info->setScale(0.55f);
        info->m_baseScale = info->getScale();
        info->setPosition(pos + ccp(label->getScaledContentSize().width + 30, 0) + ccp(-5, 5));
        info->setContentSize(info->getContentSize() * 2.5f);
        static_cast<CCSprite*>(info->getChildren()->objectAtIndex(0))->setPosition(info->getContentSize() / 2);
        info->setUserData(this);

        menu->addChild(info);
    }

    if (options.size() != 0 && !isInComp)
    {
        auto options = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_plus3Btn_001.png"), menu, menu_selector(Module::onOptionsAndroid));
        options->setUserData(this);
        options->setPosition(pos + ccp(label->getScaledContentSize().width + 30, 0) + ccp(-2, 0));

        menu->addChild(options);
    }

    if (isInComp)
    {
        label->setOpacity(150);
        btn->m_onButton->setOpacity(150);
        btn->m_offButton->setOpacity(150);
    }

    menu->addChild(btn);
    menu->addChild(label);
}

void Module::setIncompatible(std::string str)
{
    this->isInComp = true;
    this->inCompAlert = str;
    if (this->enabled)
    {
        this->enabled = false;
        this->onChange();
    }
}

void Module::addHookRaw(Result<Hook*> hook)
{
    if (!hook.isOk() || !hook.has_value())
        return log::error("Error adding hook: {}", hook.err());

    addHook(hook.unwrap());
}

void Module::addHook(Hook* hook)
{
    if (hook)
    {
        hooks.push_back(hook);
        hook->setAutoEnable(false);

        if (!enabled)
            hook->disable();
    }
}

void Module::addPatch(Patch* hook)
{

}

void Module::disableHooks()
{
    for (auto hook : hooks)
    {
        if (hook)
        {
            auto v = hook->disable();
            if (v.has_error())
            {
                log::error("Error Disabling hook: {}, {}", hook->getDisplayName(), v.err());
            }
        }
    }
}

void Module::enableHooks()
{
    for (auto hook : hooks)
    {
        if (hook)
        {
            auto v = hook->enable();
            if (v.has_error())
            {
                log::error("Error Enabling hook: {}, {}", hook->getDisplayName(), v.err());
            }
        }
    }
}

void Module::disablePatches()
{

}

void Module::enablePatches()
{

}