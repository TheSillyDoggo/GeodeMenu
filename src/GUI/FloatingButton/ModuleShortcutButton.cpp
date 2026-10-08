#include "ModuleShortcutButton.hpp"
#include "../../Client/ModuleNode.hpp"
#include "../../Client/ButtonModule.hpp"
#include "../../Notifications/NotificationManager.hpp"
#include "../../Localisation/LocalisationManager.hpp"
#include "../../Client/Module.hpp"
#include <ShortcutSprite.hpp>
#include <../../FloatingShortcuts/ShortcutManager.hpp>

using namespace qolmod;

ModuleShortcutButton* ModuleShortcutButton::create(Module* module)
{
    auto pRet = new ModuleShortcutButton();

    pRet->mod = module;
    if (pRet && pRet->init())
    {
        pRet->setup();
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void ModuleShortcutButton::setup()
{
    auto def = ccp(
        CCDirector::get()->getWinSize().width - 30,
        CCDirector::get()->getWinSize().height / 2
    );
    
    auto pos = ccp(
        Mod::get()->getSavedValue<float>(fmt::format("{}_shortcutpos.x", mod->getID()), def.x),
        Mod::get()->getSavedValue<float>(fmt::format("{}_shortcutpos.y", mod->getID()), def.y)
    );
    setPosition(pos, true);

    offSprite = ShortcutSprite::create();
    offSprite->setScale(getContentWidth() / offSprite->getContentWidth());
    offSprite->setPosition(getContentSize() / 2);

    onSprite = ShortcutSprite::create();
    onSprite->setScale(offSprite->getScale());
    onSprite->setPosition(offSprite->getPosition());

    this->addChild(offSprite, -3);
    this->addChild(onSprite, -3);
    updateSettings();
}

void ModuleShortcutButton::updateVisuals(float opacity)
{
    offSprite->setOpacity(opacity * 255);
    onSprite->setOpacity(opacity * 255);

    auto vis = mod->shouldShortcutShowActivated();

    offSprite->setVisible(!vis);
    onSprite->setVisible(vis);

    hitboxType = (vis ? onSprite->getShape() : offSprite->getShape()) == ShortcutShape::Square ? HitboxType::Square : HitboxType::Circle;
}

void ModuleShortcutButton::onClick()
{
    if (typeinfo_cast<ButtonModule*>(mod))
    {
        static_cast<ButtonModule*>(mod)->onClick();
        NotificationManager::get()->notifyToast(mod->getNotificationString());
        return;
    }

    mod->setUserEnabled(!mod->getUserEnabled());
    mod->onToggle();
    ModuleNode::updateAllNodes(nullptr);

    NotificationManager::get()->notifyToast(mod->getNotificationString());
}

void ModuleShortcutButton::update(float dt)
{    
    FloatingButtonBase::update(dt);

    // if (overlaySpr)
        // overlaySpr->setColor(mod->getShortcutConfig().colour.colourForConfig(fmt::format("{}_shortcut", mod->getID())));
}

void ModuleShortcutButton::updateSettings()
{
    auto conf = ShortcutManager::get()->getConfig(mod);

    setBaseScale(conf->getScale());
    setBaseOpacity(conf->getOpacity());
    setMovable(conf->isMovable());

    offSprite->m_bDontDraw = !conf->hasOffBG();
    onSprite->m_bDontDraw = !conf->hasOnBG();

    offSprite->setBoth(conf->getOffBG().first, conf->getOffBG().second);
    onSprite->setBoth(conf->getOnBG().first, conf->getOnBG().second);
}

/*
/*btn->setBackgroundSprites(shortcutConf.bgOffSprite, shortcutConf.bgOnSprite);
btn->setOverlaySprite(shortcutConf.shortcutOverlay);
btn->setButtonVisibilityConfig(shortcutConf.visibility);
btn->setMovable(shortcutConf.isMovable);
btn->setBaseScale(shortcutConf.scale);
btn->setBaseOpacity(shortcutConf.opacity);
btn->setAnimation(shortcutConf.animation);* /
*/