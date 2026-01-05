#include "SubCategoryNode.hpp"
#include "../Localisation/LocalisationManager.hpp"
#include "../Utils/RealtimeAction.hpp"
        
void SubCategoryNode::updateUI()
{
    float width = std::min<float>((getContentWidth() - (selectSubMenu->getChildrenCount() + 1) * 2.5f) / selectSubMenu->getChildrenCount(), 100);
    auto size = ccp(width, 20);

    for (auto btn : normalSprs)
    {
        btn.second->updateSelection(btn.first == selectedSub[getID()] ? CategorySelectionType::Selected : CategorySelectionType::Deselected);
        btn.second->setContentSize(size);
        btn.second->setPosition(size / 2);
    }

    for (auto btn : normalBtns)
    {
        btn.second->setEnabled(btn.first != selectedSub[getID()]);
        btn.second->setContentSize(size);
        btn.second->getSelectedImage()->setContentSize(size);
        btn.second->getSelectedImage()->setPosition(size / 2);
    }

    for (auto node : categoryNodes)
    {
        node.second->setVisible(node.first == selectedSub[getID()]);
    }

    selectSubMenu->updateLayout();
}

bool SubCategoryNode::init()
{
    if (!CategoryNode::init())
        return false;

    this->scheduleUpdate();

    float height = 25;

    selectSubMenu = CCMenu::create();
    selectSubMenu->setAnchorPoint(ccp(0.5f, 1));
    selectSubMenu->ignoreAnchorPointForPosition(false);
    selectSubMenu->setContentSize(ccp(getContentWidth(), height));
    selectSubMenu->setLayout(AxisLayout::create(Axis::Row)->setGap(2.5f)->setAutoScale(false)->setCrossAxisOverflow(false));

    this->scroll->setVisible(false);
    this->scrollbar->setVisible(false);

    this->addChildAtPosition(selectSubMenu, Anchor::Top);
    return true;
}

void SubCategoryNode::addModule(Module* mod, std::string subCategory)
{
    if (!categoryNodes.contains(subCategory))
        addSubCategory(subCategory);

    categoryNodes[subCategory]->addModule(mod);
}

void SubCategoryNode::addSubCategory(std::string name)
{
    auto sprite = fmt::format("{}{}.png", ""_spr, utils::string::toLower(name));

    std::string nameStr = LocalisationManager::get()->getLocalisedString(fmt::format("categories/{}", utils::string::toLower(name)));

    auto sprNormal = CategoryTabSprite::create(CategoryTabType::Text, nameStr, sprite);
    auto sprHeld = CategoryTabSprite::create(CategoryTabType::Text, nameStr, sprite);
    sprHeld->updateSelection(CategorySelectionType::Hovered);

    sprNormal->setContentSize(ccp(80, 20));
    sprHeld->setContentSize(ccp(80, 20));

    auto btn = CCMenuItemSpriteExtra::create(sprNormal, this, menu_selector(SubCategoryNode::onSelectSub));
    btn->setID(name);
    btn->setSelectedImage(sprHeld);
    btn->m_scaleMultiplier = 1;

    sprNormal->setAnchorPoint(ccp(0, 0));
    sprHeld->setPosition(btn->getContentSize() / 2);

    selectSubMenu->addChild(btn);

    normalSprs.emplace(name, sprNormal);
    normalBtns.emplace(name, btn);

    if (!selectedSub.contains(getID()))
        selectedSub.emplace(getID(), name);

    auto node = CategoryNode::getNode(fmt::format("{}/{}", getID(), name));
    node->bg->setVisible(false);
    node->setAnchorPoint(ccp(0, 0));
    node->setContentHeight(getContentHeight() - 25);
    categoryNodes.emplace(name, node);

    this->addChild(node);
    updateUI();
}

void SubCategoryNode::onSelectSub(CCObject* sender)
{
    selectedSub[getID()] = static_cast<CCNode*>(sender)->getID();

    updateUI();
}

void SubCategoryNode::update(float dt)
{
    for (auto btn : normalBtns)
    {
        bool sel = btn.second->m_bSelected || (btn.first == selectedSub[getID()]);
        float opacity = sel ? 125 : 100;

        if (!categoryBtnsSelCheck.contains(btn.second))
        {
            categoryBtnsSelCheck.emplace(btn.second, sel);

            static_cast<CategoryTabSprite*>(btn.second->getNormalImage())->background->setOpacity(opacity);
            static_cast<CategoryTabSprite*>(btn.second->getSelectedImage())->background->setOpacity(opacity);
        }
        else
        {
            if (categoryBtnsSelCheck[btn.second] != sel)
            {
                categoryBtnsSelCheck[btn.second] = sel;

                static_cast<CategoryTabSprite*>(btn.second->getNormalImage())->background->stopAllActions();
                static_cast<CategoryTabSprite*>(btn.second->getSelectedImage())->background->stopAllActions();

                static_cast<CategoryTabSprite*>(btn.second->getNormalImage())->background->runAction(RealtimeAction::create(CCFadeTo::create(0.1f, opacity)));
                static_cast<CategoryTabSprite*>(btn.second->getSelectedImage())->background->runAction(RealtimeAction::create(CCFadeTo::create(0.1f, opacity)));
            }
        }
    }
}