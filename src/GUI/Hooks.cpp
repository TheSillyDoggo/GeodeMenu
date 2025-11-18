#include "Hooks.hpp"
#include "AndroidUI.hpp"
#include "AndroidBall.hpp"

void BallEGLView::swapBuffers()
{
    if (!AndroidUI::get())
        AndroidBall::get()->visit();

    CCEGLView::swapBuffers();
}

void BallGameManager::reloadAllStep5()
{
    GameManager::reloadAllStep5();

    AndroidBall::get()->reloadTextures();
}

void BallTouchDispatcher::touches(CCSet *pTouches, CCEvent *pEvent, unsigned int uIndex)
{
    if (pTouches && AndroidBall::get()->shouldFunction() && !AndroidUI::get())
    {
        if (auto t = static_cast<CCTouch*>(pTouches->anyObject()))
        {
            switch (uIndex)
            {
                case ccTouchType::CCTOUCHBEGAN:
                    if (AndroidBall::get()->ccTouchBegan(t))
                        return;
                    
                    break;

                case ccTouchType::CCTOUCHMOVED:
                    if (AndroidBall::get()->ccTouchMoved(t))
                        return;
                    
                    break;

                case ccTouchType::CCTOUCHENDED:
                    if (AndroidBall::get()->ccTouchEnded(t))
                        return;
                    
                    break;
            }
        }
    }

    CCTouchDispatcher::touches(pTouches, pEvent, uIndex);
}