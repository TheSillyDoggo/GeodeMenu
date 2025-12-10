#include "SetupColourCOnfigUI.hpp"

SetupColourConfigUI* SetupColourConfigUI::create(std::function<void(ColourConfig)> onFinishFunc)
{
    auto pRet = new SetupColourConfigUI();

    CCSize size = ccp(360, 250);
    pRet->onFinishFunc = onFinishFunc;

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

    auto title = CCLabelBMFont::create("Select Colour", "goldFont.fnt");
    title->setScale(0.7f);

    auto menu = CCMenu::create();

    auto btn = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK"), this, menu_selector(SetupColourConfigUI::onClose));
    menu->addChild(btn);

    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, -18));
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));



    startColour = CCLayerColor::create(ccc4(0, 0, 0, 255), 30, 15);
    startColour->ignoreAnchorPointForPosition(false);
    endColour = CCLayerColor::create(ccc4(0, 0, 0, 255), 30, 30);
    endColour->ignoreAnchorPointForPosition(false);

    picker = CCControlColourPicker::colourPicker();
    picker->setDelegate(this);
    picker->setAnchorPoint(ccp(0, 0));
    picker->setScale(0.8f);

    auto typeMenu = CCMenu::create();
    typeMenu->setContentSize(ccp(0, 0));
    typeMenu->setScale(0.8f);

    auto topRightMenu = CCMenu::create();
    topRightMenu->setContentSize(ccp(0, 0));
    topRightMenu->setScale(0.75f);
    topRightMenu->addChild(CCMenuItemSpriteExtra::create(ButtonSprite::create("Default", 56, 56, 0.6f, false, "goldFont.fnt", "GJ_button_04.png", 30), this, menu_selector(SetupColourConfigUI::onSetDefault)));

    updateUI();
    addTypeButtons(typeMenu);

    m_mainLayer->addChildAtPosition(picker, Anchor::Center, ccp(0, 0));
    m_mainLayer->addChildAtPosition(endColour, Anchor::TopLeft, ccp(30, -30));
    m_mainLayer->addChildAtPosition(startColour, Anchor::TopLeft, ccp(30, -30 - (15 * 1.5f)));
    m_mainLayer->addChildAtPosition(typeMenu, Anchor::BottomRight, ccp(-80, 0));
    m_mainLayer->addChildAtPosition(topRightMenu, Anchor::TopRight, ccp(-38, -22));

    return true;
}

void SetupColourConfigUI::addTypeButtons(CCMenu* menu)
{
    for (size_t i = 1; i < 5 + 1; i++)
    {
        std::string labelText = "";

        switch ((ColourConfigType)i)
        {
            case Player1:
                labelText = "Player\nPrimary";
                break;

            case Player2:
                labelText = "Player\nSecondary";
                break;

            case PlayerGlow:
                labelText = "Player\nGlow";
                break;

            case Chroma:
                labelText = "Chroma";
                break;

            case Pastel:
                labelText = "Pastel";
                break;
        }

        float yPos = 30 * (5 - (i - 1));

        auto toggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(SetupColourConfigUI::onChangeType), 0.75f);
        toggler->setPosition(ccp(0, yPos));

        auto label = CCLabelBMFont::create(labelText.c_str(), "bigFont.fnt");
        label->setAnchorPoint(ccp(0, 0.5f));
        label->setPosition(ccp(18, yPos));
        label->setScale(0.35f);

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
}

void SetupColourConfigUI::onChangeType(CCObject* sender)
{
    auto btn = static_cast<CCMenuItemToggler*>(sender);

    if (currentConfig.type == configTypes[btn])
        currentConfig.type = CustomColour;
    else
        currentConfig.type = configTypes[btn];

    updateTypeButtons(btn);
}

void SetupColourConfigUI::update(float dt)
{
    currentConfig.customColour = picker->getColorValue();

    startColour->setColor(startConfig.colourForConfig(previewChannel));
    endColour->setColor(currentConfig.colourForConfig(previewChannel));
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

    updateUI();
}

void SetupColourConfigUI::setStartConfig(ColourConfig config)
{
    this->startConfig = config;
    this->currentConfig = config;

    updateUI();
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

    updateTypeButtons(nullptr);
}

void SetupColourConfigUI::colorValueChanged(ccColor3B colour)
{

}