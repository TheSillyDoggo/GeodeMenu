#include "InputModuleNode.hpp"

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
    auto mod = as<InputModule*>(module);

    this->scheduleUpdate();

    auto label = CCLabelBMFont::create(fmt::format("{}{}", mod->getName(), mod->appendColon() ? ":" : "").c_str(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->limitLabelWidth(75, 0.575f, 0);

    input = TextInput::create(90 + (75 - label->getScaledContentWidth()), mod->getPlaceholderString());
    input->getInputNode()->setUserObject("disable-char-bypass"_spr, CCNode::create());
    input->setMaxCharCount(mod->getMaxCharCount());
    input->setFilter(mod->getStringFilter());
    input->setString(mod->getString());
    input->setDelegate(this);
    input->setScale(0.775f);
    input->setAnchorPoint(ccp(1, 0.5f));

    this->addChildAtPosition(label, Anchor::Left, ccp(4, 0));
    this->addChildAtPosition(input, Anchor::Right, ccp(-10, 0));
}

void InputModuleNode::textChanged(CCTextInputNode* p0)
{
    auto mod = as<InputModule*>(module);

    mod->setString(input->getString());

    ModuleNode::updateAllNodes(this);
}

void InputModuleNode::updateNode()
{
    auto mod = as<InputModule*>(module);

    this->input->setString(mod->getString());
}

void InputModuleNode::update(float dt)
{
    auto mod = as<InputModule*>(module);

    if (input->getInputNode()->getString().size() > 0)
        input->getInputNode()->m_textLabel->setString(utils::string::replace(mod->getDisplayFilter(), "%s", input->getString()).c_str());
}