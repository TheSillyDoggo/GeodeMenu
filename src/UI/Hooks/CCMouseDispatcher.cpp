#include <MouseDispatcherExt.hpp>
#include <Num.hpp>
#include <Utils.hpp>

#ifndef GEODE_IS_IOS

bool MouseDispatcherExt::handleForNodeRecursive(CCNode* node, float y, float x)
{
    if (!node || !node->isVisible())
        return false;

    if (auto delegate = typeinfo_cast<CCMouseDelegate*>(node))
    {
        bool enabled = true;

        if (auto layer = typeinfo_cast<CCLayer*>(node))
        {
            if (!layer->isMouseEnabled())
                enabled = false;
        }

        if (enabled)
        {
            if (node->getUserObject("MouseDispatcherExt"_spr))
            {
                auto local = node->convertToNodeSpace(getMousePos());
                auto r = qolmod::utils::getBasicRect(node);
                r.origin = CCPointZero;

                if (r.containsPoint(local))
                {
                    delegate->scrollWheel(y, x);
                    return true;
                }
            }
        }
    }

    std::vector<CCNode*> children = std::vector<CCNode*>(node->getChildrenExt().begin(), node->getChildrenExt().end());
    std::reverse(children.begin(), children.end());

    for (auto child : children)
    {
        if (handleForNodeRecursive(child, y, x))
            return true;
    }

    return false;
}

bool MouseDispatcherExt::dispatchScrollMSG(float y, float x)
{
    #if GEODE_COMP_GD_VERSION >= 22081

    if (handleForNodeRecursive(OverlayManager::get(), y, x))
        return true;

    #endif

    if (handleForNodeRecursive(CCScene::get(), y, x))
        return true;

    return CCMouseDispatcher::dispatchScrollMSG(y, x);
}

void MouseDispatcherExt::onModify(auto& self)
{
    // (void)self.setHookPriorityPost("cocos2d::CCMouseDispatcher::dispatchScrollMSG", Priority::Replace);
}

#endif