#include "FloatingUIManager.hpp"
#include "../AndroidUI.hpp"
#include "../Modules/DisableShortcuts.hpp"

using namespace geode::prelude;

FloatingUIManager* FloatingUIManager::get()
{
    static FloatingUIManager* instance = nullptr;

    if (!instance)
    {
        instance = new FloatingUIManager();
        CCDirector::get()->m_pNotificationNode = new FloatingUIDrawHook();
    }

    return instance;
}

void FloatingUIManager::addButton(FloatingUIButton* btn)
{
    buttons.push_back(btn);
    this->addChild(btn);
}

void FloatingUIManager::removeButton(FloatingUIButton* btn)
{
    if (std::find(buttons.begin(), buttons.end(), btn) == buttons.end())
        return;

    buttons.erase(std::remove(buttons.begin(), buttons.end(), btn), buttons.end());
    this->removeChild(btn);
}

void FloatingUIManager::updateSprites()
{
    for (auto btn : buttons)
    {
        btn->updateSprites();
    }
}

int FloatingUIManager::getHighestButtonZ()
{
    int z = 0;

    for (auto btn : buttons)
    {
        z = std::max(z, btn->getZOrder());
    }

    return z;
}

void FloatingUIManager::visit()
{
    if (DisableShortcuts::get()->getRealEnabled())
        return;

    if (!CCScene::get() || CCScene::get()->getChildByType<LoadingLayer>(0))
        return;

    sortButtons();

    return CCNode::visit();
}

void FloatingUIDrawHook::visit()
{
    if (!AndroidUI::get())

    FloatingUIManager::get()->visit();
};

bool FloatingUIManager::touches(CCSet *pTouches, CCEvent *pEvent, unsigned int uIndex)
{
    if (DisableShortcuts::get()->getRealEnabled())
        return false;

    if (!CCScene::get() || CCScene::get()->getChildByType<LoadingLayer>(0))
        return false;

    if (AndroidUI::get())
        return false;

    sortButtons();
    std::reverse(buttons.begin(), buttons.end());

    if (auto touch = static_cast<CCTouch*>(pTouches->anyObject()))
    {
        switch (uIndex)
        {
            case ccTouchType::CCTOUCHBEGAN:
                selected = nullptr;

                for (auto button : buttons)
                {
                    if (button->ccTouchBegan(touch))
                    {
                        selected = button;
                        return true;
                    }
                }
                
                break;

            case ccTouchType::CCTOUCHMOVED:
                if (selected)
                {
                    selected->ccTouchMoved(touch);
                    return true;
                }
                
                break;

            case ccTouchType::CCTOUCHENDED:
                if (selected)
                {
                    selected->ccTouchEnded(touch);
                    return true;
                }

                selected = nullptr;
                break;
        }
    }

    return false;
}

void FloatingUIManager::sortButtons()
{
    std::sort(buttons.begin(), buttons.end(), [](FloatingUIButton* a, FloatingUIButton* b)
    {
        return a->getZOrder() < b->getZOrder();
    });
}