#include "SearchNode.hpp"
#include "../CategoryTabSprite.hpp"
#include "../SearchFiltersUI.hpp"
#include "../../Localisation/LocalisationManager.hpp"

SearchNode* SearchNode::get()
{
    return instance;
}

SearchNode::~SearchNode()
{
    instance = nullptr;
}

bool SearchNode::init()
{
    instance = this;
    this->alwaysShowScrollbar = true;

    if (!CategoryNode::init())
        return false;

    float height = 25;

    textInput = BetterInputNode::create((getContentWidth() - 5 * 2) - height - 5, LocalisationManager::get()->getLocalisedString("search-tab/placeholder"));
    textInput->setAnchorPoint(ccp(0, 1));
    textInput->setDelegate(this);
    textInput->setContentHeight(height);

    errorMenu = CCMenu::create();
    errorMenu->ignoreAnchorPointForPosition(false);
    errorMenu->setContentSize(ccp(100, 65));

    auto filterMenu = CCMenu::create();
    auto filterSpr = CCSprite::createWithSpriteFrameName("GJ_plus3Btn_001.png");
    filterSpr->setScale(1.15f);
    auto filterBtn = CCMenuItemSpriteExtra::create(filterSpr, this, menu_selector(SearchNode::onFilter));

    filterMenu->addChild(filterBtn);

    error = CCLabelBMFont::create("No results found", "bigFont.fnt");
    error->setScale(0.5f);
    error->setColor(ccc3(255, 155, 155));
    error->setPosition(ccp(100 / 2, errorMenu->getContentHeight()));
    errorMenu->addChild(error);

    auto suggest = CCLabelBMFont::create("You can suggest features\nin the discord server", "bigFont.fnt");
    suggest->setAlignment(kCCTextAlignmentCenter);
    suggest->setPosition(ccp(100 / 2, errorMenu->getContentHeight() - 29));
    suggest->setScale(0.5f);
    errorMenu->addChild(suggest);

    for (size_t i = 0; i < 7; i++)
    {
        static_cast<CCSprite*>(suggest->getChildByTag(32 + i))->setColor(ccc3(86, 98, 246));
    }

    auto joinSpr = CategoryTabSprite::create(CategoryTabType::Image, "Click to join!", "discord.png"_spr);
    joinSpr->setContentSize(ccp(145, 25));
    joinSpr->updateSelection(CategorySelectionType::Deselected);
    joinSpr->label->setScale(0.45f);

    auto joinSpr2 = CategoryTabSprite::create(CategoryTabType::Image, "Click to join!", "discord.png"_spr);
    joinSpr2->setContentSize(joinSpr->getContentSize());
    joinSpr2->updateSelection(CategorySelectionType::Hovered);
    joinSpr2->label->setScale(0.45f);

    auto joinBtn = CCMenuItemSpriteExtra::create(joinSpr, joinSpr2, this, menu_selector(SearchNode::onJoinDiscord));
    joinBtn->m_scaleMultiplier = 1;
    joinBtn->setPosition(ccp(100 / 2, 0));
    errorMenu->addChild(joinBtn);

    joinSpr2->setAnchorPoint(ccp(0.5f, 0.5f));
    joinSpr2->setPosition(joinSpr->getContentSize() / 2);

    errorMenu->setVisible(false);

    scroll->setContentHeight(scroll->getContentHeight() - 30);
    updateUI();
    scroll->moveToTop();

    textChanged(nullptr);

    this->addChildAtPosition(textInput, Anchor::TopLeft, ccp(5, -5));
    this->addChildAtPosition(errorMenu, Anchor::Center, ccp(0, -30 / 2));
    this->addChildAtPosition(filterMenu, Anchor::TopRight, ccp(-5, -5) - ccp(height, height) / 2);
    return true;
}

void SearchNode::onJoinDiscord(CCObject* sender)
{
    utils::web::openLinkInBrowser("https://discord.gg/3Ua5GZq8Hs");
}

void SearchNode::onFilter(CCObject* sender)
{
    SearchFiltersUI::create()->show();
}

void SearchNode::textChanged(CCTextInputNode* input)
{
    removeAll();
    errorMenu->setVisible(false);

    if (textInput->getString().empty())
        return;

    auto query = utils::string::replace(utils::string::toLower(textInput->getString()), " ", "");
    std::vector<Module*> mods = Module::getAll();

    std::sort(mods.begin(), mods.end(), [query](Module* a, Module* b)
    {
        auto aP = a->getSearchWeight(query);
        auto bP = b->getSearchWeight(query);

        return aP > bP;
    });

    for (auto module : mods)
    {
        if (!SearchShowOptions::get()->getRealEnabled() && module->getParent())
            continue;

        if (module->getSearchWeight(query) < 4.5f)
            continue;

        addModule(module);
    }

    errorMenu->setVisible(modules.size() == 0);
}

void SearchShowOptions::onToggle()
{
    if (SearchNode::get())
        SearchNode::get()->textChanged(nullptr);
}

void SearchNode::addModule(Module* module)
{
    auto node = module->getNode();
    node->setTag(modules.size());

    modules.emplace(module, node);

    scroll->m_contentLayer->addChild(node);
    updateUI();
    scroll->moveToTop();

    // return;
    // i cant get it to look good no matter the colour
    
    if (module->getParent())
    {
        auto bg = NineSlice::create("square02b_small.png");
        bg->setScaleMultiplier(0.5f);
        bg->setOpacity(50);
        bg->setColor(SearchOptionsColour::get()->getColour());
        bg->setScale(0.9f);
        bg->setContentSize(node->getContentSize() * (1.0f / 0.9f) - ccp(2, 2));
        bg->setZOrder(-80085);
        bg->setID("mod-option-search-result-bg");
        bg->setAnchorPoint(ccp(0, 0));
        bg->setPosition(ccp(1, 1));
        bg->schedule(schedule_selector(SearchNode::updateColour));
        node->addChild(bg);
    }
}

void SearchNode::updateColour(float dt)
{
    auto ns = reinterpret_cast<geode::NineSlice*>(this);

    ns->setColor(SearchOptionsColour::get()->getColour());
}