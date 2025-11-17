#include "AdvLabelBMFont.hpp"

AdvLabelBMFont* AdvLabelBMFont::createWithStruct(AdvLabelStruct lblStruct, std::string font)
{
    auto pRet = new AdvLabelBMFont();

    if (pRet && pRet->initWithStruct(lblStruct, font))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

AdvLabelBMFont* AdvLabelBMFont::createWithString(std::string label, std::string font)
{
    return createWithStruct(structFromString(label), font);
}

AdvLabelStruct AdvLabelBMFont::structFromString(std::string lbl)
{
    AdvLabelStruct str;
    std::string c = "";

    for (auto ch : utils::string::split(lbl))
    {
        
    }

    return str;
}

void AdvLabelBMFont::setStruct(AdvLabelStruct str)
{
    this->str = str;

    updateLabel();
}

void AdvLabelBMFont::updateLabel()
{

}

bool AdvLabelBMFont::initWithStruct(AdvLabelStruct lblStruct, std::string font)
{
    if (!CCNode::init())
        return false;

    this->font = font;
    setStruct(lblStruct);

    return true;
}