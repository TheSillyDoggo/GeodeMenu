#include "Labels.hpp"
#include "../../Layers/EditLabelPopup.hpp"
#include "../../Layers/EditSafeZonePopup.hpp"
#include "../../Labels/LabelLayer.hpp"
#include "../AndroidUI.h"
#include "../../DragDrop.hpp"
#include "../../UI/GrabNodeLayer.hpp"

#define BUTTON_WIDTH 200

#define PRESET_BUTTON(__text, __id) \
btn = ButtonSprite::create(__text, BUTTON_WIDTH, 0, 1.0f, false, "bigFont.fnt", "GJ_button_05.png"); \
btn->setContentWidth(BUTTON_WIDTH); \
btn->m_label->setPositionX(BUTTON_WIDTH / 2); \
btn->m_BGSprite->setPositionX(BUTTON_WIDTH / 2); \
btn->m_BGSprite->setContentWidth(BUTTON_WIDTH); \
btn->m_label->setScale(0.6f); \
button = CCMenuItemSpriteExtra::create(btn, this, menu_selector(Labels::onAddItem)); \
button->setTag(__id); \
rightMenu->addChild(button)

void Labels::cocosCreate(CCMenu* menu)
{
    scroll = nullptr;
    this->menu = menu;

    auto back = CCScale9Sprite::create("square02_small.png");
    back->setContentSize(menu->getContentSize() / 0.5f * ccp(0.65f, 1) - ccp(5, 0));
    back->setPosition(ccp(0, 0));
    back->setAnchorPoint(ccp(0, 0));
    back->setScale(0.5f);
    back->setOpacity(100);
    menu->addChild(back, -69);

    back = CCScale9Sprite::create("square02_small.png");
    back->setContentSize(menu->getContentSize() / 0.5f * ccp(0.35f, 1) - ccp(5, 0));
    back->setPosition(ccp(menu->getContentWidth(), 0));
    back->setAnchorPoint(ccp(1, 0));
    back->setScale(0.5f);
    back->setOpacity(100);
    menu->addChild(back, -69);

    auto title = CCLabelBMFont::create("Presets", "bigFont.fnt");
    title->setScale(0.5f * 2);
    back->addChildAtPosition(title, Anchor::Top, ccp(0, -28));

    auto seperator = CCSprite::createWithSpriteFrameName("edit_vLine_001.png");
    seperator->setRotation(90);
    seperator->setScaleY(2.5f);
    seperator->setScaleX(1.5f);
    back->addChildAtPosition(seperator, Anchor::Top, ccp(0, -55));

    auto rightMenu = CCMenu::create();
    rightMenu->ignoreAnchorPointForPosition(false);
    rightMenu->setAnchorPoint(ccp(0.5f, 1));
    rightMenu->setContentHeight(1000);
    rightMenu->setScale(0.9f);
    rightMenu->setLayout(AxisLayout::create(Axis::Column)->setAutoScale(false)->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End));

    ButtonSprite* btn;
    CCMenuItemSpriteExtra* button;

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

    rightMenu->updateLayout();
    back->addChildAtPosition(rightMenu, Anchor::Top, ccp(0, -70));

    auto safeZoneMenu = CCMenu::create();
    safeZoneMenu->setPosition(CCPointZero);

    auto safeBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("accountBtn_settings_001.png"), this, menu_selector(Labels::onSetupSafeZone));
    safeBtn->setPosition(ccp(240, 15));
    safeBtn->getNormalImage()->setScale(0.5f);

    // ButtonSprite * create(const char *caption, int width, bool absolute, const char *font, const char *texture, float height, float scale)

    auto importSpr = ButtonSprite::create("Import From File", 100, false, "bigFont.fnt", "GJ_button_05.png", 30, 1.0f);
    importSpr->setScale(0.7f);

    auto importBtn = CCMenuItemSpriteExtra::create(importSpr, this, menu_selector(Labels::onImportFromFile));
    importBtn->setPosition(safeBtn->getPosition() + ccp(55, 0));
    //importBtn->getNormalImage()->setScale(0.7f);
    safeZoneMenu->addChild(importBtn);

    safeZoneMenu->addChild(safeBtn);
    menu->addChild(safeZoneMenu);

    refreshList();
}

void Labels::onAddItem(CCObject* sender)
{
    LabelModule* module;

    switch (sender->getTag())
    {
        case 1:
            module = new LabelModule(".", "bigFont.fnt");
            module->name = "Cheat Indicator";
            module->isCheatIndicator = true;
            module->presetType = sender->getTag();
            break;

        case 2:
            module = new LabelModule("{precision(fps, 0)} FPS", "bigFont.fnt");
            module->name = "FPS Counter";
            module->presetType = sender->getTag();
            break;

        case 3:
            module = new LabelModule("{total_cps} CPS", "bigFont.fnt");
            module->name = "CPS Counter";
            module->events.push_back(LabelEvent { .colour = ccc4(0, 255, 0, 255), .fadeIn = 0, .hold = 0, .fadeOut = -1, .type = LabelEventType::ClickStarted});
            module->events.push_back(LabelEvent { .colour = ccc4(255, 255, 255, 255), .fadeIn = 0, .hold = 0, .fadeOut = -1, .type = LabelEventType::ClickEnded});
            module->presetType = sender->getTag();
            break;

        case 4:
            module = new LabelModule("{noclip_accuracy}%", "bigFont.fnt");
            module->name = "Noclip Accuracy";
            module->events.push_back(LabelEvent { .colour = ccc4(255, 0, 0, 255), .fadeIn = 0, .hold = 0, .fadeOut = 0.5f, .type = LabelEventType::PlayerTookDamage});
            module->presetType = sender->getTag();
            module->noclipOnly = true;
            break;

        case 5:
            module = new LabelModule("{noclip_deaths} Death{noclip_deaths == 1 ? \"\" : \"s\"}", "bigFont.fnt");
            module->name = "Noclip Deaths";
            module->events.push_back(LabelEvent { .colour = ccc4(255, 0, 0, 255), .fadeIn = 0, .hold = 0, .fadeOut = 0.5f, .type = LabelEventType::PlayerTookDamage});
            module->presetType = sender->getTag();
            module->noclipOnly = true;
            break;

        case 6:
            module = new LabelModule("Attempt {attempt}", "bigFont.fnt");
            module->name = "Attempt";
            module->presetType = sender->getTag();
            break;

        case 7:
            module = new LabelModule("{leftPad((session_hours < 10 ? \"0\" : \"\") + session_hours + \"\", 2)}:{leftPad((session_minutes < 10 ? \"0\" : \"\") + session_minutes, 2)}:{leftPad((session_seconds < 10 ? \"0\" : \"\") + session_seconds + \"\", 2)}", "bigFont.fnt");
            module->name = "Session Time";
            module->presetType = sender->getTag();
            break;

        case 8:
            module = new LabelModule("Best Run: {isEditor ? \"Editor\" : ((bestRun_from == 0 ? \"\" : (bestRun_from + \"% - \"))) + (bestRun_to + \"%\")}", "bigFont.fnt");
            module->name = "Best Run";
            module->presetType = sender->getTag();
            break;

        case 9:
            module = new LabelModule("{leftPad((clock_hours < 10 ? \"0\" : \"\") + clock_hours + \"\", 2)}:{leftPad((clock_minutes < 10 ? \"0\" : \"\") + clock_minutes, 2)}:{leftPad((clock_seconds < 10 ? \"0\" : \"\") + clock_seconds + \"\", 2)}", "bigFont.fnt");
            module->name = "Clock";
            module->presetType = sender->getTag();
            break;

        default:
            module = new LabelModule("Default Format", "bigFont.fnt");
            module->name = "Custom Label";
            break;
    }

    modules.push_back(module);
    refreshList();
    save();

    if (auto bg = cells[cells.size() - 1]->getChildByID("background"))
    {
        as<CCScale9Sprite*>(bg)->setColor(rand() % 100 == 69 ? ccc3(200, 0, 255) : ccc3(255, 255, 0));
        as<CCScale9Sprite*>(bg)->runAction(CCTintTo::create(0.35f, 0, 0, 0));
    }

    if (LabelLayer::get())
        LabelLayer::get()->updateLabels();
}

void Labels::onSetupSafeZone(CCObject* sender)
{
    EditSafeZonePopup::addToScene();
}

void Labels::onDelete(CCObject* sender)
{
    auto mod = as<LabelModule*>(as<CCNode*>(sender)->getUserData());

    geode::createQuickPopup("Delete Label",
        "Are you sure you want to <cr>delete</c> this <cc>label</c>?",
        "Cancel",
        "Delete",
        [mod, this](FLAlertLayer* alert, bool right){
            if (right)
            {
                std::erase(modules, mod);

                delete mod;

                refreshList();
                save();

                if (LabelLayer::get())
                    LabelLayer::get()->updateLabels();
            }
        }
    );
}

void Labels::onSettings(CCObject* sender)
{
    auto mod = as<LabelModule*>(as<CCNode*>(sender)->getUserData());

    EditLabelPopup::addToScene(mod, mod->presetType == -1);
}

void Labels::onMoveLabelUp(CCObject* sender)
{
    if (sender->getTag() > 0)
        std::swap(modules[sender->getTag()], modules[sender->getTag() - 1]);

    refreshList();
    save();

    if (LabelLayer::get())
        LabelLayer::get()->updateLabels();
}

void Labels::onMoveLabelDown(CCObject* sender)
{
    // cuz someones gonna fuck around in devtools and crash the game
    if (sender->getTag() < modules.size())
        std::swap(modules[sender->getTag()], modules[sender->getTag() + 1]);

    refreshList();
    save();

    if (LabelLayer::get())
        LabelLayer::get()->updateLabels();
}

void Labels::onToggleVisible(CCObject* sender)
{
    auto mod = as<LabelModule*>(as<CCNode*>(sender)->getUserData());

    mod->visible = !mod->visible;
    save();

    if (LabelLayer::get())
        LabelLayer::get()->updateLabels();
}

void Labels::refreshList()
{
    if (scroll)
        scroll->removeFromParent();

    cells.clear();

    scroll = ScrollLayer::create(menu->getContentSize() * ccp(0.65f, 1) - ccp(2.5f, 0));

    menu->addChild(scroll);

    float height = 23;
    float y = 0;
    int i = 0;

    for (auto module : modules)
    {
        if (auto lbl = typeinfo_cast<LabelModule*>(module))
        {
            auto cell = CCNode::create();
            cell->setPositionY(y);
            cell->setContentHeight(height);
            cell->setContentWidth(scroll->getContentWidth());
            cell->setUserData(module);
            cell->setAnchorPoint(ccp(0.5f, 0.5f));
            y += height + 4;

            auto bg = CCScale9Sprite::create("square02b_001.png");
            bg->setColor(ccc3(0, 0, 0));
            bg->setOpacity(100);
            bg->setID("background");
            bg->setAnchorPoint(ccp(0, 0));
            bg->setScale(1.0f / 2.5f);
            bg->setContentHeight(cell->getContentHeight());
            bg->setContentWidth(scroll->getContentWidth() - 8);
            bg->setContentSize(bg->getContentSize() * 2.5f);
            bg->setPositionX(4);

            auto nameBack = CCScale9Sprite::create("square02_small.png");
            nameBack->setOpacity(100);
            nameBack->setContentSize(ccp(120, 18) * 3);
            nameBack->setAnchorPoint(ccp(0, 0.5f));
            nameBack->setScale(1.0f / 3.0f);
            nameBack->setPositionX(6.5f);
            nameBack->setPositionY(cell->getContentHeight() / 2);

            auto nameInp = TextInput::create(160, "Display Name", "bigFont.fnt");
            nameInp->setAnchorPoint(ccp(0, 0.5f));
            nameInp->setPosition(ccp(10, cell->getContentHeight() / 2));
            nameInp->setScale(0.7f);

            nameInp->getBGSprite()->setVisible(false);
            // nameInp->getInputNode()->m_placeholderLabel->setAnchorPoint(ccp(0, 0.5f));
            // nameInp->getInputNode()->m_placeholderLabel->setPositionX(-nameInp->getContentWidth() / 2);

            nameInp->setString(lbl->name);
            nameInp->setCallback([this, lbl](const std::string& str)
            {
                lbl->name = str;

                save();
            });

            auto m = CCMenu::create();
            m->setPosition(ccp(0, 0));

            auto op = CCSprite::createWithSpriteFrameName("accountBtn_settings_001.png");
            op->setScale(0.4f);

            auto options = CCMenuItemSpriteExtra::create(op, this, menu_selector(Labels::onSettings));
            options->setPositionX(cell->getContentWidth() - 16);
            options->setPositionY(cell->getContentHeight() / 2);
            options->setUserData(module);

            auto del = CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
            del->setScale(0.55f);

            auto deleteBtn = CCMenuItemSpriteExtra::create(del, this, menu_selector(Labels::onDelete));
            deleteBtn->setPositionX(cell->getContentWidth() - 35);
            deleteBtn->setPositionY(cell->getContentHeight() / 2);
            deleteBtn->setUserData(module);

            bool single = false;// i == 0 || i == modules.size() - 1;

            auto arrowBack = CCScale9Sprite::create("square02_small.png");
            arrowBack->setOpacity(100);
            arrowBack->setContentSize(ccp(18, 18) * 3);
            arrowBack->setScale(1.0f / 3.0f);
            arrowBack->setPositionX(cell->getContentWidth() - (195 - ((18 / 2) / arrowBack->getScale())) * arrowBack->getScale());
            arrowBack->setPositionY(cell->getContentHeight() / 2);

            auto grab = GrabNodeLayer::create();
            grab->setNodeToGrab(cell);
            grab->setLockedAxis(LockedAxis::Vertical);
            grab->setOnStartDrag([cell, bg]
            {
                cell->runAction(CCEaseInOut::create(CCScaleTo::create(0.2f, 0.95f), 2));

                bg->runAction(CCTintTo::create(0.35f, 145, 0, 255));

                cell->setZOrder(42069);
            });
            grab->setOnEndDrag([cell, this, i, bg]
            {
                cell->runAction(CCEaseBackOut::create(CCScaleTo::create(0.35f, 1.0f)));

                bg->runAction(CCTintTo::create(0.35f, 0, 0, 0));

                std::sort(cells.begin(), cells.end(), [](CCNode* a, CCNode* b)
                {
                    return a->getPositionY() > b->getPositionY();
                });

                int index = 0;

                for (size_t i = 0; i < cells.size(); i++)
                {
                    if (cells[i] == cell)
                    {
                        index = i;
                        break;
                    }
                }

                log::info("index: {}", index);

                std::swap(modules[i], modules[index]);
                
                save();

                if (LabelLayer::get())
                    LabelLayer::get()->updateLabels();

                dirtyRefreshList(true);
            });

            grab->setContentSize(ccp(cell->getContentHeight() - 2, cell->getContentHeight() - 2));
            grab->setPosition(arrowBack->getPosition());

            auto grabSpr = CCSprite::create("draggable.png"_spr);
            grabSpr->setPosition(grab->getPosition());
            grabSpr->setScale(0.6f);

            auto toggleBtn = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(Labels::onToggleVisible), 0.45f);
            toggleBtn->setUserData(module);
            toggleBtn->toggle(lbl->visible);
            toggleBtn->setPosition(arrowBack->getPosition() + ccp(-arrowBack->getScaledContentWidth() / 2, 0) + ccp(-12, 0) + ccp(modules.size() == 1 ? 41 : 0, 0));

            auto toggleBG = CCScale9Sprite::create("square02_small.png");
            toggleBG->setOpacity(100);
            toggleBG->setContentSize(ccp(18, 18) * 3);
            toggleBG->setScale(1.0f / 3.0f);
            toggleBG->setPosition(toggleBtn->getPosition());

            m->addChild(options);
            m->addChild(deleteBtn);
            m->addChild(arrowBack);
            m->addChild(grab);
            m->addChild(grabSpr);
            m->addChild(toggleBG);
            m->addChild(toggleBtn);

            cell->addChild(bg);
            cell->addChild(nameBack);
            cell->addChild(nameInp);
            cell->addChild(m);

            scroll->m_contentLayer->addChild(cell);

            cells.push_back(cell);

            i++;
        }
    }

    scroll->m_contentLayer->setContentHeight(std::max<float>(scroll->m_contentLayer->getContentHeight(), y + 4));
    scroll->setTouchEnabled(scroll->m_contentLayer->getContentHeight() != scroll->getContentHeight());

    for (auto cell : cells)
    {
        cell->setPositionY(scroll->m_contentLayer->getContentHeight() - cell->getPositionY() - 4 - cell->getContentHeight());
        cell->setPosition(cell->getPosition() + cell->getContentSize() / 2);
    }

    if (cells.size() == 0)
    {
        auto help = CCLabelBMFont::create("No labels added yet :(\nYou can add then with\nthe buttons on the right", "bigFont.fnt");
        help->setPosition(scroll->m_contentLayer->getContentSize() / 2);
        help->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
        help->setScale(0.45f);

        scroll->m_contentLayer->addChild(help);
    }
}

void Labels::dirtyRefreshList(bool smooth)
{
    for (size_t i = 0; i < modules.size(); i++)
    {
        // im going to kill myself very soon
        auto pos = ccp(cells[i]->getContentWidth() / 2, (scroll->m_contentLayer->getContentHeight() - ((23 + 4) * i) - cells[i]->getContentHeight() / 2) - 4);

        if (smooth)
            cells[i]->runAction(CCEaseInOut::create(CCMoveTo::create(0.35f, pos), 2));
        else
            cells[i]->setPosition(pos);

        cells[i]->setZOrder(i);
    }
}

void Labels::save()
{
    Mod::get()->setSavedValue<float>("safe-zone.x", safeZone.origin.x);
    Mod::get()->setSavedValue<float>("safe-zone.y", safeZone.origin.y);
    Mod::get()->setSavedValue<float>("safe-zone.width", safeZone.size.width);
    Mod::get()->setSavedValue<float>("safe-zone.height", safeZone.size.height);

    std::vector<matjson::Value> arr;

    for (auto module : modules)
    {
        if (auto lblMod = typeinfo_cast<LabelModule*>(module))
            arr.push_back(lblMod->saveToObject());
    }

    Mod::get()->setSavedValue<matjson::Value>("selected-labels", arr);

    (void) Mod::get()->saveData();
}

void Labels::load()
{
    safeZone = CCRectMake(Mod::get()->getSavedValue<float>("safe-zone.x", 3), Mod::get()->getSavedValue<float>("safe-zone.y", 3), Mod::get()->getSavedValue<float>("safe-zone.width", 3), Mod::get()->getSavedValue<float>("safe-zone.height", 3));

    if (!Mod::get()->hasSavedValue("selected-labels"))
        return loadFromPrevSave();

    auto arr = Mod::get()->getSavedValue<matjson::Value>("selected-labels");

    if (!arr.isArray())
        return;

    for (auto obj : arr.asArray().unwrap())
    {
        if (obj.isObject())
            modules.push_back(LabelModule::createFromObject(obj));
    }
}

void Labels::loadFromPrevSave()
{
    auto font = Mod::get()->getSavedValue<std::string>("labels-font_font", "bigFont.fnt");
    float opacity = numFromString<float>(Mod::get()->getSavedValue<std::string>("status-op_value", "0.9")).unwrapOr(0.9f);
    float scale = numFromString<float>(Mod::get()->getSavedValue<std::string>("status-sc_value", "1")).unwrapOr(1);

    if (Mod::get()->getSavedValue<bool>("cheat-indicator_enabled", false))
    {
        auto side = Mod::get()->getSavedValue<int>("cheat-indicator-side_index", 0);

        auto mod = new LabelModule(".", "bigFont.fnt");
        mod->name = "Cheat Indicator";
        mod->setSide(side == 0 ? LabelAnchor::TopLeft : (side == 1 ? LabelAnchor::TopRight : (side == 2 ? LabelAnchor::BottomLeft : LabelAnchor::BottomRight)));
        mod->setFont(font);
        mod->setScale(scale);
        mod->setOpacity(opacity);
        mod->isCheatIndicator = true;

        modules.push_back(mod);
    }

    if (Mod::get()->getSavedValue<bool>("status-fps_enabled", false))
    {
        auto side = Mod::get()->getSavedValue<int>("status-fps-side_index", 0);

        auto mod = new LabelModule("{precision(fps, 0)} FPS", "bigFont.fnt");
        mod->name = "FPS Counter";
        mod->setSide(side == 0 ? LabelAnchor::TopLeft : (side == 1 ? LabelAnchor::TopRight : (side == 2 ? LabelAnchor::BottomLeft : LabelAnchor::BottomRight)));
        mod->setFont(font);
        mod->setScale(scale);
        mod->setOpacity(opacity);

        modules.push_back(mod);
    }

    if (Mod::get()->getSavedValue<bool>("status-attempt_enabled", false))
    {
        auto side = Mod::get()->getSavedValue<int>("status-attempt-side_index", 0);

        auto mod = new LabelModule("Attempt {attempt}", "bigFont.fnt");
        mod->name = "Attempt";
        mod->setSide(side == 0 ? LabelAnchor::TopLeft : (side == 1 ? LabelAnchor::TopRight : (side == 2 ? LabelAnchor::BottomLeft : LabelAnchor::BottomRight)));
        mod->setFont(font);
        mod->setScale(scale);
        mod->setOpacity(opacity);

        modules.push_back(mod);
    }

    if (Mod::get()->getSavedValue<bool>("status-message_enabled", false))
    {
        auto side = Mod::get()->getSavedValue<int>("status-message-side_index", 0);

        auto mod = new LabelModule(Mod::get()->getSavedValue<std::string>("status-message-text_value", "Default Message"), "bigFont.fnt");
        mod->name = "Message";
        mod->setSide(side == 0 ? LabelAnchor::TopLeft : (side == 1 ? LabelAnchor::TopRight : (side == 2 ? LabelAnchor::BottomLeft : LabelAnchor::BottomRight)));
        mod->setFont(font);
        mod->setScale(scale);
        mod->setOpacity(opacity);

        modules.push_back(mod);
    }

    if (Mod::get()->getSavedValue<bool>("status-session_enabled", false))
    {
        auto side = Mod::get()->getSavedValue<int>("status-session-side_index", 0);

        auto mod = new LabelModule("{leftPad((session_hours < 10 ? \"0\" : \"\") + session_hours + \"\", 2)}:{leftPad((session_minutes < 10 ? \"0\" : \"\") + session_minutes, 2)}:{leftPad((session_seconds < 10 ? \"0\" : \"\") + session_seconds + \"\", 2)}", "bigFont.fnt");
        mod->name = "Session Time";
        mod->setSide(side == 0 ? LabelAnchor::TopLeft : (side == 1 ? LabelAnchor::TopRight : (side == 2 ? LabelAnchor::BottomLeft : LabelAnchor::BottomRight)));
        mod->setFont(font);
        mod->setScale(scale);
        mod->setOpacity(opacity);

        modules.push_back(mod);
    }

    if (Mod::get()->getSavedValue<bool>("status-cps_enabled", false))
    {
        auto side = Mod::get()->getSavedValue<int>("status-cps-side_index", 0);

        auto mod = new LabelModule("{total_cps}", "bigFont.fnt");
        mod->name = "CPS Counter";

        if (Mod::get()->getSavedValue<bool>("status-cps-total_enabled", false))
            mod->format +=" / {total_clicks}";

        mod->setSide(side == 0 ? LabelAnchor::TopLeft : (side == 1 ? LabelAnchor::TopRight : (side == 2 ? LabelAnchor::BottomLeft : LabelAnchor::BottomRight)));
        mod->setFont(font);
        mod->setScale(scale);
        mod->setOpacity(opacity);

        modules.push_back(mod);
    }

    if (Mod::get()->getSavedValue<bool>("best-run_enabled", false))
    {
        auto side = Mod::get()->getSavedValue<int>("best-run-side_index", 0);
        int precision = Mod::get()->getSavedValue("best-run-decimals_enabled", true) ? 2 : 0;

        auto mod = new LabelModule("Best Run: {isEditor ? \"Editor\" : ((precision(bestRun_from, " + fmt::format("{}", precision)
        
        + ") == 0 ? \"\" : (precision(bestRun_from, " + fmt::format("{}", precision) + ") + \"% - \"))) + (precision(bestRun_to, " + fmt::format("{}", precision) + ") + \"%\")}", "bigFont.fnt");
        mod->name = "Best Run";
        mod->setSide(side == 0 ? LabelAnchor::TopLeft : (side == 1 ? LabelAnchor::TopRight : (side == 2 ? LabelAnchor::BottomLeft : LabelAnchor::BottomRight)));
        mod->setFont(font);
        mod->setScale(scale);
        mod->setOpacity(opacity);

        modules.push_back(mod);
    }

    if (Mod::get()->getSavedValue<bool>("status-clock_enabled", false))
    {
        auto side = Mod::get()->getSavedValue<int>("status-clock-side_index", 0);

        auto mod = new LabelModule(!Mod::get()->getSavedValue<bool>("status-clock-24h-time_enabled", false) ? 
        "{leftPad(((clock_hours - (clock_hours < 12 ? 12 : 0)) < 10 ? \"0\" : \"\") + (clock_hours - (clock_hours < 12 ? 12 : 0)) + \"\", 2)}:{leftPad((clock_minutes < 10 ? \"0\" : \"\") + clock_minutes, 2)}:{leftPad((clock_seconds < 10 ? \"0\" : \"\") + clock_seconds + \"\", 2)} {clock_hours < 12 ? \"AM\" : \"PM\"}"
        :
        "{leftPad((clock_hours < 10 ? \"0\" : \"\") + clock_hours + \"\", 2)}:{leftPad((clock_minutes < 10 ? \"0\" : \"\") + clock_minutes, 2)}:{leftPad((clock_seconds < 10 ? \"0\" : \"\") + clock_seconds + \"\", 2)}", "bigFont.fnt");
        mod->name = "Clock";
        mod->setSide(side == 0 ? LabelAnchor::TopLeft : (side == 1 ? LabelAnchor::TopRight : (side == 2 ? LabelAnchor::BottomLeft : LabelAnchor::BottomRight)));
        mod->setFont(font);
        mod->setScale(scale);
        mod->setOpacity(opacity);

        modules.push_back(mod);
    }

    save();
}

Labels* Labels::get()
{
    return instance;
}

void Labels::onImportFromFile(CCObject* sender)
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
                importFromFile(path);
            }
        }
    });
}

void Labels::importFromFile(std::filesystem::path path)
{
    auto res = file::readJson(path);

    if (res.isOk())
    {
        auto mod = LabelModule::createFromObject(res.unwrap());
        mod->name = fmt::format("{} ({})", mod->name, path.filename().string());

        modules.push_back(mod);

        save();
        
        if (AndroidUI::get())
            refreshList();

        FLAlertLayer::create("Success!", "<cg>Successfully</c> imported <cc>label</c>!", "Yay!")->show();
    }
    else
    {
        FLAlertLayer::create("Failure!", fmt::format("<cr>Failed</c> to import <cc>label</c>.\n<cl>{}</c>", res.unwrapErr()), "OK")->show();
    }
}

$on_mod(Loaded)
{
    DragDrop::get()->addListener("import-labels"_spr, [](std::vector<std::string> paths)
    {
        for (auto path : paths)
        {
            Labels::get()->importFromFile(path);
        }
    });
}