#include "ButtonModule.hpp"
#include "ButtonModuleNode.hpp"

ModuleNode* ButtonModule::getNode()
{
    return ButtonModuleNode::create(this);
}

bool ButtonModule::shouldSave()
{
    return false;
}

void ButtonModule::onKeybindActivated(KeyState state)
{
    onClick();
}