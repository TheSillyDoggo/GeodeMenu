#include "UIComponent.hpp"

void UIComponent::drawImGui()
{
    
}

void UIComponent::refreshNode()
{
    
}

void UIComponent::setParent(CCNode* parent)
{
    CCNode::setParent(parent);

    this->removeAllChildrenWithCleanup(true);
    refreshNode();
}