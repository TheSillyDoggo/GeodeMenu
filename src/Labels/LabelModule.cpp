#include "LabelModule.hpp"
#include "Labels.h"

LabelModule::LabelModule(std::string format, std::string font)
{
    this->format = format;
    this->font = font;
}

void LabelModule::setFont(std::string newFont)
{
    this->font = newFont;

    if (labelNode)
        labelNode->setFntFile(getFont().c_str());
}

std::string LabelModule::getFont()
{
    return CCFileUtils::sharedFileUtils()->isFileExist(CCFileUtils::sharedFileUtils()->fullPathForFilename(font.c_str(), false)) ? font : "bigFont.fnt";
}

void LabelModule::setScale(float newScale)
{
    this->scale = newScale;

    if (labelNode)
    {
        labelNode->setScale(newScale * 0.5f);
        labelNode->getParent()->updateLayout();
    }
}

float LabelModule::getScale()
{
    return scale;
}

void LabelModule::setOpacity(float newOpacity)
{
    this->opacity = newOpacity;

    if (labelNode)
    {
        labelNode->setScale(newOpacity * 255);
        labelNode->getParent()->updateLayout();
    }
}

float LabelModule::getOpacity()
{
    return opacity;
}

void LabelModule::setSide(LabelSide newSide)
{
    this->side = newSide;

    if (StatusNode::get())
    {
        StatusNode::get()->reorderSides();
    }
}

LabelSide LabelModule::getSide()
{
    return side;
}
