#include "ModuleNode.hpp"
#include "../GUI/Categories/FavouritesNode.hpp"
#include "../GUI/OptionsUI.hpp"
#include "../GUI/Modules/SeperateColourCheatNames.hpp"
#include "../Utils/ColourUtils.hpp"
#include "../GUI/EditKeyConfigUI.hpp"
#include "../GUI/BlurLayer.hpp"
#include "../GUI/BetterAlertLayer.hpp"

ModuleNode* ModuleNode::create(Module* module)
{
    auto pRet = new ModuleNode();

    if (pRet && pRet->init(module))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void ModuleNode::setup()
{
    bool hasDesc = module->getDescription().size() > 0;
    bool hasOptions = module->getOptions().size() > 0;
    bool isDisabled = module->isDisabled();

    btn = CCMenuItemToggler::createWithStandardSprites(this, isDisabled ? menu_selector(ModuleNode::onToggleError) : menu_selector(ModuleNode::onToggle), 0.75f);
    btn->toggle(module->getUserEnabled());

    label = AdvLabelBMFont::createWithString(module->getName(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->limitLabelWidth(110 - (hasOptions ? 3 : 0), 0.5f, 0.1f);

    favBtn = CCMenuItemToggler::create(CCSprite::create("favourites.png"_spr), CCSprite::create("favourites.png"_spr), this, menu_selector(ModuleNode::onToggleFavourite));
    favBtn->setScale(0.65f);
    favBtn->toggle(module->isFavourited());
    favBtn->setVisible(false);

    favBtn->m_offButton->setColor(ccc3(150, 150, 150));
    favBtn->m_offButton->setOpacity(150);

    if (isDisabled)
    {
        label->setOpacity(150);
        btn->m_onButton->setOpacity(150);
        btn->m_offButton->setOpacity(150);
    }

    this->schedule(schedule_selector(ModuleNode::onUpdateLabelColour));
    onUpdateLabelColour(0);

    this->addChildAtPosition(btn, Anchor::Left, ccp(15, 0));
    this->addChildAtPosition(label, Anchor::Left, ccp(30, 0));
    this->addChildAtPosition(favBtn, Anchor::Left, ccp(30 + label->getScaledContentWidth() + 9, -8));

    if (hasDesc && !hasOptions)
    {
        auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        infoSpr->setScale(0.55f + 0.35f);

        auto infoBtn = CCMenuItemSpriteExtra::create(infoSpr, this, menu_selector(ModuleNode::onInfo));

        infoSpr->setScale(infoSpr->getScale() - 0.35f);

        this->addChildAtPosition(infoBtn, Anchor::Left, ccp(30 + label->getScaledContentWidth() + 9, 5));
    }

    if (hasOptions)
    {
        auto optionsSpr = CCSprite::createWithSpriteFrameName("GJ_plus3Btn_001.png");
        auto optionsBtn = CCMenuItemSpriteExtra::create(optionsSpr, this, menu_selector(ModuleNode::onOptions));

        this->addChildAtPosition(optionsBtn, Anchor::Left, ccp(label->getScaledContentWidth() + 43, 0));
    }

    if (hasDesc)
    {
        // tooltips integration
        // auto tooltipBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"), this, nullptr);
        // tooltipBtn->setAnchorPoint(ccp(0, 0));
        // tooltipBtn->setContentSize(getContentSize());
        // this->addChild(tooltipBtn);
        // tooltipBtn->setUserObject("alphalaneous.tooltips/tooltip", CCString::create("ASDFUISDFGSDFGIYU"));
    }
}

void ModuleNode::onUpdateLabelColour(float dt)
{
    auto col = SeperateColourCheatNames::get()->getRealEnabled() && (module->getSafeModeTrigger() != SafeModeTrigger::None) ? CheatNameColour::get()->getColour() : ccWHITE;

    label->setColor(col);
}

void ModuleNode::updateNode()
{
    if (btn->isToggled() != module->getUserEnabled())
        btn->toggle(module->getUserEnabled());

    if (favBtn->isToggled() != module->isFavourited())
        favBtn->toggle(module->isFavourited());
}

void ModuleNode::onChangeKeybind(CCObject* sender)
{
    auto mod = static_cast<Module*>(this->getUserData());

    auto ui = EditKeyConfigUI::create([this, mod](KeyConfigStruct config)
    {
        mod->setKeybind(config);
    });

    ui->setDefaultConfig({ {}, Keycode::KEY_Unknown });
    ui->setStartConfig(mod->getKeybind());
    ui->show();
}

void ModuleNode::onToggle(CCObject* sender)
{
    if (module->showDisableWarning() && module->getUserEnabled())
    {
        Loader::get()->queueInMainThread([this]
        {
            FLAlertLayer::create("Warning", module->getOnDisableWarning(), "OK")->show();
            Mod::get()->setSavedValue<bool>(fmt::format("{}_disablewarningshown", getID()), true);

            updateAllNodes(nullptr);
        });

        return;
    }

    if (module->showEnableWarning() && !module->getUserEnabled())
    {
        Loader::get()->queueInMainThread([this]
        {
            FLAlertLayer::create("Warning", module->getOnEnableWarning(), "OK")->show();
            Mod::get()->setSavedValue<bool>(fmt::format("{}_enablewarningshown", getID()), true);

            updateAllNodes(nullptr);
        });

        return;
    }

    module->setUserEnabled(!module->getUserEnabled());
    updateAllNodes(this);

    module->onToggle();
}

void ModuleNode::onToggleError(CCObject* sender)
{
    auto toggler = static_cast<CCMenuItemToggler*>(sender);
    toggler->toggle(!toggler->isToggled());

    toggler->m_onButton->setScale(1);
    toggler->m_offButton->setScale(1);

    toggler->m_onButton->stopAllActions();
    toggler->m_offButton->stopAllActions();

    FLAlertLayer::create(module->getName().c_str(), module->getDisabledMessage(), "OK")->show();
}

void ModuleNode::onToggleFavourite(CCObject* sender)
{
    module->setFavourited(!module->isFavourited());
    updateAllNodes(this);

    FavouritesNode::get()->refresh();
}

void ModuleNode::onInfoToggleFavourite(CCObject* sender)
{
    // 'this' is the alert in the context of this function

    auto mod = static_cast<Module*>(this->getUserData());

    mod->setFavourited(!mod->isFavourited());

    if (FavouritesNode::get())
        FavouritesNode::get()->refresh();
}

void ModuleNode::onOptions(CCObject* sender)
{
    OptionsUI::create(module)->show();
}

void ModuleNode::onInfo(CCObject* sender)
{
    auto alert = BetterAlertLayer::createWithLocalisation(fmt::format("names/{}", getID()).c_str(), fmt::format("descriptions/{}", getID()), "ui/ok-button");
    alert->addChild(CCBlurLayer::create(), -3);
    alert->setUserData(module);
    alert->show();

    auto menu = CCMenu::create();
    // この二行は怖いだ
    menu->setPosition(CCDirector::get()->getWinSize() / 2 - ccp(0, alert->m_mainLayer->getChildByType<CCScale9Sprite>(0)->getContentHeight() / 2) + ccp(0, 25));
    menu->setTouchPriority(-42069);

    auto btn = CCMenuItemToggler::create(CCSprite::create("favourites.png"_spr), CCSprite::create("favourites.png"_spr), alert, menu_selector(ModuleNode::onInfoToggleFavourite));
    btn->setPositionX(-alert->m_mainLayer->getChildByType<CCScale9Sprite>(0)->getContentWidth() / 2 + 25);
    btn->toggle(module->isFavourited());

    btn->setContentSize(btn->getContentSize() * 3);

    btn->m_offButton->setContentSize(btn->getContentSize());
    btn->m_offButton->setPosition(btn->getContentSize() / 2);
    btn->m_offButton->getNormalImage()->setPosition(btn->getContentSize() / 2);
    
    btn->m_onButton->setContentSize(btn->getContentSize());
    btn->m_onButton->setPosition(btn->getContentSize() / 2);
    btn->m_onButton->getNormalImage()->setPosition(btn->getContentSize() / 2);

    btn->m_offButton->setColor(ccc3(150, 150, 150));
    btn->m_offButton->setOpacity(150);

    auto btnKeybind = CCMenuItemSpriteExtra::create(CCSprite::create("keybinds.png"_spr), alert, menu_selector(ModuleNode::onChangeKeybind));
    btnKeybind->setContentSize(btnKeybind->getContentSize() * 3);
    btnKeybind->setPositionX(alert->m_mainLayer->getChildByType<CCScale9Sprite>(0)->getContentWidth() / 2 - 25);
    btnKeybind->getNormalImage()->setPosition(btnKeybind->getContentSize() / 2);

    menu->addChild(btn);
    menu->addChild(btnKeybind);
    alert->m_mainLayer->addChild(menu, 8008569);

    // title
    if (auto label = alert->m_mainLayer->getChildByType<CCLabelBMFont>(0))
    {
        label->limitLabelWidth(270, 0.9f, 0);
    }
}

bool ModuleNode::init(Module* module)
{
    if (!CCMenu::init())
        return false;

    this->setID(module->getID());

    this->setAnchorPoint(ccp(0.5f, 0.5f));
    this->ignoreAnchorPointForPosition(false);
    this->setContentSize(ccp(165, 28));

    this->module = module;
    setup();

    nodes.push_back(this);

    return true;
}

void ModuleNode::updateAllNodes(ModuleNode* except)
{
    for (auto node : nodes)
    {
        if (node != except)
            node->updateNode();
    }
}

ModuleNode::~ModuleNode()
{
    nodes.erase(std::find(nodes.begin(), nodes.end(), this));
}