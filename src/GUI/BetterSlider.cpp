#include "BetterSlider.hpp"
#include "../Client/Module.hpp"

BetterSlider* BetterSlider::create(CCNode* target, SEL_MenuHandler handler)
{
    auto pRet = create();
    pRet->target = target;
    pRet->handler = handler;

    return pRet;
}

bool BetterSlider::init()
{
    if (!CCMenu::init())
        return false;

    this->setContentSize(ccp(210, 16));
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(ccp(0.5f, 0.5f));

    groove = CCScale9Sprite::create("slidergroove.png"_spr);

    thumbContainer = CCNode::create();

    thumbSpriteNormal = CCSprite::create("sliderthumb.png");
    thumbSpriteSelected = CCSprite::create("sliderthumbsel.png");
    thumbContainer->addChild(thumbSpriteNormal);
    thumbContainer->addChild(thumbSpriteSelected);

    snapContainer = CCNode::create();

    ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE};

    fill = CCSprite::create("sliderBar.png");
    fill->getTexture()->setTexParameters(&params);
    fill->setAnchorPoint(ccp(0, 0.5f));

    this->addChild(fill, -1);
    this->addChild(groove, 2);
    this->addChild(snapContainer);
    this->addChild(thumbContainer, 4);
    return true;
}

CCNode* BetterSlider::getThumb()
{
    return thumbContainer;
}

CCSprite* BetterSlider::getFill()
{
    return fill;
}

bool BetterSlider::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!nodeIsVisible(this))
        return false;

    auto touch = pTouch->getLocation();

    auto parent = thumbSpriteNormal->getParent();
	auto bounding_box = thumbSpriteNormal->boundingBox();
	CCPoint bb_min(bounding_box.getMinX(), bounding_box.getMinY());
	CCPoint bb_max(bounding_box.getMaxX(), bounding_box.getMaxY());

	auto min = parent ? parent->convertToWorldSpace(bb_min) : bb_min;
	auto max = parent ? parent->convertToWorldSpace(bb_max) : bb_max;

    if (touch.x >= min.x && touch.x <= max.x)
    {
        if (touch.y >= min.y && touch.y <= max.y)
        {
            selectOffsetX = touch.x - (min.x + ((max.x - min.x) / 2));
            selected = true;

            return true;
        }
    }

    return false;
}

void BetterSlider::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    auto touch = pTouch->getLocation();

    auto parent = this->getParent();
	auto bounding_box = this->boundingBox();
	CCPoint bb_min(bounding_box.getMinX(), bounding_box.getMinY());
	CCPoint bb_max(bounding_box.getMaxX(), bounding_box.getMaxY());

	auto min = parent ? parent->convertToWorldSpace(bb_min) : bb_min;
	auto max = parent ? parent->convertToWorldSpace(bb_max) : bb_max;

    float x = touch.x - selectOffsetX;
    float value = (x - (min.x + 5)) / ((max.x - min.x) - 10);

    if (!(bypassAllowed && Module::getByID("slider-limit")->getRealEnabled()))
        value = std::clamp<float>(value, 0, 1);

    for (size_t i = 0; i < std::min<int>(snapSprites.size(), snapValues.size()); i++)
    {
        auto posX = snapContainer->convertToWorldSpace(snapSprites[i]->getPosition()).x;

        if (x >= posX - snapRange && x <= posX + snapRange)
        {
            value = snapValues[i];
            break;
        }
    }

    setValue01(value);
    runCallback();
}

void BetterSlider::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    selected = false;
}

void BetterSlider::setSnapValues(std::vector<float> snapValues)
{
    this->snapValues = snapValues;

    updateSnapSprites();
}

void BetterSlider::setSnapValuesRanged(std::vector<float> snapValues)
{
    for (auto& snap : snapValues)
    {
        snap = (snap - rangeMin) / (rangeMax - rangeMin);
    }

    this->snapValues = snapValues;

    updateSnapSprites();
}

void BetterSlider::setUseAltSnapSprite(bool alt)
{
    this->useAltSnapSprite = alt;

    updateSnapSprites();
}

void BetterSlider::updateSnapSprites()
{
    for (auto snap : snapSprites)
    {
        snap->removeFromParent();
    }

    snapSprites.clear();

    for (auto snap : snapValues)
    {
        auto spr = CCSprite::create(useAltSnapSprite ? "slidersnap2.png"_spr : "slidersnap.png"_spr);

        if (useAltSnapSprite)
            spr->setOpacity(150);

        snapContainer->addChild(spr);
        snapSprites.push_back(spr);
    }

    snapContainer->setZOrder(useAltSnapSprite ? 1 : 3);
}

void BetterSlider::visit()
{
    groove->setPosition(getContentSize() / 2);
    groove->setContentSize(getContentSize());

    thumbContainer->setPosition(ccp(value01ToXPos(getValue01()), getContentHeight() / 2));
    thumbSpriteNormal->setVisible(!selected);
    thumbSpriteSelected->setVisible(selected);

    snapContainer->setPositionY(getContentHeight() / 2);

    for (size_t i = 0; i < std::min<int>(snapSprites.size(), snapValues.size()); i++)
    {
        snapSprites[i]->setPosition(ccp(value01ToXPos(snapValues[i]), 0));
    }

    fill->setPosition(ccp(2, getContentHeight() / 2));
    fill->setTextureRect(CCRectMake(0, 0, value01ToXPos(std::clamp<float>(getValue01(), 0, 1), 2), 8));

    CCMenu::visit();
}

void BetterSlider::setValue01(float value)
{
    this->value = value;
}

float BetterSlider::getValue01()
{
    return value;
}

void BetterSlider::setRange(float min, float max)
{
    this->rangeMin = min;
    this->rangeMax = max;
}

void BetterSlider::setValueRanged(float value)
{
    setValue01((value - rangeMin) / (rangeMax - rangeMin));
}

float BetterSlider::getValueRanged()
{
    return (rangeMax - rangeMin) * getValue01() + rangeMin;
}

float BetterSlider::value01ToXPos(float value, float inset)
{
    return inset + ((getContentWidth() - inset * 3) * value);
}

void BetterSlider::setBypassAllowed(bool bypass)
{
    this->bypassAllowed = bypass;
}

void BetterSlider::runCallback()
{
    if (target && handler)
    {
        (target->*handler)(this);
    }
}