#include "CategoryNode.hpp"

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

void CategoryNode::addModule(Module* module)
{
    auto node = module->getNode();

    modules.emplace(module, node);

    contentMenu->addChild(node);

    float height = (std::floor((modules.size() / 2.0f) + 1)) * 28.0f;
    float height2 = std::max<float>((std::floor((modules.size() / 2.0f) + 1)) * 28.0f, scroll->getContentHeight());

    scroll->m_contentLayer->setContentHeight(height2);
    contentMenu->setContentHeight(height2 + 6);
    contentMenu->setPositionY(scroll->getContentHeight() - height);

    int x = 0;
    int y = 0;

    for (auto node : modules)
    {
        node.second->setPosition(ccp(x == 0 ? 85 : 255, -(28 * y) + 28 / 2) + ccp(0, -3));

        x++;
        if (x == 2)
        {
            x = 0;
            y++;
        }
    }

    // x: 85, 255
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
    bg->setColor(ccColor3B(0, 0, 0));
    bg->setOpacity(100);

    scroll = geode::ScrollLayer::create(this->getContentSize());
    scroll->m_peekLimitTop = 15;
    scroll->m_peekLimitBottom = 15;
    
    contentMenu = CCMenu::create();
    contentMenu->ignoreAnchorPointForPosition(false);
    contentMenu->setContentSize(getContentSize());
    contentMenu->setPosition(ccp(0, 0));
    contentMenu->setAnchorPoint(ccp(0, 0));
    scroll->m_contentLayer->addChild(contentMenu);

    this->addChildAtPosition(bg, Anchor::Center);
    this->addChild(scroll);
    return true;
}