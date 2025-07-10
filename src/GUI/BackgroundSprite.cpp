#include "BackgroundSprite.hpp"
#include "../Utils/Casts.hpp"

bool BackgroundSprite::init()
{
    if (!CCNode::init())
        return false;

    colouredBG = CCScale9Sprite::create("GJ_square01.png");
    outlineSpr = CCScale9Sprite::create("GJ_square07.png");

    gradientBG = CCLayerGradient::create(ccc4(0, 0, 0, 255), ccc4(0, 0, 0, 255));
    gradientBG->ignoreAnchorPointForPosition(false);

    clippingStencil = CCScale9Sprite::create("GJ_square01.png");
    clipping = CCClippingNode::create(clippingStencil);
    clipping->setAlphaThreshold(0.03f);

    auto gm = GameManager::get();
    gradientBG->setStartColor(gm->colorForIdx(gm->m_playerColor.value()));
    gradientBG->setEndColor(gm->colorForIdx(gm->m_playerColor2.value()));

    gradientOutline = CCScale9Sprite::create("GJ_square07.png");
    gradientDarken = CCScale9Sprite::create("GJ_square01.png");

    gradientDarken->setColor(ccColor3B(0, 0, 0));
    gradientDarken->setOpacity(30);
    gradientDarken->setScale(0.5f);

    clipping->addChild(gradientBG);
    clipping->addChild(gradientDarken);

    updateTheme(Mod::get()->getSavedValue<int>("theme", -5));

    this->addChildAtPosition(colouredBG, Anchor::Center);
    this->addChildAtPosition(clipping, Anchor::Center);
    this->addChildAtPosition(outlineSpr, Anchor::Center);
    this->addChildAtPosition(gradientOutline, Anchor::Center);
    return true;
}

void BackgroundSprite::updateTheme(int theme)
{
    this->theme = theme;

    auto bgStr = fmt::format("GJ_square0{}.png", theme < 0 ? 6 : theme);

    if (theme == -4)
        bgStr = "geode.loader/GE_square01.png";

    if (theme == -5)
        bgStr = "geode.loader/GE_square02.png";

    if (theme == -6)
        bgStr = "geode.loader/GE_square03.png";

    public_cast(colouredBG, _scale9Image)->setTexture(CCSprite::create(bgStr.c_str())->getTexture());

    colouredBG->setColor(theme == -2 ? ccc3(0, 0, 0) : ccc3(255, 255, 255));
    colouredBG->setOpacity(theme == -2 ? 175 : 255);

    colouredBG->setVisible(theme != -1);
    outlineSpr->setVisible(theme == -2);

    clipping->setVisible(theme == -1);
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