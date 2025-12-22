#include "Hooks.hpp"
#include "AndroidUI.hpp"
#include "AndroidBall.hpp"

bool BallMenuLayer::init()
{
    if (!MenuLayer::init())
        return false;

    if (AndroidBall::get()->areImportantTexturesMissing())
    {
        Loader::get()->queueInMainThread([this]
        {
            FLAlertLayer::create("QOLMod", "Geode <cr>failed</c> to extract assets <cl>required</c> for <co>QOLMod</c> to function.\nIt is <cc>recommended</c> that you <cl>reinstall</c> <co>QOLMod</c> to avoid <cr>crashes</c>", "OK")->show();
        });
    }

    return true;
}

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