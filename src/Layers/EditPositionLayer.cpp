#include "EditPositionLayer.hpp"

void EditPositionLayer::customSetup()
{
    this->removeAllChildren();
    this->setZOrder(69);
    this->setTouchEnabled(true);
    this->scheduleUpdate();

    bg = CCLayerColor::create(ccc4(51, 68, 153, 255));

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

    if (type == EditPositionType::StartposSwitcher)
    {
        scaleSlider = Slider::create(this, menu_selector(EditPositionLayer::sliderValueChanged));
        scaleSlider->getThumb()->setTag(1);
        scaleSlider->setValue(unscaleValue(scale));
        scaleSlider->setScale(0.55f);
        scaleSlider->setAnchorPoint(ccp(0, 0));
        scaleSlider->setPosition(ccp(CCDirector::get()->getWinSize().width / 2 + 55, CCDirector::get()->getWinSize().height - 70));

        opacitySlider = Slider::create(this, menu_selector(EditPositionLayer::sliderValueChanged));
        opacitySlider->getThumb()->setTag(2);
        opacitySlider->setValue(opacity);
        opacitySlider->setScale(0.55f);
        opacitySlider->setAnchorPoint(ccp(0, 0));
        opacitySlider->setPosition(ccp(CCDirector::get()->getWinSize().width / 2 + 55, CCDirector::get()->getWinSize().height - 70 - 30));

        auto scaleLabel = CCLabelBMFont::create("Scale:", "chatFont.fnt");
        scaleLabel->setAnchorPoint(ccp(1, 0.5f));
        scaleLabel->setScale(0.85f);
        scaleLabel->setPositionY(scaleSlider->getPositionY() + 1);
        scaleLabel->setPositionX(CCDirector::get()->getWinSize().width / 2 - 15);

        auto opacityLabel = CCLabelBMFont::create("Opacity:", "chatFont.fnt");
        opacityLabel->setAnchorPoint(ccp(1, 0.5f));
        opacityLabel->setScale(0.85f);
        opacityLabel->setPositionY(opacitySlider->getPositionY() + 1);
        opacityLabel->setPositionX(CCDirector::get()->getWinSize().width / 2 - 15);

        this->addChild(scaleSlider, 5);
        this->addChild(scaleLabel, 5);
        this->addChild(opacitySlider, 5);
        this->addChild(opacityLabel, 5);
    }
    
    auto topRightMenu = CCMenu::create();
    topRightMenu->setPosition(CCDirector::get()->getWinSize() + ccp(-32, -35));
    topRightMenu->addChild(CCMenuItemToggler::createWithStandardSprites(this, menu_selector(EditPositionLayer::onTogglePreview), 0.75f));

    auto previewLabel = CCLabelBMFont::create("Preview", "bigFont.fnt");
    previewLabel->setPosition(topRightMenu->getPosition() + ccp(0, 23));
    previewLabel->setScale(0.4f);

    previewBG = CCSprite::create("screenshot.png"_spr);
    previewBG->setScale(CCDirector::get()->getWinSize().width / previewBG->getContentWidth());
    previewBG->setPosition(CCDirector::get()->getWinSize() / 2);
    previewBG->setVisible(false);

    this->addChild(bg);
    this->addChild(previewBG);
    this->addChild(topMenu, 3);
    this->addChild(topRightMenu, 3);
    this->addChild(previewLabel, 3);
    this->addChild(node, 1);
    this->addChild(nodeOuter, 2);
}

void EditPositionLayer::onClose(CCObject*)
{
    if (type == EditPositionType::StartposSwitcher)
    {
        Mod::get()->setSavedValue<float>("startpos-position.x", position.x);
        Mod::get()->setSavedValue<float>("startpos-position.y", position.y);

        Mod::get()->setSavedValue<float>("startpos-scale", scale);
        Mod::get()->setSavedValue<float>("startpos-opacity", opacity);
    }

    as<SillyBaseLayer*>(this->getParent())->onClose(nullptr);
}

void EditPositionLayer::onReset(CCObject*)
{
    if (type == EditPositionType::StartposSwitcher)
    {
        Mod::get()->setSavedValue<float>("startpos-position.x", CCDirector::get()->getWinSize().width / 2);
        Mod::get()->setSavedValue<float>("startpos-position.y", 25);
        Mod::get()->setSavedValue<float>("startpos-scale", 1);
        Mod::get()->setSavedValue<float>("startpos-opacity", 75.0f / 255.0f);

        position = ccp(Mod::get()->getSavedValue<float>("startpos-position.x", CCDirector::get()->getWinSize().width / 2), Mod::get()->getSavedValue<float>("startpos-position.y", 25));
        scale = Mod::get()->getSavedValue<float>("startpos-scale", 1);
        opacity = Mod::get()->getSavedValue<float>("startpos-opacity", 75.0f / 255.0f);

        scaleSlider->setValue(unscaleValue(1));
        opacitySlider->setValue(75.0f / 255.0f);
    }
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
        opacity = Mod::get()->getSavedValue<float>("startpos-opacity", 75.0f / 255.0f);

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

void EditPositionLayer::sliderValueChanged(CCObject* sender)
{
    if (sender->getTag() == 1)
        scale = scaleValue(as<SliderThumb*>(sender)->getValue());

    if (sender->getTag() == 2)
        opacity = as<SliderThumb*>(sender)->getValue();
}

void EditPositionLayer::onTogglePreview(CCObject*)
{
    previewBG->setVisible(!previewBG->isVisible());
    bg->setColor(previewBG->isVisible() ? ccc3(0, 0, 0) : ccc3(51, 68, 153));
}

float EditPositionLayer::scaleValue(float originalValue) {
    float minValue = 0.4f;
    float maxValue = 1.5f;
    float scaledValue = (maxValue - minValue) * originalValue + minValue;
    return scaledValue;
}

float EditPositionLayer::unscaleValue(float scaledValue) {
    float minValue = 0.4f;
    float maxValue = 1.5f;
    float originalValue = (scaledValue - minValue) / (maxValue - minValue);
    return originalValue;
}