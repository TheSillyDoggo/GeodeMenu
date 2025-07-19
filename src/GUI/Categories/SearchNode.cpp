#include "SearchNode.hpp"

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

    error = CCLabelBMFont::create("No results found", "bigFont.fnt");
    error->setScale(0.5f);
    error->setColor(ccc3(255, 155, 155));
    error->setVisible(false);

    scroll->setContentHeight(scroll->getContentHeight() - 30);

    this->addChildAtPosition(textInput, Anchor::Top, ccp(0, -5));
    this->addChildAtPosition(error, Anchor::Center, ccp(0, -30 / 2));
    return true;
}

void SearchNode::textChanged(CCTextInputNode* input)
{
    removeAll();
    error->setVisible(false);

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

    error->setVisible(modules.size() == 0);
}