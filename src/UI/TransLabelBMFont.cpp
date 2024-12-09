#include "TransLabelBMFont.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/TranslationManager.hpp"

bool TransLabelBMFont::init(std::string text, std::string font)
{
    if (!CCNode::init())
        return false;

    text = TranslationManager::get()->getTranslatedString(text);

    if (text == "Noclip")
        text = "ノークリップ";

    this->text = text;
    this->font = text;

    label = CCLabelBMFont::create(text.c_str(), font.c_str());
    ttf = CCLabelTTF::create("", "Arial.ttf", 27.5f);

    label->setAnchorPoint(ccp(0, 0));
    ttf->setAnchorPoint(ccp(0, 0));

    updateTTFVisible();

    this->addChild(label);
    this->addChild(ttf);

    return true;
}

void TransLabelBMFont::limitLabelWidth(float width, float defaultScale, float minScale)
{
    this->setScale(clamp<float>(width / getContentWidth(), minScale, defaultScale));
}

void TransLabelBMFont::updateTTFVisible()
{
    useTtf = false;

    for (auto letter : this->text)
    {
        if (!label->getConfiguration()->getCharacterSet()->contains(as<int>(letter)))
        {
            useTtf = true;
            break;
        }
    }

    if (useTtf)
        ttf->setString(text.c_str());

    label->setVisible(!useTtf);
    ttf->setVisible(useTtf);

    this->setContentSize(useTtf ? ttf->getScaledContentSize() : label->getScaledContentSize());
}

void TransLabelBMFont::setOpacity(int opacity)
{
    label->setOpacity(opacity);
    ttf->setOpacity(opacity);
}

TransLabelBMFont* TransLabelBMFont::create(std::string text, std::string font)
{
    auto pRet = new TransLabelBMFont();

    if (pRet && pRet->init(text, font))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}