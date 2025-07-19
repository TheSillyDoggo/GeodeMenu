#include "CategoryNode.hpp"
#include "../Utils/Casts.hpp"

CategoryNode* CategoryNode::create()
{
    auto pRet = new CategoryNode();

    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void CategoryNode::addAdvanced(std::string name, std::function<CategoryNode*()> func)
{
    advCategories.emplace(name, func);
}

CategoryNode* CategoryNode::getNode(std::string category)
{
    if (advCategories.contains(category))
        return advCategories[category]();

    return create();
}

void CategoryNode::addModule(Module* module)
{
    auto node = module->getNode();
    node->setTag(modules.size());

    modules.emplace(module, node);

    scroll->m_contentLayer->addChild(node);

    updateUI();
}

void CategoryNode::removeModule(Module* module)
{
    if (!modules.contains(module))
        return;

    auto node = modules[module];
    node->removeFromParent();
    modules.erase(module);

    updateUI();
}

void CategoryNode::removeAll()
{
    for (auto node : modules)
    {
        node.second->removeFromParent();
    }

    modules.clear();
    updateUI();
}

void CategoryNode::updateUI()
{
    bool showScrollbar = shouldScrollbarShow();

    float height = std::max<float>((std::floor((modules.size() / 2.0f) + 1)) * 28.0f, scroll->getContentHeight());
    float height2 = height - (28 / 2) - 3;

    scroll->m_contentLayer->setContentHeight(height + 6);

    for (auto node : modules)
    {
        auto n = node.second;

        float x = (n->getTag() % 2 == 0) ? 85 : (showScrollbar ? 255 : 260);
        float y = floor(n->getTag() / 2);

        n->setPosition(ccp(x, height2 - (y * 28) + 6));
    }

    scroll->moveToTop();
    scroll->setTouchEnabled(height != scroll->getContentHeight());
    scrollbar->setVisible(showScrollbar);
}

bool CategoryNode::shouldScrollbarShow()
{
    return false;
}

bool CategoryNode::init()
{
    if (!CCNode::init())
        return false;

    this->setAnchorPoint(ccp(1, 0.5f));
    this->setContentSize(ccp(340, 280 - 10 * 2));
    this->ignoreAnchorPointForPosition(false);

    auto bg = CCScale9Sprite::create("square02b_small.png");
    bg->setContentSize(this->getContentSize() / 0.5f);
    bg->setScale(0.5f);
    bg->setColor(ccc3(0, 0, 0));
    bg->setOpacity(100);

    scroll = geode::ScrollLayer::create(this->getContentSize());
    scroll->m_peekLimitTop = 15;
    scroll->m_peekLimitBottom = 15;

    scrollbar = Scrollbar::create(scroll);
    // updates size
    scrollbar->visit();
    public_cast(scrollbar, m_track)->setScale(0.5f);
    scrollbar->setVisible(shouldScrollbarShow());
    
    this->addChildAtPosition(bg, Anchor::Center);
    this->addChild(scroll);
    this->addChildAtPosition(scrollbar, Anchor::Right, ccp(-4, 0));
    return true;
}