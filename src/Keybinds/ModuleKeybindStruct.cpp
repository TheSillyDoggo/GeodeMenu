#include "ModuleKeybindStruct.hpp"
#include "../Client/Module.hpp"
#include "../Client/ModuleNode.hpp"

void ModuleKeybindStruct::onActivate(KeyState state)
{
    if (auto mod = Module::getByID(modID))
    {
        if (type == KeybindType::Toggle)
        {
            if (state.isDown && !state.isRepeat)
            {
                mod->setUserEnabled(!mod->getUserEnabled());
                mod->onToggle();

                ModuleNode::updateAllNodes(nullptr);
            }
        }

        if (type == KeybindType::Hold)
        {
            if (state.isDown && !state.isRepeat)
            {
                mod->setUserEnabled(!invertHold);
                mod->onToggle();

                ModuleNode::updateAllNodes(nullptr);
            }

            if (!state.isDown && !state.isRepeat)
            {
                mod->setUserEnabled(invertHold);
                mod->onToggle();

                ModuleNode::updateAllNodes(nullptr);
            }
        }
    }
}