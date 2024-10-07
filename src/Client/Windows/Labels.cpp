#include "Labels.hpp"
#include "../../Layers/EditLabelPopup.hpp"

#define PRESET_BUTTON(__text, __id) \
btn = ButtonSprite::create(__text, 120, 0, 1.0f, false, "bigFont.fnt", "GJ_button_05.png"); \
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
            break;

        case 2:
            module = new LabelModule("{precision(fps, 0)} FPS", "bigFont.fnt");
            module->name = "FPS Counter";
            break;

        case 3:
            module = new LabelModule("{total_cps} CPS", "bigFont.fnt");
            module->name = "CPS Counter";
            break;

        case 6:
            module = new LabelModule("Attempt {attempt}", "bigFont.fnt");
            module->name = "Attempt";
            break;

        case 7:
            module = new LabelModule("{leftPad((session_hours < 10 ? \"0\" : \"\") + session_hours + \"\", 2)}:{leftPad((session_minutes < 10 ? \"0\" : \"\") + session_minutes, 2)}:{leftPad((session_seconds < 10 ? \"0\" : \"\") + session_seconds + \"\", 2)}", "bigFont.fnt");
            module->name = "Session Time";
            break;

        case 8:
            module = new LabelModule("Best Run: {isEditor ? \"Editor\" : ((bestRun_from == 0 ? \"\" : (bestRun_from + \"% - \"))) + (bestRun_to + \"%\")}", "bigFont.fnt");
            module->name = "Best Run";
            break;

        case 9:
            module = new LabelModule("{leftPad((clock_hours < 10 ? \"0\" : \"\") + clock_hours + \"\", 2)}:{leftPad((clock_minutes < 10 ? \"0\" : \"\") + clock_minutes, 2)}:{leftPad((clock_seconds < 10 ? \"0\" : \"\") + clock_seconds + \"\", 2)}", "bigFont.fnt");
            module->name = "Clock";
            break;

        default:
            module = new LabelModule("Default Format", "bigFont.fnt");
            module->name = "Custom Label";
            break;
    }

    modules.push_back(module);
    refreshList();

    if (auto bg = cells[cells.size() - 1]->getChildByID("background"))
    {
        as<CCScale9Sprite*>(bg)->setColor(rand() % 100 == 69 ? ccc3(200, 0, 255) : ccc3(255, 255, 0));
        as<CCScale9Sprite*>(bg)->runAction(CCTintTo::create(0.35f, 0, 0, 0));
    }
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
            }
        }
    );
}

void Labels::onSettings(CCObject* sender)
{
    auto mod = as<LabelModule*>(as<CCNode*>(sender)->getUserData());

    EditLabelPopup::addToScene(mod, true);
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

    for (auto module : modules)
    {
        if (auto lbl = typeinfo_cast<LabelModule*>(module))
        {
            auto cell = CCNode::create();
            cell->setPositionY(y);
            cell->setContentHeight(height);
            cell->setContentWidth(scroll->getContentWidth());
            cell->setUserData(module);
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

            auto name = CCLabelBMFont::create(module->name.c_str(), "bigFont.fnt");
            name->setAnchorPoint(ccp(0, 0.5f));
            name->setPosition(ccp(10, cell->getContentHeight() / 2));
            name->limitLabelWidth(cell->getContentWidth() - 70, 0.4f, 0);

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

            m->addChild(options);
            m->addChild(deleteBtn);

            cell->addChild(bg);
            cell->addChild(name);
            cell->addChild(m);

            scroll->m_contentLayer->addChild(cell);

            cells.push_back(cell);
        }
    }

    scroll->m_contentLayer->setContentHeight(std::max<float>(scroll->m_contentLayer->getContentHeight(), y + 4));
    scroll->setTouchEnabled(scroll->m_contentLayer->getContentHeight() != scroll->getContentHeight());

    for (auto cell : cells)
    {
        cell->setPositionY(scroll->m_contentLayer->getContentHeight() - cell->getPositionY() - 4 - cell->getContentHeight());
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

Labels* Labels::get()
{
    return instance;
}