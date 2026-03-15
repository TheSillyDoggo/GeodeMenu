#include "FloatingUIManager.hpp"
#include "../AndroidUI.hpp"
#include "../Modules/DisableShortcuts.hpp"
#include "../../Hacks/Universal/Paint/PaintControl.hpp"

using namespace geode::prelude;

FloatingUIManager* FloatingUIManager::get()
{
    static FloatingUIManager* instance = nullptr;

    if (!instance)
    {
        instance = new FloatingUIManager();
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
    qolmod::PaintControl::get()->preVisit();

    if (DisableShortcuts::get()->getRealEnabled())
    {
        qolmod::PaintControl::get()->visit();
        return;
    }

    sortButtons();

    return CCNode::visit();
}

bool FloatingUIManager::touches(CCSet *pTouches, CCEvent *pEvent, unsigned int uIndex)
{
    if (!CCScene::get() || CCScene::get()->getChildByType<LoadingLayer>(0))
        return false;

    if (AndroidUI::get())
        return false;

    sortButtons();
    std::reverse(buttons.begin(), buttons.end());

    switch (uIndex)
    {
        case CCTOUCHBEGAN:
            for (auto button : buttons)
            {
                if (DisableShortcuts::get()->getRealEnabled())
                    if (!typeinfo_cast<qolmod::PaintControl*>(button))
                        continue;

                for (auto item : *pTouches->m_pSet)
                {
                    if (auto touch = typeinfo_cast<CCTouch*>(item))
                    {
                        if (button->ccTouchBegan(touch))
                        {
                            trackingTouches.emplace(touch, button);
                            return true;
                        }
                    }
                }
            }
            
            break;

        case CCTOUCHMOVED:
            for (auto tracked : trackingTouches)
            {
                tracked.second->ccTouchMoved(tracked.first);
            }

            break;

        case CCTOUCHENDED:
            for (auto item : *pTouches->m_pSet)
            {
                if (auto touch = typeinfo_cast<CCTouch*>(item))
                {
                    if (trackingTouches.contains(touch))
                    {
                        trackingTouches[touch]->ccTouchEnded(touch);
                        trackingTouches.erase(touch);
                    }
                }
            }

            break;
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