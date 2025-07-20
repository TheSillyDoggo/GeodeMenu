#include "SearchNode.hpp"
#include "../CategoryTabSprite.hpp"

bool SearchNode::init()
{
    if (!CategoryNode::init())
        return false;

    textInput = TextInput::create(getContentWidth() - 5 * 2, "Search a mod...");
    textInput->setAnchorPoint(ccp(0.5f, 1));
    textInput->setDelegate(this);
    textInput->setCommonFilter(CommonFilter::Any);
    // dear geode team.. why isnt height easily changable
    float height = 25;

    textInput->setContentHeight(height);
    textInput->getBGSprite()->setPositionY(height / 2);
    textInput->getBGSprite()->setContentHeight(height * 2);
    textInput->getInputNode()->setPositionY(height / 2);
    textInput->getInputNode()->setContentHeight(height);

    errorMenu = CCMenu::create();
    errorMenu->ignoreAnchorPointForPosition(false);
    errorMenu->setContentSize(ccp(100, 65));

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
        as<CCSprite*>(suggest->getChildByTag(32 + i))->setColor(ccc3(86, 98, 246));
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

    joinSpr->setAnchorPoint(ccp(0, 0));
    joinSpr2->setPosition(joinSpr->getContentSize() / 2);

    errorMenu->setVisible(false);

    scroll->setContentHeight(scroll->getContentHeight() - 30);

    this->addChildAtPosition(textInput, Anchor::Top, ccp(0, -5));
    this->addChildAtPosition(errorMenu, Anchor::Center, ccp(0, -30 / 2));
    return true;
}

void SearchNode::onJoinDiscord(CCObject* sender)
{
    utils::web::openLinkInBrowser("https://discord.gg/3Ua5GZq8Hs");
}

void SearchNode::textChanged(CCTextInputNode* input)
{
    removeAll();
    errorMenu->setVisible(false);

    if (textInput->getString().empty())
        return;

    auto str = utils::string::replace(utils::string::toLower(textInput->getString()), " ", "");

    for (auto module : Module::getAll())
    {
        auto name = utils::string::replace(utils::string::toLower(module->getName()), " ", "");
        auto id = utils::string::replace(utils::string::toLower(module->getID()), " ", "");
        auto desc = utils::string::replace(utils::string::toLower(module->getDescription()), " ", "");

        if (name.find(str) != std::string::npos)
        {
            addModule(module);
            continue;
        }

        if (id.find(str) != std::string::npos)
        {
            addModule(module);
            continue;
        }

        if (desc.find(str) != std::string::npos)
        {
            addModule(module);
            continue;
        }
    }

    errorMenu->setVisible(modules.size() == 0);
}