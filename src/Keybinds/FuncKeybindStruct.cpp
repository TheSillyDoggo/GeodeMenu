#include "FuncKeybindStruct.hpp"

void FuncKeybindStruct::onActivate(KeyState state)
{
    if (func)
        func(state);
}