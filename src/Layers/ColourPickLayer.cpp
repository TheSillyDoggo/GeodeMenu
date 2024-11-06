/*#include "IconOptionsLayer.h"

void IconOptionsLayer::customSetup()
{
    ok->m_pfnSelector = menu_selector(IconOptionsLayer::onClose); // sexy
}

void IconOptionsLayer::onColour(CCObject* sender)
{
    auto popup = ColorPickPopup::create((as<CCNode*>(sender)->getTag() == 1) ? startFade : endFade);
    popup->setTag(as<CCNode*>(sender)->getTag());
    popup->show();
    popup->setDelegate(this);
}

void IconOptionsLayer::onClose(CCObject* sender)
{
    Mod::get()->setSavedValue<ccColor3B>(fmt::format("fadeColour1{}", icon), startFade);
    Mod::get()->setSavedValue<ccColor3B>(fmt::format("fadeColour2{}", icon), endFade);

    SillyBaseLayer::onClose(sender);
}

void IconOptionsLayer::updateColor(cocos2d::ccColor4B const& color)
{
    if (auto popup = getChildByType<ColorPickPopup>(CCScene::get(), 0))
    {
        auto col = ccc3(color.r, color.g, color.b);

        if (popup->getTag() == 1)
            startFade = col;
        else
            endFade = col;

        spr1->setColor(startFade);
        spr2->setColor(endFade);
    }
}*/