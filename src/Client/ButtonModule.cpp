#include "ButtonModule.hpp"
#include "ButtonModuleNode.hpp"

ModuleNode* ButtonModule::getNode()
{
    return ButtonModuleNode::create(this);
}

void ButtonModule::onKeybindActivated(KeyState state)
{
    onClick();
}