#include "ModuleShortcutButton.hpp"
#include "../../Client/ModuleNode.hpp"

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
        mod->setUserEnabled(!mod->getUserEnabled());

        mod->onToggle();
        ModuleNode::updateAllNodes(nullptr);
    });

    auto def = CCDirector::get()->getWinSize() / 2;
    
    auto pos = ccp(
        Mod::get()->getSavedValue<float>(fmt::format("{}_shortcutpos.x", mod->getID()), def.width),
        Mod::get()->getSavedValue<float>(fmt::format("{}_shortcutpos.y", mod->getID()), def.height)
    );

    this->updatePosition(pos);
    this->setPosition(pos);
}

void ModuleShortcutButton::updatePosition(cocos2d::CCPoint point)
{
    FloatingUIButton::updatePosition(point);
    position = point;

    Mod::get()->setSavedValue<float>(fmt::format("{}_shortcutpos.x", mod->getID()), point.x);
    Mod::get()->setSavedValue<float>(fmt::format("{}_shortcutpos.y", mod->getID()), point.y);
}

void ModuleShortcutButton::updateSprs()
{
    auto bg = mod->shouldShortcutShowActivated() ? 
        "geode.loader/baseCircle_Medium_Green.png" : 
        "geode.loader/baseCircle_Medium_Gray.png";

    updateSprites(bg, overlaySprite, true, false);
}

void ModuleShortcutButton::setOverlaySprite(std::string spr)
{
    this->overlaySprite = spr;
    updateSprs();
}

void ModuleShortcutButton::update(float dt)
{
    if (lastUpdated != mod->shouldShortcutShowActivated())
    {
        lastUpdated = mod->shouldShortcutShowActivated();
        updateSprs();
    }
    
    FloatingUIButton::update(dt);
}