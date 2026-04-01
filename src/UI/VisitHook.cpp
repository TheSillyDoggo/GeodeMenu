#include "VisitHook.hpp"
#include <Geode/modify/CCGridBase.hpp>

using namespace geode::prelude;
using namespace qolmod;

bool VisitHook::init()
{
    initWithSize(CCSizeMake(100, 100));
    m_bActive = true;
    return true;
}

void VisitHook::setCallbackPre(const VisitHook::VisitCallbackFunc& func)
{
    this->callbackPre = std::move(func);
}

void VisitHook::setCallbackPost(const VisitHook::VisitCallbackFunc& func)
{
    this->callbackPost = std::move(func);
}

void VisitHook::preVisit()
{
    if (callbackPre)
        callbackPre(nullptr);
}

void VisitHook::postVisit(cocos2d::CCNode* node)
{
    if (callbackPost)
        callbackPost(node);
}

class $modify (VisitHookGridBase, CCGridBase)
{
    void beforeDraw(void)
    {
        if (auto hook = typeinfo_cast<VisitHook*>(this))
        {
            hook->preVisit();
            return;
        }

        CCGridBase::beforeDraw();
    }

    void afterDraw(CCNode *pTarget)
    {
        if (auto hook = typeinfo_cast<VisitHook*>(this))
        {
            hook->postVisit(pTarget);
            return;
        }

        CCGridBase::afterDraw(pTarget);
    }
};