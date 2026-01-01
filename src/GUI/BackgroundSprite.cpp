#include "BackgroundSprite.hpp"
#include "../Utils/Casts.hpp"
#include "Modules/GradientBGColours.hpp"

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
    clipping = CCClippingNode::create(clippingStencil);
    clipping->setAlphaThreshold(0.03f);

    gradientOutline = CCScale9Sprite::create("GJ_square07.png");
    gradientDarken = CCScale9Sprite::create("GJ_square01.png");

    gradientDarken->setColor(ccc3(0, 0, 0));
    gradientDarken->setOpacity(30);
    gradientDarken->setScale(0.5f);

    clipping->addChild(gradientBG);
    clipping->addChild(gradientDarken);

    setTheme(Mod::get()->getSavedValue<int>("theme", -6));

    this->addChildAtPosition(colouredBG, Anchor::Center);
    this->addChildAtPosition(clipping, Anchor::Center);
    this->addChildAtPosition(outlineSpr, Anchor::Center);
    this->addChildAtPosition(gradientOutline, Anchor::Center);
    return true;
}

void BackgroundSprite::setTheme(int theme)
{
    this->theme = theme;

    auto bgStr = fmt::format("GJ_square0{}.png", theme < 0 ? 6 : theme);

    if (theme == -4)
        bgStr = "geode.loader/GE_square01.png";

    if (theme == -5)
        bgStr = "geode.loader/GE_square02.png";

    if (theme == -6)
        bgStr = "geode.loader/GE_square03.png";

    colouredBG->_scale9Image->setTexture(CCSprite::create(bgStr.c_str())->getTexture());

    colouredBG->setColor(theme == -2 ? ccc3(0, 0, 0) : ccc3(255, 255, 255));
    colouredBG->setOpacity(theme == -2 ? 175 : 255);

    colouredBG->setVisible(theme != -1);
    outlineSpr->setVisible(theme == -2);

    clipping->setVisible(theme == -1);
    clipping->getStencil()->setVisible(theme == -1);
    gradientOutline->setVisible(theme == -1);
    gradientDarken->setVisible(theme == -1 && gradientDarkenVisible);
}

void BackgroundSprite::setGradientDarkenVisible(bool visible)
{
    gradientDarkenVisible = visible;

    gradientDarken->setVisible(theme == -1 && gradientDarkenVisible);
}

void BackgroundSprite::setContentSize(const CCSize& contentSize)
{
    CCNode::setContentSize(contentSize);

    colouredBG->setContentSize(contentSize);
    outlineSpr->setContentSize(contentSize);

    clippingStencil->setContentSize(contentSize - ccp(2, 2));
    gradientBG->setContentSize(contentSize);
    gradientOutline->setContentSize(contentSize);
    gradientDarken->setContentSize((contentSize - ccp(15, 15)) / 0.5f);
}

void BackgroundSprite::update(float dt)
{
    gradientBG->setStartColor(GradientBGStart::get()->getColour());
    gradientBG->setEndColor(GradientBGEnd::get()->getColour());
    gradientBG->setVector(GradientBGDirection::get()->getRotationVector());
}

void BackgroundSprite::setOpacity(float opacity)
{
    colouredBG->setOpacity((theme == -2 ? 175.0f : 255.0f) * (opacity / 255.0f));
    outlineSpr->setOpacity(opacity);
    gradientBG->setOpacity(opacity);
    gradientOutline->setOpacity(opacity);
    gradientDarken->setOpacity(opacity);
}

void BackgroundSprite::setColour(ccColor3B colour)
{
    colouredBG->setColor(theme == -2 ? ccc3(0, 0, 0) : colour);
    outlineSpr->setColor(colour);
    gradientOutline->setColor(colour);
}