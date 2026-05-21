#include "LayoutModule.hpp"
#include "ButtonModuleNode.hpp"
#include <EditLayoutUI.hpp>

void LayoutModule::save()
{

}

void LayoutModule::load()
{

}


void LayoutModule::setPreviewNode(std::function<cocos2d::CCNode*()> preview)
{
    this->previewNode = std::move(preview);
}

cocos2d::CCNode* LayoutModule::createPreviewNode()
{
    return previewNode();
}

ModuleNode* LayoutModule::getNode()
{
    return ButtonModuleNode::create(this);
}

void LayoutModule::onClick()
{
    qolmod::EditLayoutUI::create(this)->show();
}