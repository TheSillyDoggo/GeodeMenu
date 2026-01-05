#include "InputModuleNode.hpp"
#include "../Localisation/LocalisationManager.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"

InputModuleNode* InputModuleNode::create(InputModule* module)
{
    auto pRet = new InputModuleNode();

    if (pRet && pRet->init(module))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

InputModuleNode::~InputModuleNode()
{

}

void InputModuleNode::setup()
{
    auto mod = static_cast<InputModule*>(module);

    this->scheduleUpdate();

    label = AdvLabelBMFont::createWithString(fmt::format("{}{}", mod->getName(), mod->appendColon() ? ":" : "").c_str(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->limitLabelWidth(75, 0.5f, 0);

    input = BetterInputNode::create(getContentWidth() - 4 - label->getScaledContentWidth() - 15, mod->getPlaceholderString());
    input->setContentHeight(getContentHeight() - 2.5f * 2);
    input->setAnchorPoint(ccp(1, 0.5f));
    input->setMaxChars(mod->getMaxCharCount());
    input->setCharFilter(mod->getStringFilter());
    input->setString(mod->getString());
    input->setDelegate(this);
    input->setAnchorPoint(ccp(1, 0.5f));

    if (mod->getStringFilter() == "1234567890")
    {
        input->setNumHoldValues(true, 1, 20, utils::numFromString<int>(mod->getDefaultString()).unwrapOr(0));
    }

    if (mod->getStringFilter() == "1234567890.")
    {
        input->setNumHoldValues(true, 1.0f / 10.0f, 20.0f / 10.0f, utils::numFromString<float>(mod->getDefaultString()).unwrapOr(0));
    }

    if (!mod->getHint().empty())
    {
        auto hint = AdvLabelBMFont::createWithString(LocalisationManager::get()->getLocalisedString(fmt::format("input-hints/{}", mod->getHint())).c_str(), "bigFont.fnt");
        hint->setOpacity(150);
        hint->setScale(0.25f);
        hint->setAnchorPoint(ccp(1, 0));
        hint->setZOrder(3);

        this->addChildAtPosition(hint, Anchor::BottomRight, ccp(-2.5f, (getContentHeight() - input->getScaledContentHeight()) / 2));
    }

    this->addChildAtPosition(label, Anchor::Left, ccp(4, 0));
    this->addChildAtPosition(input, Anchor::Right, ccp(-2.5f, 0));
}

void InputModuleNode::textChanged(CCTextInputNode* p0)
{
    auto mod = static_cast<InputModule*>(module);

    mod->setString(input->getString());

    ModuleNode::updateAllNodes(this);
}

void InputModuleNode::updateNode()
{
    auto mod = static_cast<InputModule*>(module);

    this->input->setString(mod->getString());
}

void InputModuleNode::update(float dt)
{
    auto mod = static_cast<InputModule*>(module);

    // if (input->getString().size() > 0)
        // input->getInputNode()->m_textLabel->setString(utils::string::replace(mod->getDisplayFilter(), "%s", input->getString()).c_str());
}