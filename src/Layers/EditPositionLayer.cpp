#include "EditPositionLayer.hpp"

void EditPositionLayer::customSetup()
{
    if (blur)
        blur->removeFromParent();

    baseLayer->removeFromParent();
    this->setZOrder(69);
    this->setTouchEnabled(true);
    this->scheduleUpdate();

    auto bg = CCLayerColor::create(ccc4(51, 68, 153, 255));

    auto okSpr = ButtonSprite::create("OK", 40, 0, 1, true, "goldFont.fnt", "GJ_button_01.png", 32);
    auto resetSpr = ButtonSprite::create("Reset", 40, 0, 1, true, "goldFont.fnt", "GJ_button_04.png", 32);

    auto okBtn = CCMenuItemSpriteExtra::create(okSpr, this, menu_selector(EditPositionLayer::onClose));
    auto resetBtn = CCMenuItemSpriteExtra::create(resetSpr, this, menu_selector(EditPositionLayer::onReset));

    okBtn->setPositionX(-40);
    resetBtn->setPositionX(40);

    auto topMenu = CCMenu::create();
    topMenu->setPosition(ccp(CCDirector::get()->getWinSize().width / 2, CCDirector::get()->getWinSize().height - 30));
    topMenu->addChild(okBtn);
    topMenu->addChild(resetBtn);

    node = getNodeForType();

    nodeOuter = CCMenu::create();
    nodeOuter->ignoreAnchorPointForPosition(false);
    nodeOuter->setContentSize(ccp(175, 55));

    this->addChild(bg);
    this->addChild(topMenu);
    this->addChild(node);
    this->addChild(nodeOuter);
}

void EditPositionLayer::onClose(CCObject*)
{
    if (type == EditPositionType::StartposSwitcher)
    {
        Mod::get()->setSavedValue<float>("startpos-position.x", position.x);
        Mod::get()->setSavedValue<float>("startpos-position.y", position.y);
    }

    as<SillyBaseLayer*>(this->getParent())->onClose(nullptr);
}

void EditPositionLayer::onReset(CCObject*)
{
    if (type == EditPositionType::StartposSwitcher)
    {
        Mod::get()->setSavedValue<float>("startpos-position.x", CCDirector::get()->getWinSize().width / 2);
        Mod::get()->setSavedValue<float>("startpos-position.y", 25);
    }

    position = ccp(Mod::get()->getSavedValue<float>("startpos-position.x", CCDirector::get()->getWinSize().width / 2), Mod::get()->getSavedValue<float>("startpos-position.y", 25));
}

void EditPositionLayer::update(float dt)
{
    label->setOpacity(opacity * 255);
    left->setOpacity(opacity * 255);
    right->setOpacity(opacity * 255);
    
    node->setScale(scale);
    node->setPosition(position);
    nodeOuter->setPosition(position);
}

CCMenu* EditPositionLayer::getNodeForType()
{
    if (type == EditPositionType::StartposSwitcher)
    {
        position = ccp(Mod::get()->getSavedValue<float>("startpos-position.x", CCDirector::get()->getWinSize().width / 2), Mod::get()->getSavedValue<float>("startpos-position.y", 25));
        scale = Mod::get()->getSavedValue<float>("startpos-scale", 1);
        opacity = Mod::get()->getSavedValue<float>("startpos-opacity", 1);

        auto menu = CCMenu::create();
        menu->setContentSize(ccp(0, 0));

        label = CCLabelBMFont::create("69/420", "bigFont.fnt");
        label->limitLabelWidth(100, 0.65f, 0);

        left = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
        right = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
        left->setScale(0.65f);
        right->setScale(0.65f);

        right->setFlipX(true);

        menu->addChildAtPosition(left, Anchor::Center, ccp(-65, 0));
        menu->addChildAtPosition(right, Anchor::Center, ccp(65, 0));
        menu->addChildAtPosition(label, Anchor::Center);

        return menu;
    }

    return nullptr;
}

bool EditPositionLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (CCLayer::ccTouchBegan(touch, event))
    {
        if (nodeOuter->boundingBox().containsPoint(touch->getLocation()))
        {
            isDragging = true;
            offset = nodeOuter->getPosition() - touch->getLocation();

            return true;
        }
    }

    return false;
}

void EditPositionLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    if (isDragging)
        position = touch->getLocation() + offset;
}

void EditPositionLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    isDragging = false;
}

void EditPositionLayer::ccTouchCancelled(CCTouch* touch, CCEvent* event)
{
    isDragging = false;
}
