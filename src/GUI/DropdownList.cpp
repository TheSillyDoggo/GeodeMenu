#include "DropdownList.hpp"

/*
auto test = DropdownList::create({DropdownItem({"questsBtn.png"_spr, "i send you on a quest to find yuri"}), DropdownItem({"heart.png"_spr, "yuri yuri yuri"}), DropdownItem({"keybinds.png"_spr, "use this keyboard to find yuri"}), DropdownItem({"discord.png"_spr, "send me yuri on this platform"})});
test->setPosition(ccp(150, 150));

this->addChild(test);
*/

DropdownList* DropdownList::create(std::vector<DropdownItem> items)
{
    auto pRet = new DropdownList();

    if (pRet && pRet->init(items))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void DropdownList::addItem(DropdownItem item)
{
    float inset = 3.5f;

    auto container = CCNode::create();
    container->setContentSize(this->getContentSize());
    
    if (!item.icon.empty())
    {
        CCSprite* spr = CCSprite::create(item.icon.c_str());

        if (spr)
        {
            spr->setAnchorPoint(ccp(0, 0.5f));
            spr->setScale((getContentHeight() - inset * 2) / spr->getContentHeight());

            container->addChildAtPosition(spr, Anchor::Left, ccp(inset, 0));
        }

        auto lbl = CCLabelBMFont::create(item.label.c_str(), "bigFont.fnt");
        lbl->setAnchorPoint(ccp(0, 0.5f));

        // とても怖い
        lbl->limitLabelWidth(getContentWidth() - inset - inset - (spr ? (spr->getScaledContentWidth() + inset) : 0), 0.5f, 0);

        container->addChildAtPosition(lbl, Anchor::Left, ccp(inset + (spr ? (spr->getScaledContentWidth() + inset) : 0), 0));
    }

    auto btn = CCMenuItemSpriteExtra::create(container, this, nullptr);
    btn->setPosition(getContentSize() / 2 - ccp(0, getContentHeight() * (items.size() + 1)));
    this->addChild(btn);

    items.emplace(btn, item);
}

void DropdownList::setSelected(DropdownItem item)
{
    int i = 0;
    for (auto item2 : items)
    {
        if ((item2.second.icon == item.icon) && (item2.second.label == item.label))
        {
            setSelectedIndex(i);

            return;
        }

        i++;
    }
}

void DropdownList::setSelectedIndex(int index)
{
    this->selectedIndex = index;
    updateSelection();
}

DropdownItem DropdownList::getSelected()
{
    return {};
}

int DropdownList::getSelectedIndex()
{
    return selectedIndex;
}

void DropdownList::updateSelection()
{

}

bool DropdownList::init(std::vector<DropdownItem> items)
{
    if (!CCMenu::init())
        return false;

    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(ccp(0.5f, 0.5f));
    this->setContentSize(ccp(150, 25));
    this->setPosition(ccp(0, 0));

    bg = CCScale9Sprite::create("square02_small.png");
    bg->setAnchorPoint(ccp(0, 1));
    bg->setPosition(ccp(0, getContentHeight()));
    bg->setContentWidth(getContentWidth());
    bg->setOpacity(100);

    this->addChild(bg);

    for (auto item : items)
    {
        addItem(item);
    }

    bg->setContentHeight(getContentHeight() * (items.size() + 1));

    updateSelection();

    return true;
}