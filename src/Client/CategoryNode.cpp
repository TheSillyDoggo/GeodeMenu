#include "CategoryNode.hpp"
#include "../Utils/Casts.hpp"
#include "../GUI/BetterMouseDispatcher.hpp"
#include "../Utils/Num.hpp"
#include <ScrollLayer.hpp>

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
    node->setLinkedRect(scroll);
    node->setTag(modules.size());

    modules.emplace(module, node);

    scroll->m_contentLayer->addChild(node);
    layoutIsDirty = true;
}

void CategoryNode::removeModule(Module* module)
{
    if (!modules.contains(module))
        return;

    auto node = modules[module];
    node->removeFromParent();
    modules.erase(module);

    layoutIsDirty = true;
}

void CategoryNode::removeAll()
{
    for (auto node : modules)
    {
        if (node.second)
            node.second->removeFromParent();
    }

    modules.clear();
    layoutIsDirty = true;
}

bool CategoryNode::shouldScrollbarShow()
{
    return alwaysShowScrollbar || (scroll->m_contentLayer->getContentHeight() > scroll->getContentHeight());
}

bool CategoryNode::init()
{
    if (!CCNode::init())
        return false;

    this->setAnchorPoint(ccp(1, 0.5f));
    this->setContentSize(ccp(340, 280 - 10 * 2));
    this->ignoreAnchorPointForPosition(false);

    bg = EasyBG::create();
    bg->setContentSize(getContentSize());

    scroll = qolmod::ScrollLayer::create(this->getContentSize());
    scroll->setAnchorPoint(ccp(0.5f, 0));

    scrollbar = BetterScrollbar::create(scroll);
    scrollbar->visit();
    scrollbar->setUseDynamicHandle(true);
    scrollbar->setVisible(shouldScrollbarShow());
    scrollbar->setAnchorPoint(ccp(1, 0));
    scrollbar->setDisabled(true);
    
    this->addChildAtPosition(bg, Anchor::Center);
    this->addChildAtPosition(scroll, Anchor::Bottom);
    this->addChildAtPosition(scrollbar, Anchor::BottomRight);
    return true;
}

void CategoryNode::updateUILayout()
{
    bg->setContentSize(getContentSize());
    scroll->setContentSize(getContentSize());
    updateLayout();

    constexpr float node_height = 28.0f;
    constexpr float scroll_padding = 3.0f;

    float contentHeight = std::max<float>(
        std::ceil(modules.size() / 2.0f) * node_height + scroll_padding * 2,
        scroll->getContentHeight()
    );

    scroll->m_contentLayer->setContentHeight(contentHeight);
    bool showScrollbar = shouldScrollbarShow();
    bool canScroll = contentHeight > scroll->getContentHeight();
    float usWidth = showScrollbar ? getContentWidth() - scrollbar->getScaledContentWidth() + 2.5f : getContentWidth();

    for (auto& node : modules)
    {
        auto n = node.second;
        int i = n->getTag();

        if (!n)
            continue;

        bool left = (i % 2 == 0);
        
        n->setAnchorPoint(ccp((float)!left, 0.5f));
        n->setPosition(ccp(
            left ? 2.5f : usWidth - 2.5f,
            contentHeight - (std::floor((float)i / 2.0f) + 0.5f) * node_height - scroll_padding
        ));
    }

    scroll->moveToTop();

    scroll->setTouchEnabled(canScroll);
    scroll->setMouseEnabled(canScroll);
    scrollbar->setVisible(showScrollbar);
    scrollbar->setDisabled(!canScroll);
}

void CategoryNode::visit()
{
    if (layoutIsDirty)
    {
        updateUILayout();
        layoutIsDirty = false;
    }

    CCNode::visit();
}

void CategoryNode::setContentSize(const CCSize& contentSize)
{
    CCMenu::setContentSize(contentSize);
    layoutIsDirty = true;
}