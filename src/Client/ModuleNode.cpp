#include "ModuleNode.hpp"
#include "../GUI/Categories/FavouritesNode.hpp"
#include "../GUI/OptionsUI.hpp"
#include "../GUI/Modules/SeperateColourCheatNames.hpp"
#include "../Utils/ColourUtils.hpp"
#include "../GUI/EditKeyConfigUI.hpp"
#include "../GUI/BetterAlertLayer.hpp"
#include "../GUI/SetupShortcutUI.hpp"
#include "../SafeMode/Modules/DisableCheatsInMenu.hpp"
#include <ModuleInfoAlert.hpp>
#include <Utils.hpp>

using namespace qolmod;

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

    btn = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 0.75f);
    btn->toggle(module->getUserEnabled());

    label = AdvLabelBMFont::createWithString(module->getName(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->limitLabelWidth(110 - (hasOptions ? 3 : 0), 0.5f, 0.1f);

    this->schedule(schedule_selector(ModuleNode::onUpdateLabelColour));
    onUpdateLabelColour(0);

    this->addChildAtPosition(btn, Anchor::Left, ccp(15, 0));
    this->addChildAtPosition(label, Anchor::Left, ccp(30, 0));

    if (hasDesc && !hasOptions)
    {
        auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        infoSpr->setScale(0.55f + 0.35f);

        infoBtn = Button::create(infoSpr, this, menu_selector(ModuleNode::onInfo));

        infoSpr->setScale(infoSpr->getScale() - 0.35f);

        this->addChildAtPosition(infoBtn, Anchor::Left, ccp(30 + label->getScaledContentWidth() + 9, 5));
    }

    if (hasOptions)
    {
        auto optionsSpr = CCSprite::createWithSpriteFrameName("GJ_plus3Btn_001.png");
        auto optionsBtn = Button::create(optionsSpr, this, menu_selector(ModuleNode::onOptions));

        this->addChildAtPosition(optionsBtn, Anchor::Left, ccp(label->getScaledContentWidth() + 43, 0));
    }

    updateNode();
}

void ModuleNode::onUpdateLabelColour(float dt)
{
    auto col = SeperateColourCheatNames::get()->getRealEnabled() && (module->getSafeModeTrigger() != SafeModeTrigger::None) ? CheatNameColour::get()->getColour() : ccWHITE;

    if (isDisabled())
    {
        col.r = (float)col.r * (150.0f / 255.0f);
        col.g = (float)col.g * (150.0f / 255.0f);
        col.b = (float)col.b * (150.0f / 255.0f);
    }

    label->setColor(col);
}

void ModuleNode::updateNode()
{
    if (btn)
    {
        if (btn->isToggled() != module->getUserEnabled())
            btn->toggle(module->getUserEnabled());
    }
    
    disabled = false;
    if (SafeModeDisableCheats::get()->getRealEnabled())
    {
        if (module->getSafeModeTrigger() != SafeModeTrigger::None)
        {
            disabled = true;
        }
    }

    auto c = isDisabled() ? ccc3(150, 150, 150) : ccWHITE;

    if (btn)
    {
        btn->setTarget(this, isDisabled() ? menu_selector(ModuleNode::onToggleError) : menu_selector(ModuleNode::onToggle));
        btn->m_onButton->setColor(c);
        btn->m_offButton->setColor(c);
    }

    if (infoBtn)
        infoBtn->setColor(c);

    onUpdateLabelColour(0);
}

void ModuleNode::onToggle(CCObject* sender)
{
    if (module->showDisableWarning() && module->getUserEnabled())
    {
        Loader::get()->queueInMainThread([this]
        {
            BetterAlertLayer::create("Warning", module->getOnDisableWarning(), "OK")->show();
            Mod::get()->setSavedValue<bool>(fmt::format("{}_disablewarningshown", getID()), true);

            updateAllNodes(nullptr);
        });

        return;
    }

    if (module->showEnableWarning() && !module->getUserEnabled())
    {
        Loader::get()->queueInMainThread([this]
        {
            BetterAlertLayer::create("Warning", module->getOnEnableWarning(), "OK")->show();
            Mod::get()->setSavedValue<bool>(fmt::format("{}_enablewarningshown", getID()), true);

            updateAllNodes(nullptr);
        });

        return;
    }

    module->setUserEnabled(!module->getUserEnabled());
    module->onToggle();

    updateAllNodes(this);
}

void ModuleNode::onToggleError(CCObject* sender)
{
    auto toggler = static_cast<CCMenuItemToggler*>(sender);
    toggler->toggle(!toggler->isToggled());

    toggler->m_onButton->setScale(1);
    toggler->m_offButton->setScale(1);

    toggler->m_onButton->stopAllActions();
    toggler->m_offButton->stopAllActions();

    std::string disabledMsg = module->getDisabledMessage();

    if (SafeModeDisableCheats::get()->getRealEnabled())
    {
        if (module->getSafeModeTrigger() != SafeModeTrigger::None)
        {
            disabledMsg = "Cheats are <cr>disabled</c> by safe mode option";
        }
    }

    BetterAlertLayer::create(module->getName().c_str(), disabledMsg, "OK")->show();
}

void ModuleNode::onOptions(CCObject* sender)
{
    OptionsUI::create(module)->show();
}

void ModuleNode::onInfo(CCObject* sender)
{
    ModuleInfoAlert::create(module)->show();
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

bool ModuleNode::isDisabled()
{
    return module->isDisabled() || disabled;
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

void ModuleNode::draw()
{
    #ifndef QOLMOD_GUI_DEBUG
    return;
    #endif

    ccDrawSolidRect(ccp(0, 0), getContentSize(), ccc4f(1, 0, 0, 1));

    if (label)
    {
        auto bb = label->boundingBox();

        ccDrawSolidRect(ccp(bb.getMinX(), bb.getMinY()), ccp(bb.getMaxX(), bb.getMaxY()), ccc4f(0, 0, 1, 1));
    }
}

bool ModuleNode::altClickBegan(int button, cocos2d::CCPoint point)
{
    auto r = qolmod::utils::getBasicRect(this);
    r.origin = CCPointZero;

    if (r.containsPoint(convertToNodeSpace(point)))
    {
        BetterAlertLayer::create(getID().c_str(), fmt::format("Mouse button {} was clicked!", button), "OK")->show();
        return true;
    }

    return false;
}