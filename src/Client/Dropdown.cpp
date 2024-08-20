#include "Dropdown.h"

void Dropdown::onToggleVisible(CCObject*)
{
    open = !open;

    btn->stopAllActions();

    if (open)
    {
        btn->runAction(CCEaseBackOut::create(CCScaleTo::create(0.35f, 1, -1)));

        setVis(true);
    }
    else
    {
        btn->runAction(CCEaseBackOut::create(CCScaleTo::create(0.35f, 1, 1)));

        setVis(false);
    }
}

void Dropdown::setVis(bool n)
{
    CCPoint s = ccp(size.width, size.height * (1 + (open ? strs.size() : 0)));
    bg->stopAllActions();
    #if (defined(GEODE_IS_IOS) || defined(GEODE_IS_MACOS))
    bg->setContentSize(s / 0.5f);
    #else
    bg->runAction(CCEaseInOut::create( CCContentSizeTo::create(0.35f, s / 0.5f), 2.0f));
    #endif    

    if (n)
    {
        for (size_t i = 0; i < btns.size(); i++)
        {
            btns[i]->stopAllActions();
            btns[i]->setEnabled(n);

            //btns[i]->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.1f * i), CCEaseInOut::create(CCScaleTo::create(0.5f, 1), 2)));
        }
    }
    else
    {
        std::reverse(btns.begin(), btns.end());
        
        for (size_t i = 0; i < btns.size(); i++)
        {
            btns[i]->stopAllActions();
            btns[i]->setEnabled(n);

            //btns[i]->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.1f * i), CCEaseInOut::create(CCScaleTo::create(0.35f, 0), 2)));
        }

        std::reverse(btns.begin(), btns.end());
    }
}

void Dropdown::onPress(CCObject* sender)
{
    open = false;
    setSelected(as<CCNode*>(sender)->getTag());

    btn->runAction(CCEaseBackOut::create(CCScaleTo::create(0.35f, 1, 1)));
    setVis(false);

    if (event)
        (this->*event)(sender);
}

void Dropdown::visit()
{
    auto rect = CCRect(-this->size.width, -this->bg->getContentHeight() / 2 + this->size.height, this->size.width * 3, this->bg->getContentHeight() / 2);

    //glEnable(0xc11);
    //CCEGLView::sharedOpenGLView()->setScissorInPoints(rect.getMinX(), rect.getMinY(), rect.getMaxX(), rect.getMaxY());
    GameToolbox::preVisitWithClippingRect(this, rect);

    CCMenu::visit();

    glDisable(0xc11);
}

bool Dropdown::init(CCSize size, std::vector<std::string> strs, cocos2d::SEL_MenuHandler callback, int sel)
{
    if (!CCMenu::init())
        return false;

    this->size = size;
    this->strs = strs;
    this->event = callback;

    this->setContentSize(size);
    bg = CCScale9Sprite::create("square02b_small.png");
    bg->setAnchorPoint(ccp(0, 0));
    bg->setContentSize(size / 0.5f);
    bg->setScale(0.5f);
    bg->setColor(ccc3(0, 0, 0));
    bg->setOpacity(200);
    bg->setPositionY(size.height);
    bg->setScaleY(-1 * 0.5f);
    this->addChild(bg);

    sprBtn = CCSprite::createWithSpriteFrameName("edit_downBtn_001.png");
    sprBtn->setColor({200, 200, 200});
    sprBtn2 = CCSprite::createWithSpriteFrameName("edit_downBtn_001.png");
    btn = CCMenuItemSpriteExtra::create(sprBtn, sprBtn2, this, menu_selector(Dropdown::onToggleVisible));
    btn->setSelectedImage(sprBtn2);
    btn->m_scaleMultiplier = 1;
    btn->m_animationEnabled = false;
    btn->setPosition(size + ccp(-7.5f - sprBtn->getContentSize().width / 2, -1 * (size.height / 2)));
    this->addChild(btn);

    tex = CCLabelBMFont::create(strs[sel].c_str(), "bigFont.fnt");
    tex->setPosition(size / 2 + ccp(-1 * sprBtn->getContentSize().width, 0) + ccp(10 / 2, 0));
    tex->limitLabelWidth(size.width - 10 - (7.5f + sprBtn->getContentSize().width), 0.7f, 0.05f);
    this->addChild(tex);
    
    for (size_t s = 0; s < strs.size(); s++)
    {
        auto lbl = CCLabelBMFont::create(strs[s].c_str(), "bigFont.fnt");
        lbl->limitLabelWidth(size.width - 10 - 10, 0.7f, 0.01f);
        lbl->setColor(ccc3(200, 200, 200));

        auto btn = CCMenuItemSpriteExtra::create(lbl, this, menu_selector(Dropdown::onPress));
        btn->setPosition(ccp(size.width / 2, (size.height * (s + 1) * -1) + size.height / 2 ));
        btn->setScale(1);
        btn->setTag(s);
        btn->setEnabled(false);
        this->addChild(btn);

        btns.push_back(btn);
    }            

    this->scheduleUpdate();

    this->setTouchPriority(-503);
    this->registerWithTouchDispatcher();

    return true;
}

void Dropdown::registerWithTouchDispatcher()
{
    CCTouchDispatcher::get()->addTargetedDelegate(this, -512, true);
}

Dropdown* Dropdown::create(CCSize size, std::vector<std::string> strs, cocos2d::SEL_MenuHandler callback, int sel) {
    Dropdown* ret = new Dropdown();
    if (ret && ret->init(size, strs, callback, sel)) {
        ret->autorelease();
        return ret;
    } else {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
}