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
#ifndef ENABLE_NEW_UI
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
#else


    auto onSpr = ButtonSprite::create(name.c_str(), 140, true, "bigFont.fnt", "square02b_small.png", 27, 0.65);
    onSpr->m_BGSprite->setColor({0, 0, 0});
    onSpr->m_BGSprite->setOpacity(100);
    onSpr->m_label->setOpacity(255);

    auto outline = CCScale9Sprite::create("GJ_square07.png");
    outline->setContentSize(onSpr->m_BGSprite->getContentSize());
    outline->setPosition(onSpr->m_BGSprite->getPosition());
    onSpr->addChild(outline);

    auto offSpr = ButtonSprite::create(name.c_str(), 140, true, "bigFont.fnt", "square02b_small.png", 27, 0.65);
    offSpr->m_BGSprite->setColor({0, 0, 0});
    offSpr->m_BGSprite->setOpacity(100);
    offSpr->m_label->setOpacity(150);

    auto outline2 = CCScale9Sprite::create("GJ_square07.png");
    outline2->setContentSize(offSpr->m_BGSprite->getContentSize());
    outline2->setPosition(offSpr->m_BGSprite->getPosition());
    offSpr->addChild(outline2);
    outline2->setOpacity(150);

    auto btn = CCMenuItemToggler::create(offSpr, onSpr, menu, menu_selector(Module::onToggleAndroid));
    btn->setUserData(this);
    btn->setID(id);
    btn->toggle(enabled);
    btn->setPosition(pos);
    btn->m_fSizeMult = 1.f;

    menu->addChild(btn);
#endif
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

void ColourDelegate::updateColor(cocos2d::ccColor4B const& color)
{
    auto v = as<ColourModule*>(mod);

    v->colour = ccc3(color.r, color.g, color.b);
    v->btnSpr->setColor(v->colour);
    v->save();
}

void DropdownModule::makeAndroid(CCNode* menu, CCPoint pos)
{
    auto dd = Dropdown::create(ccp(140, 30), content, menu_selector(DropdownModule::onDropdownSelectionChanged), index);
    dd->setUserData(this);
    dd->setPosition(pos + ccp(0, -15));
    menu->addChild(dd);
}

void DropdownModule::onDropdownSelectionChanged(CCObject* sender)
{
    auto mod = as<DropdownModule*>(as<CCNode*>(sender)->getParent()->getUserData());
    auto drop = as<Dropdown*>(as<CCNode*>(sender)->getParent());

    mod->index = drop->getSelectedIndex();
    mod->save();

    if (mod->delegate)
        mod->delegate->onModuleChanged(mod->enabled);
}

void TransCustomizerModule::makeAndroid(CCNode* menu, CCPoint pos)
{
    label = CCLabelBMFont::create(transNames[Mod::get()->getSavedValue<int>("transition", 0)].c_str(), "bigFont.fnt");
    label->limitLabelWidth(160, 0.8f, 0);
    menu->addChildAtPosition(label, Anchor::Center, ccp(0, 8));

    left = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), menu, menu_selector(TransCustomizerModule::onLeft));
    left->setVisible(Mod::get()->getSavedValue<int>("transition", 0) != 0);
    menu->addChildAtPosition(left, Anchor::Center, ccp(-110, 8));

    right = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), menu, menu_selector(TransCustomizerModule::onRight));
    as<CCSprite*>(right->getNormalImage())->setFlipX(true);
    right->setVisible(Mod::get()->getSavedValue<int>("transition", 0) != (transNames.size() - 1));
    menu->addChildAtPosition(right, Anchor::Center, ccp(110, 8));
}

void TransCustomizerModule::onLeft(CCObject*)
{
    Mod::get()->setSavedValue<int>("transition", Mod::get()->getSavedValue<int>("transition", 0) - 1);
    label->setString(transNames[Mod::get()->getSavedValue<int>("transition", 0)].c_str());
    label->limitLabelWidth(160, 0.8f, 0);

    left->setVisible(Mod::get()->getSavedValue<int>("transition", 0) != 0);
    right->setVisible(Mod::get()->getSavedValue<int>("transition", 0) != (transNames.size() - 1));
}

void TransCustomizerModule::onRight(CCObject*)
{
    Mod::get()->setSavedValue<int>("transition", Mod::get()->getSavedValue<int>("transition", 0) + 1);
    label->setString(transNames[Mod::get()->getSavedValue<int>("transition", 0)].c_str());
    label->limitLabelWidth(160, 0.8f, 0);

    left->setVisible(Mod::get()->getSavedValue<int>("transition", 0) != 0);
    right->setVisible(Mod::get()->getSavedValue<int>("transition", 0) != (transNames.size() - 1));
}

void SetValueModule::makeAndroid(CCNode* menu, CCPoint pos)
{
    auto label = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setScale(0.575f);
    label->setPosition(pos + ccp(-10, 0));

    inp = TextInput::create(115, "Value");
    inp->setPosition(pos + ccp(-20, 0) + ccp(230, 0));
    inp->setFilter(allowedChars);
    inp->setScale(0.8f);
    inp->getInputNode()->setID("IGNOREBYPASSES"_spr);

    auto setSpr = ButtonSprite::create("Set", "bigFont.fnt", "GJ_button_05.png");
    setSpr->setScale(0.65f);

    auto setBtn = CCMenuItemSpriteExtra::create(setSpr, menu, menu_selector(SetValueModule::onSet));
    setBtn->setUserData(this);
    setBtn->setPosition(pos + ccp(-20, 0) + ccp(308, 0));

    if (id == std::string("set-scale"))
    {
        CCSprite* ok = as<CCSprite*>(as<CCNode*>(CCLabelBMFont::create(".", "bigFont.fnt")));
        ok->setAnchorPoint(ccp(0.43f, 0.2f));
        
        auto btn = CCMenuItemToggler::create(ButtonSprite::create(reinterpret_cast<CCSprite*>(CCLabelBMFont::create("XY", "bigFont.fnt")), 30, 69, 30, 1, false, "GJ_button_05.png", true), ButtonSprite::create(ok, 30, 69, 30, 1, false, "GJ_button_05.png", true), menu, menu_selector(SetValueModule::onScaleToggle));
        btn->setPosition(pos + ccp(145, 0));
        btn->setScale(0.55f);
        btn->setUserData(this);
        ok->setScale(1.5f);

        inpX = TextInput::create(50, "X");
        inpX->setPosition(pos + ccp(-20, 0) + ccp(204, 0));
        inpX->setFilter(allowedChars);
        inpX->setScale(0.8f);
        inpX->setVisible(false);
        inpX->getInputNode()->setID("IGNOREBYPASSES"_spr);

        inpY = TextInput::create(50, "Y");
        inpY->setPosition(pos + ccp(-20, 0) + ccp(256, 0));
        inpY->setFilter(allowedChars);
        inpY->setScale(0.8f);
        inpY->setVisible(false);
        inpY->getInputNode()->setID("IGNOREBYPASSES"_spr);

        menu->addChild(btn);
        menu->addChild(inpX);
        menu->addChild(inpY);
    }

    menu->addChild(inp);
    menu->addChild(label);
    menu->addChild(setBtn);
}

void SetValueModule::onSet(CCObject* sender)
{
    auto mod = as<SetValueModule*>(as<CCNode*>(sender)->getUserData());

    if (LevelEditorLayer::get() && LevelEditorLayer::get()->m_editorUI && (LevelEditorLayer::get()->m_editorUI->m_selectedObject || LevelEditorLayer::get()->m_editorUI->m_selectedObjects->count() > 0))
    {
        float v = 1.0f;

        auto vx = numFromString<float>(mod->inp->getString());

        if (vx.isOk())
        {
            v = vx.value();
        }

        float x = 1.0f;

        auto xx = numFromString<float>(mod->inpX->getString());

        if (xx.isOk())
        {
            x = xx.value();
        }

        float y = 1.0f;

        auto yx = numFromString<float>(mod->inpY->getString());

        if (yx.isOk())
        {
            y = yx.value();
        }

        auto selObj = LevelEditorLayer::get()->m_editorUI->m_selectedObject;

        if (mod->id == std::string("set-scale"))
        {
            if (selObj)
            {
                if (mod->inp->isVisible())
                {
                    selObj->m_scaleX = v;
                    selObj->m_scaleY = v;
                    selObj->setScale(v);
                }
                else
                {
                    selObj->m_scaleX = x;
                    selObj->m_scaleY = y;
                    selObj->setScaleX(x);
                    selObj->setScaleY(y);
                }
            }
            else
            {
                for (auto selObj : CCArrayExt<GameObject*>(LevelEditorLayer::get()->m_editorUI->m_selectedObjects))
                {
                    if (mod->inp->isVisible())
                    {
                        selObj->m_scaleX = v;
                        selObj->m_scaleY = v;
                        selObj->setScale(v);
                    }
                    else
                    {
                        selObj->m_scaleX = x;
                        selObj->m_scaleY = y;
                        selObj->setScaleX(x);
                        selObj->setScaleY(y);
                    }
                }
            }
        }

        if (mod->id == std::string("set-rot"))
        {
            if (selObj)
            {
                selObj->setRotation(v);
            }
            else
            {
                for (auto obj : CCArrayExt<GameObject*>(LevelEditorLayer::get()->m_editorUI->m_selectedObjects))
                {
                    obj->setRotation(v);
                }
            }
        }
    }
}

void SetValueModule::onScaleToggle(CCObject* sender)
{
    auto mod = as<SetValueModule*>(as<CCNode*>(sender)->getUserData());

    mod->inp->setVisible(!mod->inp->isVisible());
    mod->inpX->setVisible(!mod->inpX->isVisible());
    mod->inpY->setVisible(!mod->inpY->isVisible());
}

void InputModule::updateValue()
{
    lastCheckedText = text;
    floatValue = 1;
    intValue = 0;

    auto x = numFromString<float>(text);

    floatValue = x.unwrapOr(1);

    x = numFromString<int>(text);

    if (x.isOk())
        intValue = x.value();

    if (floatValue < 0.01f)
        floatValue = 0.01f;

    if (floatValue > 99999)
        floatValue = 99999;
}

float InputModule::getFloatValue()
{
    if (lastCheckedText != text)
        updateValue();

    return floatValue;
}

int InputModule::getIntValue()
{
    if (lastCheckedText != text)
        updateValue();

    return intValue;
}

float SpeedhackTop::getAdjustedValue()
{
    float v = 1;

    if (SpeedhackTop::instance)
    {
        if (SpeedhackEnabled::instance->enabled)
        {
            SpeedhackTop::instance->getFloatValue();

            if (SpeedhackGameplay::instance->enabled)
                if (!(PlayLayer::get() || GameManager::sharedState()->getEditorLayer())) { v = 1.0f; }

            v /= CCDirector::get()->getScheduler()->getTimeScale();
        }
    }

    return v;
}