#include "SetupColourConfigUI.hpp"
#include "../Utils/ColourUtils.hpp"

#include "BetterButtonSprite.hpp"

SetupColourConfigUI* SetupColourConfigUI::create(std::function<void(ColourConfig)> onFinishFunc, bool allowEffects)
{
    auto pRet = new SetupColourConfigUI();

    CCSize size = ccp(380, 250);
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

    shader = CCNodeWithShader::create();

    picker = CCControlColourPicker::colourPicker();
    picker->setDelegate(this);
    picker->setAnchorPoint(ccp(0, 0));
    picker->setScale(0.8f);
    shader->addChild(picker);

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

    bottomLeft = CCMenu::create();
    bottomLeft->setVisible(this->allowEffects);
    bottomLeft->setScale(0.65f);
    bottomLeft->setContentSize(ccp(0, 0));

    speedSlider = BetterSlider::create(this, menu_selector(SetupColourConfigUI::onSpeedSliderChanged));
    speedSlider->setRange(0.1f, 5);
    speedSlider->setSnapValuesRanged({1.0f});
    speedSlider->setPosition(ccp(0, 0));
    speedSlider->setContentWidth(150);
    bottomLeft->addChild(speedSlider);

    speedInput = BetterInputNode::create(45, "Speed");
    speedInput->setAlignment(kCCTextAlignmentCenter);
    speedInput->setAnchorPoint(ccp(0, 0.5f));
    speedInput->setPosition(ccp(speedSlider->getContentWidth() / 2 + 12, 0));
    speedInput->setDelegate(this);
    bottomLeft->addChild(speedInput);

    auto speedTitle = AdvLabelBMFont::createWithLocalisation("colour-setup/effect-speed", "goldFont.fnt");
    speedTitle->setPosition(ccp(((speedSlider->getScaledContentWidth() + 12 + speedInput->getScaledContentWidth()) / 2) - speedSlider->getScaledContentWidth() / 2, 38));
    speedTitle->limitLabelWidth((speedSlider->getScaledContentWidth() + 12 + speedInput->getScaledContentWidth()) - 10, 0.85f, 0);
    bottomLeft->addChild(speedTitle);

    colArea = CCNode::create();

    rInput = BetterInputNode::create(50, "R");
    rInput->setAlignment(kCCTextAlignmentCenter);
    rInput->setTag(1);
    rInput->setScale(0.6f);
    rInput->setPosition(ccp(-38, 20));
    rInput->setDelegate(this);

    gInput = BetterInputNode::create(50, "G");
    gInput->setAlignment(kCCTextAlignmentCenter);
    gInput->setTag(2);
    gInput->setScale(0.6f);
    gInput->setPosition(ccp(0, 20));
    gInput->setDelegate(this);

    bInput = BetterInputNode::create(50, "B");
    bInput->setAlignment(kCCTextAlignmentCenter);
    bInput->setTag(3);
    bInput->setScale(0.6f);
    bInput->setPosition(ccp(38, 20));
    bInput->setDelegate(this);

    hexInput = BetterInputNode::create(110, "HEX");
    hexInput->setAlignment(kCCTextAlignmentCenter);
    hexInput->setTag(4);
    hexInput->setScale(0.6f);
    hexInput->setPosition(ccp(0, -20));
    hexInput->setDelegate(this);

    auto rTitle = AdvLabelBMFont::createWithLocalisation("colour-setup/red", "goldFont.fnt");
    rTitle->setScale(0.5f);
    rTitle->setPosition(rInput->getPosition() + ccp(0, 19));

    auto gTitle = AdvLabelBMFont::createWithLocalisation("colour-setup/green", "goldFont.fnt");
    gTitle->setScale(0.5f);
    gTitle->setPosition(gInput->getPosition() + ccp(0, 19));

    auto bTitle = AdvLabelBMFont::createWithLocalisation("colour-setup/blue", "goldFont.fnt");
    bTitle->setScale(0.5f);
    bTitle->setPosition(bInput->getPosition() + ccp(0, 19));

    auto hexTitle = AdvLabelBMFont::createWithLocalisation("colour-setup/hex", "goldFont.fnt");
    hexTitle->setScale(0.5f);
    hexTitle->setPosition(hexInput->getPosition() + ccp(0, 19));

    colArea->addChild(rInput);
    colArea->addChild(gInput);
    colArea->addChild(bInput);
    colArea->addChild(hexInput);
    colArea->addChild(rTitle);
    colArea->addChild(gTitle);
    colArea->addChild(bTitle);
    colArea->addChild(hexTitle);

    createGradientPreview();
    addTypeButtons(typeMenu);

    updateUI();

    m_mainLayer->addChildAtPosition(shader, Anchor::Center, ccp(0, 0));
    m_mainLayer->addChildAtPosition(endColour, Anchor::TopLeft, ccp(30, -30));
    m_mainLayer->addChildAtPosition(startColour, Anchor::TopLeft, ccp(30, -30 - (15 * 1.5f)));
    m_mainLayer->addChildAtPosition(typeMenu, Anchor::BottomRight, ccp(-80, 0));
    m_mainLayer->addChildAtPosition(topRightMenu, Anchor::TopRight, ccp(-38, -12));
    m_mainLayer->addChildAtPosition(bottomLeft, Anchor::BottomLeft, ccp(65, 25));
    m_mainLayer->addChildAtPosition(colArea, Anchor::Left, ccp(65, 0));

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
    gradientTimePreview->setPositionX(gradientPreviewContainer->getContentWidth() * (ColourUtils::get()->getLoopedValue(ColourUtils::get()->getChannelValue(fmt::format("{}_preview", previewChannel)))));
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

            case CustomColour:
                labelText = "meowwww meeowwwwww meowwww mrrp mrrowwwwww~~";
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
    startColour->setColor(startConfig.colourForConfig(previewChannel));
    endColour->setColor(currentConfig.colourForConfig(fmt::format("{}_preview", previewChannel)));

    gradientLineLocation->setValue01(currentConfig.gradientLocations[selectedGradientLine].percentageLocation);

    updateGradientPreview();
}

void SetupColourConfigUI::onClose(CCObject* sender)
{
    if (onFinishFunc)
        onFinishFunc(currentConfig);

    PopupBase::onClose(sender);
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
    picker->setColorValue(currentConfig.customColour);
    picker->setTouchEnabled(currentConfig.type == CustomColour);
    picker->setVisible(currentConfig.type != Gradient);
    gradientPreviewContainer->setVisible(currentConfig.type == Gradient);
    gradientLineConfigNode->setVisible(currentConfig.type == Gradient);
    gradientAddStepBtn->setVisible(currentConfig.type == Gradient);
    gradientDelStepBtn->setVisible(currentConfig.type == Gradient);
    updateGradientLines();
    speedSlider->setValueRanged(currentConfig.chromaSpeed);
    speedInput->setString(utils::numToString<double>(currentConfig.chromaSpeed, 2));
    bottomLeft->setVisible(this->allowEffects && (currentConfig.type >= Chroma));
    colArea->setVisible(currentConfig.type != Gradient);
    updateInputs(nullptr);
}

void SetupColourConfigUI::onSpeedSliderChanged(CCObject* sender)
{
    currentConfig.chromaSpeed = speedSlider->getValueRanged();
    speedInput->setString(utils::numToString<double>(currentConfig.chromaSpeed, 2));
}

void SetupColourConfigUI::colorValueChanged(ccColor3B colour)
{
    static bool ig = false;

    if (ig)
        return;

    ig = true;

    this->currentConfig.customColour = colour;
    updateInputs(nullptr);

    ig = false;
}

void SetupColourConfigUI::textChanged(CCTextInputNode* node)
{
    if (node->getTag() > 0)
    {
        switch (node->getTag())
        {
            case 1:
                currentConfig.customColour.r = utils::numFromString<int>(rInput->getString()).unwrapOr(currentConfig.customColour.r);
                updateInputs(rInput);
                break;

            case 2:
                currentConfig.customColour.g = utils::numFromString<int>(gInput->getString()).unwrapOr(currentConfig.customColour.g);
                updateInputs(gInput);
                break;

            case 3:
                currentConfig.customColour.b = utils::numFromString<int>(bInput->getString()).unwrapOr(currentConfig.customColour.g);
                updateInputs(bInput);
                break;

            case 4:
                currentConfig.customColour = cc3bFromHexString(hexInput->getString(), true).unwrapOr(currentConfig.customColour);
                updateInputs(hexInput);
                break;
        }

        return;
    }

    currentConfig.chromaSpeed = utils::numFromString<double>(speedInput->getString()).unwrapOr(currentConfig.chromaSpeed);
    speedSlider->setValueRanged(currentConfig.chromaSpeed);
}

void SetupColourConfigUI::updateInputs(BetterInputNode* except)
{
    if (except != rInput)
        rInput->setString(utils::numToString<int>(currentConfig.customColour.r));

    if (except != gInput)
        gInput->setString(utils::numToString<int>(currentConfig.customColour.g));

    if (except != bInput)
        bInput->setString(utils::numToString<int>(currentConfig.customColour.b));

    if (except != hexInput)
        hexInput->setString(cc3bToHexString(currentConfig.customColour));

    picker->setColorValue(currentConfig.customColour);
}