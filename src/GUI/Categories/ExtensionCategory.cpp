#include "ExtensionCategory.hpp"

ExtensionCategory* ExtensionCategory::create(qolmod::ext::CategoryData* data)
{
    auto pRet = new ExtensionCategory();

    if (pRet && pRet->init(data))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool ExtensionCategory::init(qolmod::ext::CategoryData* data)
{
    if (!CategoryNode::init())
        return false;

    this->data = data;

    if (data->customFunc)
    {
        menu = CCMenu::create();
        menu->setAnchorPoint(ccp(0.5f, 0.5f));
        menu->ignoreAnchorPointForPosition(false);
        menu->setPosition(getContentSize() / 2);
        menu->setContentSize(getContentSize());

        data->customFunc(menu);

        this->addChild(menu);
        this->setLayout(CopySizeLayout::create()->add(menu));
    }
    
    return true;
}