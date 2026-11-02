#include "ScrollLayer.hpp"

using namespace geode::prelude;
using namespace qolmod;

qolmod::ScrollLayer* qolmod::ScrollLayer::create(cocos2d::CCSize const& size, bool scrollWheelEnabled, bool vertical)
{
    auto ret = new qolmod::ScrollLayer(CCRectMake(0, 0, size.width, size.height), scrollWheelEnabled, vertical);
    ret->autorelease();
    return ret;
}

qolmod::ScrollLayer::ScrollLayer(cocos2d::CCRect const& rect, bool scrollWheelEnabled, bool vertical) : geode::ScrollLayer::ScrollLayer(rect, scrollWheelEnabled, vertical)
{
    m_peekLimitTop = 15;
    m_peekLimitBottom = 15;

    this->setUserObject("user95401.scrollbar_everywhere/scrollbar", CCNode::create());
}