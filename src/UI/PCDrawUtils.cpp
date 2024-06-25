#include "PCDrawUtils.hpp"

void PCDrawUtils::drawRect(CCPoint pos, CCPoint size, ccColor4B colour)
{
    auto wndSize = CCDirector::get()->getWinSize();

    ccDrawSolidRect(ccp(pos.x, wndSize.height - pos.y), ccp(pos.x, wndSize.height - pos.y) + ccp(size.x, -size.y), ccc4FFromccc4B(colour));
}

CCPoint PCDrawUtils::getMousePosition()
{
    return ccp(cocos::getMousePos().x, CCDirector::get()->getWinSize().height - cocos::getMousePos().y);
}