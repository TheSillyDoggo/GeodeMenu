#include "SetupColourConfigUI.hpp"
#include "../Utils/ColourUtils.hpp"
#include "BlurLayer.hpp"
#include "BetterButtonSprite.hpp"

SetupColourConfigUI* SetupColourConfigUI::create(std::function<void(ColourConfig)> onFinishFunc, bool allowEffects)
{
    auto pRet = new SetupColourConfigUI();

    CCSize size = ccp(360, 250);
    pRet->onFinishFunc = onFinishFunc;
    pRet->allowEffects = allowEffects;

    if (pRet && pRet->initAnchored(size.width, size.height))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool SetupColourConfigUI::setup()
{
    this->addChild(CCBlurLayer::create(), -3);
    this->scheduleUpdate();

    m_bgSprite->setVisible(false);
    bg = BackgroundSprite::create();
    bg->setContentSize(this->m_size);

    m_buttonMenu->setVisible(false);
    m_mainLayer->addChildAtPosition(bg, Anchor::Center);

    auto title = AdvLabelBMFont::createWithLocalisation("colour-setup/title", "goldFont.fnt");
    title->setScale(0.7f);

    auto menu = CCMenu::create();

    auto spr = BetterButtonSprite::createWithLocalisation(ccp(54.25f, 30), "ui/ok-button", "goldFont.fnt", "GJ_button_01.png");
    auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(SetupColourConfigUI::onClose));
    menu->addChild(btn);

    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, -18));
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));



    startColour = CCLayerColor::create(ccc4(0, 0, 0, 255), 30, 15);
    startColour->ignoreAnchorPointForPosition(false);
    endColour = CCLayerColor::create(ccc4(0, 0, 0, 255), 30, 30);
    endColour->ignoreAnchorPointForPosition(false);

    // auto prev = GameManager::sharedState()->m_levelEditorLayer;

    // if (Loader::get()->getLoadedMod("flow.betterpicker") && !prev) // im sorry
        // GameManager::sharedState()->m_levelEditorLayer = reinterpret_cast<LevelEditorLayer*>(0xB00B5);

    picker = CCControlColourPicker::colourPicker();
    picker->setDelegate(this);
    picker->setAnchorPoint(ccp(0, 0));
    picker->setScale(0.8f);

    // GameManager::sharedState()->m_levelEditorLayer = prev;

    typeMenu = CCMenu::create();
    typeMenu->setContentSize(ccp(0, 0));
    typeMenu->setScale(0.8f);
    typeMenu->setVisible(allowEffects);

    auto topRightMenu = CCMenu::create();
    topRightMenu->setContentSize(ccp(0, 300));
    topRightMenu->setAnchorPoint(ccp(0.5f, 1));
    topRightMenu->setLayout(AxisLayout::create(Axis::Column)->setAutoScale(false)->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End)->setGap(10));
    topRightMenu->setScale(0.75f);
    topRightMenu->addChild(CCMenuItemSpriteExtra::create(BetterButtonSprite::createWithLocalisation(ccp(72, 30), "ui/default-button", "goldFont.fnt", "GJ_button_04.png"), this, menu_selector(SetupColourConfigUI::onSetDefault)));
    topRightMenu->addChild(CCMenuItemSpriteExtra::create(BetterButtonSprite::createWithLocalisation(ccp(72, 30), "ui/undo-button", "goldFont.fnt", "GJ_button_04.png"), this, menu_selector(SetupColourConfigUI::onUndoChanged)));

    gradientAddStepBtn = CCMenuItemSpriteExtra::create(BetterButtonSprite::createWithLocalisation(ccp(72, 30), "colour-setup/add-step", "goldFont.fnt", "GJ_button_04.png"), this, menu_selector(SetupColourConfigUI::onAddGradientStep));
    gradientDelStepBtn = CCMenuItemSpriteExtra::create(BetterButtonSprite::createWithLocalisation(ccp(72, 30), "colour-setup/remove-step", "goldFont.fnt", "GJ_button_04.png"), this, menu_selector(SetupColourConfigUI::onDeleteGradientStep));
    topRightMenu->addChild(gradientAddStepBtn);
    topRightMenu->addChild(gradientDelStepBtn);

    topRightMenu->updateLayout();

    createGradientPreview();
    addTypeButtons(typeMenu);

    updateUI();

    m_mainLayer->addChildAtPosition(picker, Anchor::Center, ccp(0, 0));
    m_mainLayer->addChildAtPosition(endColour, Anchor::TopLeft, ccp(30, -30));
    m_mainLayer->addChildAtPosition(startColour, Anchor::TopLeft, ccp(30, -30 - (15 * 1.5f)));
    m_mainLayer->addChildAtPosition(typeMenu, Anchor::BottomRight, ccp(-80, 0));
    m_mainLayer->addChildAtPosition(topRightMenu, Anchor::TopRight, ccp(-38, -12));

    return true;
}

void SetupColourConfigUI::createGradientPreview()
{
    gradientPreviewContainer = CCNode::create();
    gradientPreviewContainer->setContentSize(ccp(180, 50));
    gradientPreviewContainer->setAnchorPoint(ccp(0.5f, 0.5f));
    gradientPreviewContainer->setZOrder(6);

    int quality = 400;
    for (size_t i = 0; i < quality; i++)
    {
        float v = (float)i / (float)quality;
        float xPos = v * gradientPreviewContainer->getContentWidth();

        auto spr = CCLayerColor::create(ccc4(0, 0, 0, 255), gradientPreviewContainer->getContentWidth() - xPos, gradientPreviewContainer->getContentHeight());
        spr->setAnchorPoint(ccp(0, 0));
        spr->ignoreAnchorPointForPosition(false);
        spr->setPosition(ccp(xPos, 0));

        gradientPreviewSprites.emplace(spr, v);
        gradientPreviewContainer->addChild(spr);
    }

    gradientTimePreview = createGradientLine(true);
    gradientPreviewContainer->addChild(gradientTimePreview, 69);

    updateGradientLines();

    gradientLineConfigNode = CCMenu::create();
    gradientLineConfigNode->setTouchPriority(-508);

    auto lblColour = AdvLabelBMFont::createWithLocalisation("colour-setup/colour-hint", "bigFont.fnt");
    lblColour->setAnchorPoint(ccp(0, 0.5f));
    lblColour->setScale(0.45f);
    lblColour->setPosition(ccp(-50, -17));
    gradientLineConfigNode->addChild(lblColour);

    gradientLineColour = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    gradientLineColour->setScale(0.65f);

    auto btn = CCMenuItemSpriteExtra::create(gradientLineColour, this, menu_selector(SetupColourConfigUI::onChangeGradientLineColour));
    btn->setPosition(ccp(30, -17));
    gradientLineConfigNode->addChild(btn);

    gradientLineLocation = BetterSlider::create(this, menu_selector(SetupColourConfigUI::onGradientLocationSlider));
    gradientLineLocation->setScale(0.5f);
    gradientLineLocation->setPosition(ccp(0, 7));
    gradientLineLocation->setContentWidth(gradientPreviewContainer->getContentWidth() / 0.5f);
    gradientLineLocation->getFill()->setOpacity(0);
    gradientLineLocation->setTouchPriority(-520);
    gradientLineConfigNode->addChild(gradientLineLocation);
    
    m_mainLayer->addChildAtPosition(gradientPreviewContainer, Anchor::Center, ccp(0, 30));
    m_mainLayer->addChildAtPosition(gradientLineConfigNode, Anchor::Center, ccp(0, -35));
}

CCNode* SetupColourConfigUI::createGradientLine(bool timePreview, ccColor3B col, bool selected, int tag)
{
    auto n = CCNode::create();
    n->setContentSize(ccp(1, 50));
    n->setAnchorPoint(ccp(0.5f, 0));

    auto layer = CCLayerColor::create(ccc4(0, 0, 0, 200), n->getContentWidth(), n->getContentHeight());
    n->addChild(layer);

    if (timePreview)
    {
        layer->setColor(ccc3(0, 119, 255));

        auto spr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
        spr->setAnchorPoint(ccp(0, 0.5f));
        spr->setRotation(-90);
        spr->setScale(0.3f);
        spr->setPosition(ccp(n->getContentWidth() / 2, n->getContentHeight()));

        n->addChild(spr);
    }
    else
    {
        layer->setOpacity(100);

        auto menu = CCMenu::create();
        menu->setPosition(ccp(n->getContentWidth() / 2, -12));
        menu->setTouchPriority(-512);

        auto spr = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
        spr->setColor(col);
        spr->setScale(0.4f);
        spr->setOpacity(selected ? 255 : 175);

        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(SetupColourConfigUI::onSelectGradientLine));
        btn->setTag(tag);

        menu->addChild(btn);
        n->addChild(menu);
    }

    return n;
}

void SetupColourConfigUI::updateGradientPreview()
{
    for (auto spr : gradientPreviewSprites)
    {
        spr.first->setColor(currentConfig.colourForGradient(spr.second));
    }

    for (auto line : gradientLines)
    {
        line->setPositionX(gradientPreviewContainer->getContentWidth() * currentConfig.gradientLocations[line->getTag()].percentageLocation);
    }

    gradientLineColour->setColor(currentConfig.gradientLocations[selectedGradientLine].colour);
    gradientTimePreview->setPositionX(gradientPreviewContainer->getContentWidth() * (ColourUtils::get()->getLoopedValue(ColourUtils::get()->getChannelValue(previewChannel))));
}

void SetupColourConfigUI::updateGradientLines()
{
    for (auto line : gradientLines)
    {
        line->removeFromParent();
    }

    gradientLines.clear();

    int i = 0;
    for (auto& line : currentConfig.gradientLocations)
    {
        auto l = createGradientLine(false, line.colour, i == selectedGradientLine, i);
        l->setTag(i);
        
        gradientPreviewContainer->addChild(l, 4);
        gradientLines.push_back(l);
        i++;
    }
}

void SetupColourConfigUI::onSelectGradientLine(CCObject* sender)
{
    selectedGradientLine = sender->getTag();

    updateGradientLines();
}

void SetupColourConfigUI::onChangeGradientLineColour(CCObject* sender)
{
    auto ui = SetupColourConfigUI::create([this](ColourConfig conf)
    {
        currentConfig.gradientLocations[selectedGradientLine].colour = conf.customColour;
        updateGradientLines();
    }, false);

    ui->setStartConfig({ currentConfig.gradientLocations[selectedGradientLine].colour });
    ui->setDefaultConfig({ ccc3(255, 0, 0) });
    ui->setPreviewChannel("");
    ui->show();
}

void SetupColourConfigUI::onAddGradientStep(CCObject* sender)
{
    float x = 0.5f;

    currentConfig.gradientLocations.push_back({
        currentConfig.colourForGradient(x),
        x
    });

    selectedGradientLine = currentConfig.gradientLocations.size() - 1;

    updateGradientLines();
}

void SetupColourConfigUI::onDeleteGradientStep(CCObject* sender)
{
    if (currentConfig.gradientLocations.size() == 1)
    {
        FLAlertLayer::create("Gradient Steps", "There must be at least <cc>one</c> gradient step", "OK")->show();
        return;
    }

    currentConfig.gradientLocations.erase(currentConfig.gradientLocations.begin() + selectedGradientLine);

    selectedGradientLine = 0;

    updateGradientLines();
}

void SetupColourConfigUI::onGradientLocationSlider(CCObject* sender)
{
    currentConfig.gradientLocations[selectedGradientLine].percentageLocation = gradientLineLocation->getValue01();
}

void SetupColourConfigUI::addTypeButtons(CCMenu* menu)
{
    for (size_t i = 1; i < 6 + 1; i++)
    {
        std::string labelText = "";

        switch ((ColourConfigType)i)
        {
            case Player1:
                labelText = "colour-setup/player-primary";
                break;

            case Player2:
                labelText = "colour-setup/player-secondary";
                break;

            case PlayerGlow:
                labelText = "colour-setup/player-glow";
                break;

            case Chroma:
                labelText = "colour-setup/chroma";
                break;

            case Pastel:
                labelText = "colour-setup/pastel";
                break;

            case Gradient:
                labelText = "colour-setup/gradient";
                break;
        }

        float yPos = 30 * (6 - (i - 1));

        auto toggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(SetupColourConfigUI::onChangeType), 0.75f);
        toggler->setPosition(ccp(0, yPos));

        auto label = AdvLabelBMFont::createWithLocalisation(labelText.c_str(), "bigFont.fnt");
        label->setAnchorPoint(ccp(0, 0.5f));
        label->setPosition(ccp(18, yPos));
        label->limitLabelWidth(65, 0.35f, 0);

        menu->addChild(toggler);
        menu->addChild(label);
        configTypes.emplace(toggler, (ColourConfigType)i);
    }
    
    updateTypeButtons(nullptr);
}

void SetupColourConfigUI::updateTypeButtons(CCMenuItemToggler* excluding)
{
    for (auto type : configTypes)
    {
        if (type.first == excluding)
            continue;
        
        type.first->toggle(type.second == currentConfig.type);
    }

    for (auto child : CCArrayExt<CCNode*>(typeMenu->getChildren()))
    {
        if (currentConfig.type == Gradient)
        {
            child->setVisible(child->getPositionY() == 30);
        }
        else
        {
            child->setVisible(true);
        }
    }
}

void SetupColourConfigUI::onChangeType(CCObject* sender)
{
    auto btn = static_cast<CCMenuItemToggler*>(sender);

    if (currentConfig.type == configTypes[btn])
        currentConfig.type = CustomColour;
    else
        currentConfig.type = configTypes[btn];

    updateTypeButtons(btn);
    updateUI();
}

void SetupColourConfigUI::update(float dt)
{
    // currentConfig.customColour = picker->getColorValue();

    startColour->setColor(startConfig.colourForConfig(previewChannel));
    endColour->setColor(currentConfig.colourForConfig(previewChannel));

    gradientLineLocation->setValue01(currentConfig.gradientLocations[selectedGradientLine].percentageLocation);

    updateGradientPreview();
}

void SetupColourConfigUI::onClose(CCObject* sender)
{
    if (onFinishFunc)
        onFinishFunc(currentConfig);

    geode::Popup<>::onClose(sender);
}

void SetupColourConfigUI::onSetDefault(CCObject* sender)
{
    this->currentConfig = defaultConfig;
    selectedGradientLine = 0;

    updateUI();
    updateTypeButtons(nullptr);
}

void SetupColourConfigUI::onUndoChanged(CCObject* sender)
{
    this->currentConfig = startConfig;
    selectedGradientLine = 0;

    updateUI();
    updateTypeButtons(nullptr);
}

void SetupColourConfigUI::setStartConfig(ColourConfig config)
{
    this->startConfig = config;
    this->currentConfig = config;

    updateUI();
    updateTypeButtons(nullptr);
}

void SetupColourConfigUI::setDefaultConfig(ColourConfig config)
{
    this->defaultConfig = config;
}

void SetupColourConfigUI::setPreviewChannel(std::string channel)
{
    this->previewChannel = channel;
}

void SetupColourConfigUI::updateUI()
{
    // auto prev = GameManager::sharedState()->m_levelEditorLayer;

    // if (Loader::get()->getLoadedMod("flow.betterpicker") && !prev) // im sorry
        // GameManager::sharedState()->m_levelEditorLayer = reinterpret_cast<LevelEditorLayer*>(0xB00B5);

    picker->setColorValue(currentConfig.customColour);
    picker->setTouchEnabled(currentConfig.type == CustomColour);
    picker->setVisible(currentConfig.type != Gradient);
    gradientPreviewContainer->setVisible(currentConfig.type == Gradient);
    gradientLineConfigNode->setVisible(currentConfig.type == Gradient);
    gradientAddStepBtn->setVisible(currentConfig.type == Gradient);
    gradientDelStepBtn->setVisible(currentConfig.type == Gradient);
    updateGradientLines();

    // GameManager::sharedState()->m_levelEditorLayer = prev;
}

void SetupColourConfigUI::colorValueChanged(ccColor3B colour)
{

}
