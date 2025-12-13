#include "CategoryNode.hpp"
#include "../Utils/Casts.hpp"
#include "../GUI/BetterMouseDispatcher.hpp"

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
    float height = std::max<float>((std::floor((modules.size() / 2.0f) + (((modules.size() % 2) == 0) ? 0 : 1))) * 28.0f, scroll->getContentHeight());
    float height2 = height - (28 / 2) - 3;

    scroll->m_contentLayer->setContentHeight(height + (modules.size() == 0 ? 0 : 6));

    bool showScrollbar = shouldScrollbarShow();

    for (auto node : modules)
    {
        auto n = node.second;

        float x = (n->getTag() % 2 == 0) ? 85 : (showScrollbar ? 252 : 260);
        float y = floor(n->getTag() / 2);

        n->setPosition(ccp(x, height2 - (y * 28) + 6));
    }

    scroll->moveToTop();
    scroll->setTouchEnabled(height != scroll->getContentHeight());
    scrollbar->setVisible(showScrollbar);
    scrollbar->setDisabled(!scroll->isTouchEnabled());
}

bool CategoryNode::shouldScrollbarShow()
{
    //return false;
    return alwaysShowScrollbar ? true : scroll->isTouchEnabled();
}

bool CategoryNode::init()
{
    if (!CCNode::init())
        return false;

    this->setAnchorPoint(ccp(1, 0.5f));
    this->setContentSize(ccp(340, 280 - 10 * 2));
    this->ignoreAnchorPointForPosition(false);
    MouseDispatcher::betterMouseDispatcherDelegates.push_back(this);

    bg = CCScale9Sprite::create("square02b_small.png");
    bg->setContentSize(this->getContentSize() / 0.5f);
    bg->setScale(0.5f);
    bg->setColor(ccc3(0, 0, 0));
    bg->setOpacity(100);

    scroll = geode::ScrollLayer::create(this->getContentSize());
    scroll->m_peekLimitTop = 15;
    scroll->m_peekLimitBottom = 15;
    scroll->setTouchEnabled(false);
    scroll->setUserObject("user95401.scrollbar_everywhere/scrollbar", CCNode::create());

    scrollbar = BetterScrollbar::create(scroll);
    // updates size
    scrollbar->visit();
    scrollbar->setUseDynamicHandle(true);
    scrollbar->setVisible(shouldScrollbarShow());
    scrollbar->setAnchorPoint(ccp(1, 0));
    scrollbar->setDisabled(true);
    
    this->addChildAtPosition(bg, Anchor::Center);
    this->addChild(scroll);
    this->addChildAtPosition(scrollbar, Anchor::BottomRight);
    return true;
}

void CategoryNode::scrollWheel(float y, float x)
{
    if (nodeIsVisible(scroll) && scroll->isTouchEnabled())
        scroll->scrollLayer(y);
}

void CategoryNode::setContentSize(const CCSize& contentSize)
{
    CCMenu::setContentSize(contentSize);

    if (bg)
        bg->setContentSize(contentSize / 0.5f);

    if (scroll)
    {
        scroll->setContentSize(contentSize);
        updateUI();
    }

    updateLayout();
}

CategoryNode::~CategoryNode()
{
    MouseDispatcher::betterMouseDispatcherDelegates.erase(std::remove(MouseDispatcher::betterMouseDispatcherDelegates.begin(), MouseDispatcher::betterMouseDispatcherDelegates.end(), this), MouseDispatcher::betterMouseDispatcherDelegates.end());
}