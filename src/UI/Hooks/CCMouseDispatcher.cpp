#include <MouseDispatcherExt.hpp>
#include <Num.hpp>

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
                if (getWorldSpaceBoundingBox(node).containsPoint(getMousePos()))
                {
                    delegate->scrollWheel(y, x);
                    return true;
                }
            }
        }
    }

    std::vector<CCNode*> children = CCArrayExt<CCNode*>(node->getChildren()).toVector();
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
    if (handleForNodeRecursive(OverlayManager::get(), y, x))
        return true;

    if (handleForNodeRecursive(CCScene::get(), y, x))
        return true;

    return CCMouseDispatcher::dispatchScrollMSG(y, x);
}

void MouseDispatcherExt::onModify(auto& self)
{
    // (void)self.setHookPriorityPost("cocos2d::CCMouseDispatcher::dispatchScrollMSG", Priority::Replace);
}