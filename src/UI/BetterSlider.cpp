#include "BetterSlider.hpp"

bool BetterSlider::init(CCObject* sender, SEL_MenuHandler selector)
{
    if (!CCLayer::init())
        return false;

    this->ignoreAnchorPointForPosition(false);
    this->setContentSize(ccp(210, 16));

    ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE};

    bg = CCScale9Sprite::create("slidergroove.png");
    fill = CCSprite::create("sliderBar.png");
    fill->getTexture()->setTexParameters(&params);
    fill->setTextureRect(CCRectMake(0, 0, getContentSize().width, getContentSize().height));

    this->setLayout(CopySizeLayout::create()->add(bg)->add(fill));

    this->addChild(fill);
    this->addChild(bg);
    return true;
}

BetterSlider* BetterSlider::create(CCObject* sender, SEL_MenuHandler selector)
{
    auto pRet = new BetterSlider();
    
    if (pRet && pRet->init(sender, selector))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}