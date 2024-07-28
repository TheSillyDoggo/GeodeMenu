#include "UIComponent.hpp"

void UIComponent::refreshNode()
{
    
}

void UIComponent::setParent(CCNode* parent)
{
    CCNode::setParent(parent);

    this->removeAllChildrenWithCleanup(true);
    refreshNode();
}