/*#include <Geode/Geode.hpp>
#include <Geode/modify/HardStreak.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

#define MEMBERBYOFFSET(type, class, offset) *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(class) + offset)
#define MBO MEMBERBYOFFSET

class $modify (CCKeyboardDispatcher)
{
    bool dispatchKeyboardMSG(enumKeyCodes key, bool isKeyDown, bool isKeyRepeat)
    {
        if (key == enumKeyCodes::CONTROLLER_X)
        {
            int s = 0x0;

            for (size_t i = 0; i < sizeof(PlayLayer::get()); i++)
            {
                auto arr = MBO(CCNode*, PlayLayer::get(), s);

                if (s == 2949)
                {
                    if (auto v = typeinfo_cast<StartPosObject*>(arr))
                    {
                        log::info("checkpoint: {}", v);
                    }
                }

                s += 0x1;
            }
        }

        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, isKeyDown, isKeyRepeat);
    }
};*/