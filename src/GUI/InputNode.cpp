#include "InputNode.hpp"
#include "../Utils/Num.hpp"
#include "../Utils/RealtimeAction.hpp"
#include "LocalisationManager.hpp"
#include <FloatingButton/FloatingUIManager.hpp>
#include <Utils.hpp>

using namespace geode::prelude;
using namespace qolmod;

InputNode* InputNode::create(float width, std::string placeholder, ZStringView font) {
    auto ret = new InputNode();
    if (ret->initWithWidth(width, std::move(placeholder), font)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

InputNode* InputNode::create(CCSize const& size, std::string placeholder, ZStringView font) {
    auto ret = new InputNode();
    if (ret->initWithSize(size, std::move(placeholder), font, false)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

InputNode* InputNode::createMultiline(CCSize const& size, std::string placeholder, ZStringView font) {
    auto ret = new InputNode();
    if (ret->initWithSize(size, std::move(placeholder), font, true)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool InputNode::initWithWidth(float width, std::string placeholder, ZStringView font)
{
    return this->initWithSize({width, 0}, std::move(placeholder), font, false);
}

bool InputNode::initWithSize(CCSize const& size, std::string placeholder, ZStringView font, bool multiline)
{
    if (!TextInput::initWithSize(size, placeholder, font, multiline))
        return false;

    getBGSprite()->setVisible(false);
    auto bg = EasyBG::create();
    bg->setTargettingNode(this);
    bg->setAnchorPoint(ccp(0, 0));

    this->setAlignment(Label::Alignment::Left);
    this->setFontScale(0.5f);
    this->setCaretWidth(2);

    setFilter("");

    this->addChild(bg, -67);
    return true;
}

void InputNode::setLanguageFonts()
{
    this->getContentLabel()->setFont(LocalisationManager::get()->getAltFont());
    this->getPlaceholderLabel()->setFont(LocalisationManager::get()->getAltFont());
}