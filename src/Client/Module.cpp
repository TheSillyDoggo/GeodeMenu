#include "Module.h"

#include "../Layers/ModuleOptionsLayer.h"

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

    #ifdef GEODE_IS_ANDROID
    auto f = reinterpret_cast<FLAlertLayer*(*)(FLAlertLayerProtocol*, char const*, std::string, char const*, char const*, float)>(dlsym(dlopen("libcocos2dcpp.so", RTLD_NOW), "_ZN12FLAlertLayer6createEP20FLAlertLayerProtocolPKcSsS3_S3_f"));
    auto al = f(nullptr, dat->name.c_str(), dat->description.c_str(), "OK", nullptr, 300); //return FLAlertLayer::create(nullptr, title, desc, btn, nullptr, 300.0)
    #else
    auto al = FLAlertLayer::create(dat->name.c_str(), dat->description.c_str(), "OK");
    #endif
    al->show();
}

void Module::onToggleAndroid(CCObject* sender)
{
    auto dat = static_cast<Module*>(static_cast<CCNode*>(sender)->getUserData());

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
    auto btn = CCMenuItemToggler::createWithStandardSprites(menu, menu_selector(Module::onToggleAndroid), 0.75f);
    btn->setUserData(this);
    btn->setID(id);
    btn->toggle(enabled);
    btn->setPosition(pos);

    auto label = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setScale(0.575f);
    label->setPosition(pos + ccp(15, 0));
    label->limitLabelWidth(110 - ((options.size() != 0) ? 3 : 0), 0.575f, 0.1f);

    if (description.size() != 0 && options.size() == 0)
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

    if (options.size() != 0)
    {
        auto options = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_plus3Btn_001.png"), menu, menu_selector(Module::onOptionsAndroid));
        options->setUserData(this);
        options->setPosition(pos + ccp(label->getScaledContentSize().width + 30, 0) + ccp(-2, 0));

        menu->addChild(options);
    }

    menu->addChild(btn);
    menu->addChild(label);
}

void ColourDelegate::updateColor(cocos2d::ccColor4B const& color)
{
    log::info("colour: {}", color);

    auto v = as<ColourModule*>(mod);

    v->colour = ccc3(color.r, color.g, color.b);
    v->btnSpr->setColor(v->colour);
    v->save();
}