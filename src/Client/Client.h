#pragma once

#include "../Defines/Platform.h"

#include "Types/InputModule.hpp"
#include "Types/ColourPickModule.hpp"
#include "Types/DropdownModule.hpp"
#include "Types/SliderModule.hpp"
#include "Types/TransitionCustomizerUI.hpp"
#include "Types/SmartStartposUI.hpp"
#include "Types/SetValueModule.hpp"

#include "idkwhattocallthis.hpp"

#include "Window.h"

class Client
{
public:
    static inline Client* instance = nullptr;
    static inline Mod* mod = nullptr;

    std::vector<Window*> windows;
    static inline CCPoint tileSize = CCPoint(42, 9);
    float animStatus = 0;
    float delta = 0;
    
    bool open;

    bool over = false;

    Client()
    {
        mod = Mod::get();
    }

    //[[deprecated("GetModuleEnabled has been deprecated due to lag, please rember to cache the module :3")]]
    static bool GetModuleEnabled(std::string id)
    {
        if (!mod)
            mod = Mod::get();

        return mod->getSavedValue<bool>(fmt::format("{}_enabled", id));
    }

    static Module* GetModule(std::string id)
    {
        if (!instance)
            return nullptr;

        for (size_t w = 0; w < instance->windows.size(); w++)
        {
            for (size_t m = 0; m < instance->windows[w]->modules.size(); m++)
            {
                if (!instance->windows[w]->modules[m]->id.compare(id))
                {
                    return instance->windows[w]->modules[m];
                }
            }
        }

        //geode::prelude::log::info("missing module :( {}", id);

        return nullptr;
    }
};