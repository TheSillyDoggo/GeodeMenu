#include "Module.h"

#include "../Layers/ModuleOptionsLayer.h"
#include "Dropdown.h"

void Module::onOptionsAndroid(CCObject* sender)
{
    auto dat = static_cast<Module*>(static_cast<CCNode*>(sender)->getUserData());

    ModuleOptionsLayer::addToScene(dat);
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
    dat->OnChange();

    if (dat->hooks.size() > 0)
    {
        for (auto hook : dat->hooks)
        {
            if (hook->isEnabled())
            {
                if (!dat->enabled)
                    hook->disable();
            }
            else
            {
                if (dat->enabled)
                    hook->enable();
            }
        }
    }
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
}

void ColourDelegate::updateColor(cocos2d::ccColor4B const& color)
{
    log::info("colour: {}", color);

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