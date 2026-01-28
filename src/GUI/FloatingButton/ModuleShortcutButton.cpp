#include "ModuleShortcutButton.hpp"
#include "../../Client/ModuleNode.hpp"
#include "../../Client/ButtonModule.hpp"
#include "../../Notifications/NotificationManager.hpp"
#include "../../Localisation/LocalisationManager.hpp"

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
    updateSprs();

    this->setOnClick([this]
    {
        if (typeinfo_cast<ButtonModule*>(mod))
        {
            static_cast<ButtonModule*>(mod)->onClick();
            return;
        }

        mod->setUserEnabled(!mod->getUserEnabled());
        mod->onToggle();
        ModuleNode::updateAllNodes(nullptr);

        auto str = mod->getUserEnabled() ? "ui/notification-mod-enabled" : "ui/notification-mod-disabled";

        NotificationManager::get()->notifyToast(utils::string::replace(LocalisationManager::get()->getLocalisedString(str), "%s", mod->getName()));
    });

    auto def = ccp(
        CCDirector::get()->getWinSize().width - 30,
        CCDirector::get()->getWinSize().height / 2
    );
    
    auto pos = ccp(
        Mod::get()->getSavedValue<float>(fmt::format("{}_shortcutpos.x", mod->getID()), def.x),
        Mod::get()->getSavedValue<float>(fmt::format("{}_shortcutpos.y", mod->getID()), def.y)
    );

    this->updatePosition(pos);
    this->setPosition(pos);
}

void ModuleShortcutButton::updatePosition(cocos2d::CCPoint point)
{
    if (CCDirector::get()->getWinSize().width != 0)
    {
        auto safe = utils::getSafeAreaRect();
        point.x = std::max<float>(safe.getMinX(), point.x);
        point.x = std::min<float>(safe.getMaxX(), point.x);
        point.y = std::max<float>(safe.getMinY(), point.y);
        point.y = std::min<float>(safe.getMaxY(), point.y);
    }

    FloatingUIButton::updatePosition(point);
    position = point;

    Mod::get()->setSavedValue<float>(fmt::format("{}_shortcutpos.x", mod->getID()), point.x);
    Mod::get()->setSavedValue<float>(fmt::format("{}_shortcutpos.y", mod->getID()), point.y);
}

void ModuleShortcutButton::updateSprs()
{
    auto bg = mod->shouldShortcutShowActivated() ? 
        bgOnSpr : 
        bgOffSpr;

    updateSprites(bg, overlaySprite, true, false);
}

void ModuleShortcutButton::setOverlaySprite(std::string spr)
{
    this->overlaySprite = spr;
    updateSprs();
}

void ModuleShortcutButton::setBackgroundSprites(std::string bgOff, std::string bgOn)
{
    this->bgOffSpr = bgOff;
    this->bgOnSpr = bgOn;
}

void ModuleShortcutButton::update(float dt)
{
    if (lastUpdated != mod->shouldShortcutShowActivated())
    {
        lastUpdated = mod->shouldShortcutShowActivated();
        updateSprs();
    }
    
    FloatingUIButton::update(dt);

    if (overlaySpr)
        overlaySpr->setColor(mod->getShortcutConfig().colour.colourForConfig(fmt::format("{}_shortcut", mod->getID())));
}