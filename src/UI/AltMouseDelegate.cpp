#include "AltMouseDelegate.hpp"
#include <Utils.hpp>

using namespace geode::prelude;
using namespace qolmod;

#if 0

void handleNode(CCNode* node, std::vector<CCNode*>* vector)
{
    if (!node->isVisible())
        return;

    vector->push_back(node);

    std::vector<CCNode*> children = std::vector<CCNode*>(node->getChildrenExt().begin(), node->getChildrenExt().end());
    // std::reverse(children.begin(), children.end());

    for (auto child : children)
    {
        handleNode(child, vector);
    }
}

$execute
{
    MouseInputEvent()
    .listen(+[](const geode::MouseInputData& event)
    {
        if ((int)event.button != 0 && event.action == MouseInputData::Action::Press)
        {
            std::vector<CCNode*> graph = {};
            handleNode(CCScene::get(), &graph);
            handleNode(OverlayManager::get(), &graph);
            std::reverse(graph.begin(), graph.end());

            auto pos = getMousePos();

            for (auto node : graph)
            {
                if (auto touch = CCTouchDispatcher::get()->findHandler(typeinfo_cast<CCTouchDelegate*>(node)))
                {
                    if (auto delegate = typeinfo_cast<AltMouseDelegate*>(node))
                    {
                        if (delegate->altClickBegan((int)event.button, pos))
                            return ListenerResult::Propagate;
                    }
                    else if (auto menu = typeinfo_cast<CCMenu*>(node))
                    {
                        for (auto child : menu->getChildrenExt<CCNode*>())
                        {
                            if (auto btn = typeinfo_cast<CCMenuItem*>(child))
                            {
                                if (child && child->isVisible() && btn->isEnabled())
                                {
                                    auto r = btn->rect();
                                    r.origin = CCPointZero;

                                    if (r.containsPoint(btn->convertToNodeSpace(pos)))
                                    {
                                        log::error("Swallowed by {}", btn);
                                        return ListenerResult::Propagate;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        auto r = qolmod::utils::getBasicRect(node);
                        r.origin = CCPointZero;

                        if (r.containsPoint(node->convertToNodeSpace(pos)))
                        {
                            log::error("Swallowed by {}", node);
                            return ListenerResult::Propagate;
                        }
                    }
                }
            }

            // log::info("graph: {}, {}, {}", graph[0], graph[1], graph[2]);
        }

        return ListenerResult::Propagate;
    }).leak();
};

#endif