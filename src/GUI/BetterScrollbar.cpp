#include "BetterScrollbar.hpp"

BetterScrollbar* BetterScrollbar::create(CCScrollLayerExt* layer)
{
    auto pRet = create();
    pRet->setLayer(layer);
    return pRet;
}

bool BetterScrollbar::init()
{
    if (!CCMenu::init())
        return false;

    this->ignoreAnchorPointForPosition(false);
    this->setContentWidth(8);
    this->setTouchEnabled(true);
    this->setTouchPriority(-80085);

    background = CCScale9Sprite::create("square02b_small.png");
    background->setColor(ccc3(0, 0, 0));
    background->setOpacity(100);
    background->setScale(0.3f);
    background->setAnchorPoint(ccp(0, 0));

    handleContainer = CCNode::create();

    handle = CCScale9Sprite::create("geode.loader/scrollbar.png");
    handle->setOpacity(100);
    handle->setScale(0.5f);
    handle->setAnchorPoint(ccp(0, 0));
    handleContainer->addChild(handle);

    this->addChild(background);
    this->addChild(handleContainer);
    visit();
    return true;
}

void BetterScrollbar::visit()
{
    this->setContentHeight(layer ? layer->getScaledContentHeight() : 0);

    // このコードが怖いだ
    if (layer)
    {
        float h = (layer->m_contentLayer->getContentHeight() - layer->getContentHeight());
        float handleH = dynamicHandle ? ((layer->getContentHeight() / layer->m_contentLayer->getContentHeight()) * getContentHeight() - (getInset() * 2)) : handleHeight;

        float scroll = h == 0 ? 1 : -(layer->m_contentLayer->getPositionY() / h);
        float overflow = (scroll < 0 ? -scroll : (scroll > 1 ? scroll - 1 : 0));

        handleContainer->setPosition(ccp(getInset(), getInset() + (std::clamp<float>(scroll, 0, 1) * (getContentHeight() - getInset() * 2 - handleH) + (scroll > 1 ? (handleH * overflow) : 0))));
        handleContainer->setContentSize(ccp(getContentWidth() - getInset() * 2, handleH - (handleH * overflow)));

        handle->setContentSize(handleContainer->getContentSize() / handle->getScale());
    }

    background->setContentSize(this->getContentSize() / background->getScale());

    handle->setOpacity(scrollbarSelected ? 200 : 100);
    handle->setColor(disabled ? ccc3(100, 100, 100) : ccc3(255, 255, 255));

    CCLayer::visit();
}

bool BetterScrollbar::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (disabled)
        return false;

    if (!nodeIsVisible(this))
        return false;

    auto touch = pTouch->getLocation();

    auto parent = this->getParent();
	auto bounding_box = this->boundingBox();
	CCPoint bb_min(bounding_box.getMinX(), bounding_box.getMinY());
	CCPoint bb_max(bounding_box.getMaxX(), bounding_box.getMaxY());

	auto min = parent ? parent->convertToWorldSpace(bb_min) : bb_min;
	auto max = parent ? parent->convertToWorldSpace(bb_max) : bb_max;

    parent = handleContainer->getParent();
	bounding_box = handleContainer->boundingBox();
	bb_min = ccp(bounding_box.getMinX(), bounding_box.getMinY());
	bb_max = ccp(bounding_box.getMaxX(), bounding_box.getMaxY());

	auto minHandle = parent ? parent->convertToWorldSpace(bb_min) : bb_min;
	auto maxHandle = parent ? parent->convertToWorldSpace(bb_max) : bb_max;

    if (touch.x >= min.x && touch.x <= max.x)
    {
        if (touch.y >= minHandle.y && touch.y <= maxHandle.y)
        {
            scrollbarSelected = true;

            scrollMaxDown = handleContainer->getPositionY() - inset;
            scrollMaxUp = getContentHeight() - inset * 2 - scrollMaxDown - handleContainer->getScaledContentHeight();
            scrollStart = layer->m_contentLayer->getPositionY();

            return true;
        }
    }

    return false;
}

void BetterScrollbar::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (layer)
    {
        auto touch = pTouch->getLocation();
        auto touchStart = pTouch->getStartLocation();

        auto parent = this->getParent();
        auto bounding_box = this->boundingBox();
        CCPoint bb_min(bounding_box.getMinX(), bounding_box.getMinY());
        CCPoint bb_max(bounding_box.getMaxX(), bounding_box.getMaxY());

        if (touch.y != 0)
        {
            bool moveUp = touch.y > touchStart.y;
            float prog = abs(touch.y - touchStart.y);

            if (moveUp && scrollMaxUp != 0)
            {
                float percent = std::clamp<float>(prog / scrollMaxUp, 0, 1);

                layer->m_contentLayer->setPositionY(scrollStart - ((layer->m_contentLayer->getContentHeight() + scrollStart - layer->getContentHeight()) * percent));
            }

            if (!moveUp && scrollMaxDown != 0)
            {
                float percent = std::clamp<float>(prog / scrollMaxDown, 0, 1);

                layer->m_contentLayer->setPositionY(scrollStart - (scrollStart * percent));
            }
        }

        // auto min = parent ? parent->convertToWorldSpace(bb_min) : bb_min;
        // auto max = parent ? parent->convertToWorldSpace(bb_max) : bb_max;

        // float percent = (touch.y - min.y - getInset()) / (max.y - min.y - getInset() * 2);
        // percent = clamp<float>(percent, 0, 1);

        // log::info("percent: {}", percent);

        // layer->m_contentLayer->setPositionY(percent * (layer->getContentHeight() - layer->m_contentLayer->getContentHeight()));
    }
}

void BetterScrollbar::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    scrollbarSelected = false;
}

void BetterScrollbar::setDisabled(bool disabled)
{
    this->disabled = disabled;
}

bool BetterScrollbar::isDisabled()
{
    return this->disabled;
}

void BetterScrollbar::setLayer(CCScrollLayerExt* layer)
{
    this->layer = layer;
}

CCScrollLayerExt* BetterScrollbar::getLayer()
{
    return layer;
}

void BetterScrollbar::setInset(float inset)
{
    this->inset = inset;
}

float BetterScrollbar::getInset()
{
    return this->inset;
}

void BetterScrollbar::setHandleHeight(float height)
{
    this->handleHeight = height;
}

float BetterScrollbar::getHandleHeight()
{
    return this->handleHeight;
}

void BetterScrollbar::setUseDynamicHandle(bool dynamic)
{
    this->dynamicHandle = dynamic;
}

bool BetterScrollbar::getUseDynamicHandle()
{
    return this->dynamicHandle;
}

CCScale9Sprite* BetterScrollbar::getBackground()
{
    return background;
}