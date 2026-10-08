#include "ShortcutGroupButton.hpp"

ShortcutGroupButton* ShortcutGroupButton::create(uint8_t group)
{
    auto pRet = new ShortcutGroupButton();

    pRet->groupID = group;
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void ShortcutGroupButton::onClick()
{
    open = !open;
}

bool ShortcutGroupButton::isOpen()
{
    return open;
}