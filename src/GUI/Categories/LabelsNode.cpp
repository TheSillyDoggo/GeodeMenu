#include "LabelsNode.hpp"
#include "../../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "../../Labels/LabelManager.hpp"
#include "../../Labels/UI/LabelConfigCell.hpp"
#include "../../Utils/Num.hpp"
#include "../BetterButtonSprite.hpp"

#define TOPBAR_SIZE 25
#define PRESETS_SIZE 115
#define PRESETS_SCALE 0.575f

bool LabelsNode::init()
{
    if (!CategoryNode::init())
        return false;

    instance = this;

    bg->setPosition(ccp(0, 0));
    bg->setAnchorPoint(ccp(0, 0));
    bg->setContentSize(ccp((getContentWidth() - PRESETS_SIZE - 5) / 0.5f, bg->getContentHeight() - ((TOPBAR_SIZE + 5) / 0.5f)));

    labelsScroll = ScrollLayer::create(bg->getContentSize() * 0.5f);
    labelsScroll->m_peekLimitTop = 15;
    labelsScroll->m_peekLimitBottom = 15;
    labelsScroll->setTouchEnabled(false);
    labelsScroll->setUserObject("user95401.scrollbar_everywhere/scrollbar", CCNode::create());

    auto bgTopbar = CCScale9Sprite::create("square02b_small.png");
    bgTopbar->setContentSize(ccp(this->getContentWidth(), TOPBAR_SIZE) / 0.5f);
    bgTopbar->setScale(0.5f);
    bgTopbar->setAnchorPoint(ccp(0.5f, 1));
    bgTopbar->setColor(ccc3(0, 0, 0));
    bgTopbar->setOpacity(100);

    auto bgPresets = CCScale9Sprite::create("square02b_small.png");
    bgPresets->setContentSize(ccp(PRESETS_SIZE, bg->getScaledContentHeight()) / 0.5f);
    bgPresets->setScale(0.5f);
    bgPresets->setAnchorPoint(ccp(1, 0));
    bgPresets->setColor(ccc3(0, 0, 0));
    bgPresets->setOpacity(100);

    auto hideLabelsNode = HideLabels::get()->getNode();
    hideLabelsNode->setAnchorPoint(ccp(0, 0.5f));
    hideLabelsNode->setScale(0.825f);

    //auto editSafezoneBtn = CCMenuItemSpriteExtra::create(BetterButtonSprite::create(ccp(90, 30), "Edit Safe Zones", "goldFont.fnt", "GJ_button_04.png"), this, menu_selector(LabelsNode::onEditSafeZones));
    //editSafezoneBtn->m_scaleMultiplier = 1.1f;
    auto importBtn = CCMenuItemSpriteExtra::create(BetterButtonSprite::create(ccp(90, 30), "Import from file", "goldFont.fnt", "GJ_button_04.png"), this, menu_selector(LabelsNode::onImportFromFile));
    importBtn->m_scaleMultiplier = 1.1f;

    auto topBarMenu = CCMenu::create();
    topBarMenu->setLayout(AxisLayout::create(Axis::Row)->setAutoScale(false)->setAxisReverse(false)->setAxisAlignment(AxisAlignment::End)->setGap(7.5f));
    topBarMenu->ignoreAnchorPointForPosition(false);
    topBarMenu->setContentSize(ccp(770, 30));
    topBarMenu->setScale(0.75f);
    // topBarMenu->addChild(editSafezoneBtn);
    topBarMenu->addChild(importBtn);
    topBarMenu->updateLayout();

    presetMenu = CCMenu::create();
    presetMenu->setLayout(AxisLayout::create(Axis::Column)->setAutoScale(false)->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End)->setGap(7.5f));
    presetMenu->setContentSize(ccp(PRESETS_SIZE / PRESETS_SCALE, 8008));
    presetMenu->setScale(PRESETS_SCALE);
    presetMenu->setAnchorPoint(ccp(0.5f, 1));

    auto widthNode = CCNode::create();
    widthNode->setContentWidth(PRESETS_SIZE / PRESETS_SCALE);

    presetMenu->addChild(widthNode);

    addPresets();

    presetMenu->updateLayout();

    hintNone = AdvLabelBMFont::createWithLocalisation("labels/none-added-hint", "bigFont.fnt");
    hintNone->setScale(0.35f);
    hintNone->setPosition(labelsScroll->getContentSize() / 2);

    updateUI();
    labelsScroll->moveToTop();
    labelsScroll->addChild(hintNone);

    this->addChildAtPosition(bgTopbar, Anchor::Top);
    this->addChildAtPosition(bgPresets, Anchor::BottomRight);
    this->addChildAtPosition(hideLabelsNode, Anchor::TopLeft, ccp(0, -12.5f));
    this->addChildAtPosition(topBarMenu, Anchor::TopLeft, ccp(50, -12.5f));
    this->addChildAtPosition(presetMenu, Anchor::TopRight, ccp(-PRESETS_SIZE / 2, -TOPBAR_SIZE - 5 + 1));
    this->addChild(labelsScroll);
    return true;
}

void LabelsNode::updateUI()
{
    labelsScroll->m_contentLayer->removeAllChildren();

    auto configs = LabelManager::get()->getConfigs();

    float cellHeight = 25;
    float height = std::max<float>(configs.size() * cellHeight, labelsScroll->getContentHeight());

    labelsScroll->m_contentLayer->setContentHeight(height);

    int i = 0;
    for (auto config : configs)
    {
        auto cell = LabelConfigCell::create(ccp(labelsScroll->getContentWidth(), cellHeight), config, i);
        cell->setPositionY(height - cellHeight * (i + 1));
        cell->setAnchorPoint(ccp(0.5f, 0.5f));
        cell->setPosition(cell->getPosition() + cell->getContentSize() / 2);

        labelsScroll->m_contentLayer->addChild(cell);
        i++;
    }

    labelsScroll->setTouchEnabled(height != labelsScroll->getContentHeight());
    hintNone->setVisible(i == 0);
}

void LabelsNode::onEditSafeZones(CCObject* sender)
{

}

void LabelsNode::onImportFromFile(CCObject* sender)
{
    file::FilePickOptions options;

    file::FilePickOptions::Filter filter;
    filter.description = "QOLMod Label";
    filter.files = { "*.qollbl" };

    options.filters.push_back(filter);

    file::pickMany(options).listen([this](Result<std::vector<std::filesystem::path>>* path)
    {
        if (path->isOk())
        {
            auto paths = path->unwrap();

            for (auto path : paths)
            {
                LabelManager::get()->addFromFile(path);
            }
        }
    });
}

void LabelsNode::onAddLabel(CCObject* sender)
{
    LabelConfig conf;

    switch (sender->getTag())
    {
        case 1:
            conf.displayName = "Cheat Indicator";
            conf.formatString = ".";
            conf.cheatIndicator = true;
            break;

        case 2:
            conf.displayName = "FPS Counter";
            conf.formatString = "{precision(fps, 0)} FPS";
            break;

        case 3:
            conf.displayName = "CPS Counter";
            conf.formatString = "{total_cps} CPS";
            // module->events.push_back(LabelEvent { .colour = ccc4(0, 255, 0, 255), .fadeIn = 0, .hold = 0, .fadeOut = -1, .type = LabelEventType::ClickStarted});
            // module->events.push_back(LabelEvent { .colour = ccc4(255, 255, 255, 255), .fadeIn = 0, .hold = 0, .fadeOut = -1, .type = LabelEventType::ClickEnded});
            break;

        case 4:
            conf.displayName = "Noclip Accuracy";
            conf.formatString = "{noclip_accuracy}%";
            conf.noclipOnly = true;
            // module->events.push_back(LabelEvent { .colour = ccc4(255, 0, 0, 255), .fadeIn = 0, .hold = 0, .fadeOut = 0.5f, .type = LabelEventType::PlayerTookDamage});
            break;

        case 5:
            conf.displayName = "Noclip Deaths";
            conf.formatString = "{noclip_deaths} Death{noclip_deaths == 1 ? \"\" : \"s\"}";
            conf.noclipOnly = true;
            // module->events.push_back(LabelEvent { .colour = ccc4(255, 0, 0, 255), .fadeIn = 0, .hold = 0, .fadeOut = 0.5f, .type = LabelEventType::PlayerTookDamage});
            break;

        case 6:
            conf.displayName = "Attempt";
            conf.formatString = "Attempt {attempt}";
            break;

        case 7:
            conf.displayName = "Session Time";
            conf.formatString = "{leftPad((session_hours < 10 ? \"0\" : \"\") + session_hours + \"\", 2)}:{leftPad((session_minutes < 10 ? \"0\" : \"\") + session_minutes, 2)}:{leftPad((session_seconds < 10 ? \"0\" : \"\") + session_seconds + \"\", 2)}";
            break;

        case 8:
            conf.displayName = "Best Run";
            conf.formatString = "Best Run: {isEditor ? \"Editor\" : ((bestRun_from == 0 ? \"\" : (bestRun_from + \"% - \"))) + (bestRun_to + \"%\")}";
            break;

        case 9:
            conf.displayName = "Clock";
            conf.formatString = "{leftPad((clock_hours < 10 ? \"0\" : \"\") + clock_hours + \"\", 2)}:{leftPad((clock_minutes < 10 ? \"0\" : \"\") + clock_minutes, 2)}:{leftPad((clock_seconds < 10 ? \"0\" : \"\") + clock_seconds + \"\", 2)}";
            break;

        default:
            conf.displayName = "Custom Label";
            conf.formatString = "Default Format";
            break;
    }

    auto configs = LabelManager::get()->getConfigs();
    configs.push_back(conf);
    LabelManager::get()->setConfigs(configs);
}

#define BUTTON_WIDTH 110 / PRESETS_SCALE
#define BUTTON_HEIGHT 32

#define PRESET_BUTTON(__text, __id) \
btn = ButtonSprite::create(__text, BUTTON_WIDTH, BUTTON_WIDTH, 1.0f, false, "bigFont.fnt", "GJ_button_05.png", BUTTON_HEIGHT); \
btn->setContentWidth(BUTTON_WIDTH); \
btn->m_label->setPositionX(BUTTON_WIDTH / 2); \
btn->m_BGSprite->setPositionX(BUTTON_WIDTH / 2); \
btn->m_BGSprite->setContentWidth(BUTTON_WIDTH); \
btn->m_label->limitLabelWidth(80 / PRESETS_SCALE, 0.75f, 0); \
button = CCMenuItemSpriteExtra::create(btn, this, menu_selector(LabelsNode::onAddLabel)); \
button->m_scaleMultiplier = 1.1f; \
button->setTag(__id); \
presetMenu->addChild(button)

void LabelsNode::addPresets()
{
    ButtonSprite* btn = nullptr;
    CCMenuItemSpriteExtra* button = nullptr;

    PRESET_BUTTON("Cheat Indicator", 1);
    PRESET_BUTTON("FPS Counter", 2);
    PRESET_BUTTON("CPS Counter", 3);
    PRESET_BUTTON("Noclip Accuracy", 4);
    PRESET_BUTTON("Noclip Deaths", 5);
    PRESET_BUTTON("Attempts", 6);
    PRESET_BUTTON("Session Time", 7);
    PRESET_BUTTON("Best Run", 8);
    PRESET_BUTTON("Clock", 9);

    PRESET_BUTTON("Custom", -69);
}

LabelsNode::~LabelsNode()
{
    instance = nullptr;
}

LabelsNode* LabelsNode::get()
{
    return instance;
}

void LabelsNode::scrollWheel(float y, float x)
{
    if (nodeIsVisible(labelsScroll) && labelsScroll->isTouchEnabled())
    {
        if (auto n = getTopLevelNonSceneNode(this))
        {
            if (CCScene::get()->getChildByIndex(-1) != n)
                return;
        }

        labelsScroll->scrollLayer(y);
    }
}