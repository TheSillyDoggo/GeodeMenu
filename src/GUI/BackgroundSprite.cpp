#include "BackgroundSprite.hpp"
#include "../Utils/Casts.hpp"
#include "Modules/GradientBGColours.hpp"
#include <ThemeManager.hpp>

using namespace qolmod;

bool BackgroundSprite::init()
{
    if (!CCNode::init())
        return false;

    this->scheduleUpdate();

    colouredBG = CCScale9Sprite::create("GJ_square01.png");
    outlineSpr = CCScale9Sprite::create("GJ_square07.png");

    gradientBG = CCLayerGradient::create(ccc4(0, 0, 0, 255), ccc4(0, 0, 0, 255));
    gradientBG->ignoreAnchorPointForPosition(false);
    gradientBG->setVector(ccp(0, -1));
    gradientBG->setScaleX(-1);
    gradientBG->setScaleY(-1);

    clippingStencil = CCScale9Sprite::create("GJ_square01.png");
    clippingStencil2 = CCScale9Sprite::create("GJ_square01.png");
    clipping = CCClippingNode::create(clippingStencil);
    clipping->setAlphaThreshold(0.03f);

    gradientOutline = CCScale9Sprite::create("GJ_square07.png");
    gradientDarken = CCScale9Sprite::create("GJ_square01.png");

    gradientDarken->setColor(ccc3(0, 0, 0));
    gradientDarken->setOpacity(30);
    gradientDarken->setScale(0.5f);

    clipping->addChild(gradientBG);
    clipping->addChild(gradientDarken);

    clippingCustom = CCClippingNode::create(clippingStencil2);
    clippingCustom->setAlphaThreshold(0.03f);

    clippingCustom->addChild(gradientDarken);

    setTheme((int)ThemeManager::get()->getBackground());

    this->addChildAtPosition(colouredBG, Anchor::Center);
    this->addChildAtPosition(clipping, Anchor::Center);
    this->addChildAtPosition(clippingCustom, Anchor::Center);
    this->addChildAtPosition(outlineSpr, Anchor::Center);
    this->addChildAtPosition(gradientOutline, Anchor::Center);
    return true;
}

void BackgroundSprite::setTheme(int theme2)
{
    this->theme = (qolmod::BackgroundType)theme2;

    auto bgStr = fmt::format("GJ_square0{}.png", theme2 < 0 ? 6 : theme2);

    if (theme == BackgroundType::Geode01)
        bgStr = "geode.loader/GE_square01.png";

    if (theme == BackgroundType::Geode02)
        bgStr = "geode.loader/GE_square02.png";

    if (theme == BackgroundType::Geode03)
        bgStr = "geode.loader/GE_square03.png";

    colouredBG->_scale9Image->setTexture(CCSprite::create(bgStr.c_str())->getTexture());

    colouredBG->setColor(theme == BackgroundType::Darken ? ccc3(0, 0, 0) : ccc3(255, 255, 255));
    colouredBG->setOpacity(theme == BackgroundType::Darken ? 175 : 255);

    colouredBG->setVisible(theme != BackgroundType::Gradient);
    outlineSpr->setVisible(theme == BackgroundType::Darken);

    clipping->setVisible(theme == BackgroundType::Gradient);
    clipping->getStencil()->setVisible(theme == BackgroundType::Gradient);
    gradientOutline->setVisible(theme == BackgroundType::Gradient || theme == BackgroundType::Custom);
    gradientDarken->setVisible((theme == BackgroundType::Gradient || theme == BackgroundType::Custom) && gradientDarkenVisible);

    clippingCustom->setVisible(theme == BackgroundType::Custom);
    updateCustomSprite();
}

void BackgroundSprite::setGradientDarkenVisible(bool visible)
{
    gradientDarkenVisible = visible;

    gradientDarken->setVisible((theme == BackgroundType::Gradient || theme == BackgroundType::Custom) && gradientDarkenVisible);
}

void BackgroundSprite::setContentSize(const CCSize& contentSize)
{
    CCNode::setContentSize(contentSize);

    colouredBG->setContentSize(contentSize);
    outlineSpr->setContentSize(contentSize);

    clippingStencil->setContentSize(contentSize - ccp(2, 2));
    clippingStencil2->setContentSize(contentSize - ccp(2, 2));
    gradientBG->setContentSize(contentSize);
    gradientOutline->setContentSize(contentSize);
    gradientDarken->setContentSize((contentSize - ccp(15, 15)) / 0.5f);
    customImg->setScaleX(getContentWidth() / customImg->getContentWidth());
    customImg->setScaleY(getContentHeight() / customImg->getContentHeight());
}

void BackgroundSprite::update(float dt)
{
    gradientBG->setStartColor(GradientBGStart::get()->getColour());
    gradientBG->setEndColor(GradientBGEnd::get()->getColour());
    gradientBG->setVector(GradientBGDirection::get()->getRotationVector());
}

void BackgroundSprite::setOpacity(float opacity)
{
    colouredBG->setOpacity((theme == BackgroundType::Darken ? 175.0f : 255.0f) * (opacity / 255.0f));
    outlineSpr->setOpacity(opacity);
    gradientBG->setOpacity(opacity);
    gradientOutline->setOpacity(opacity);
    gradientDarken->setOpacity(opacity);
}

void BackgroundSprite::setColour(ccColor3B colour)
{
    colouredBG->setColor(theme == BackgroundType::Darken ? ccc3(0, 0, 0) : colour);
    outlineSpr->setColor(colour);
    gradientOutline->setColor(colour);
}

void BackgroundSprite::updateCustomSprite()
{
    if (customImg)
        customImg->removeFromParent();
    
    customImg = ThemeManager::get()->createCustomSprite();

    clippingCustom->addChild(customImg);
    customImg->setScaleX(getContentWidth() / customImg->getContentWidth());
    customImg->setScaleY(getContentHeight() / customImg->getContentHeight());
}