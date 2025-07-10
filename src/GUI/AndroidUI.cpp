#include "AndroidUI.hpp"
#include "../Client/Module.hpp"
#include "../Client/ModuleNode.hpp"
#include "../Client/CategoryNode.hpp"

bool AndroidUI::setup()
{
    bg = BackgroundSprite::create();
    bg->setContentSize(m_size);
    bg->setAnchorPoint(ccp(0, 0));
    bg->updateLayout();

    m_bgSprite->setVisible(false);
    m_buttonMenu->setVisible(false);

    populateModules();

    m_mainLayer->addChild(bg, -1);
    return true;
}

void AndroidUI::populateModules()
{
    auto menu = CCNode::create();
    menu->setContentSize(ccp(420, 280));

    std::unordered_map<std::string, CategoryNode*> categories = {};

    for (auto module : Module::moduleMap)
    {
        if (!categories.contains(module.second->getCategory()))
        {
            auto cat = CategoryNode::create();
            categories.emplace(module.second->getCategory(), cat);

            menu->addChildAtPosition(cat, Anchor::Right, ccp(-10, 0));
        }

        categories[module.second->getCategory()]->addModule(module.second);
        categories[module.second->getCategory()]->addModule(module.second);
        categories[module.second->getCategory()]->addModule(module.second);
        categories[module.second->getCategory()]->addModule(module.second);
    }

    m_mainLayer->addChild(menu);
}

AndroidUI* AndroidUI::create()
{
    auto pRet = new AndroidUI();

    if (pRet && pRet->initAnchored(420.f, 280.f))
    {
        PlatformToolbox::showCursor();
        instance = pRet;
        pRet->m_noElasticity = true;

        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

AndroidUI* AndroidUI::addToScene()
{
    auto pRet = create();
    pRet->show();

    return pRet;
}

AndroidUI* AndroidUI::get()
{
    return instance;
}

AndroidUI::~AndroidUI()
{
    instance = nullptr;
}

void AndroidUI::close()
{
    if (PlayLayer::get() && !PlayLayer::get()->m_isPaused && !PlayLayer::get()->m_levelEndAnimationStarted && !GameManager::sharedState()->getGameVariable("0024"))
        PlatformToolbox::hideCursor();

    this->onClose(nullptr);
}

void AndroidUI::test(CCObject* sender)
{
    auto mod = Module::getByID("noclip");

    mod->setUserEnabled(!mod->getUserEnabled());
}