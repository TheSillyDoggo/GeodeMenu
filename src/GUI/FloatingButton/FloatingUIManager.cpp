#include "FloatingUIManager.hpp"
#include "../AndroidUI.hpp"

using namespace geode::prelude;

FloatingUIManager* FloatingUIManager::get()
{
    static FloatingUIManager* instance = nullptr;

    if (!instance)
        instance = new FloatingUIManager();

    return instance;
}

void FloatingUIManager::addButton(FloatingUIButton* btn)
{
    buttons.push_back(btn);
}

void FloatingUIManager::removeButton(FloatingUIButton* btn)
{
    buttons.erase(std::remove(buttons.begin(), buttons.end(), btn), buttons.end());
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
    if (!CCScene::get() || CCScene::get()->getChildByType<LoadingLayer>(0))
        return;

    sortButtons();

    for (auto btn : buttons)
    {
        btn->visit();
    }
}

bool FloatingUIManager::touches(CCSet *pTouches, CCEvent *pEvent, unsigned int uIndex)
{
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