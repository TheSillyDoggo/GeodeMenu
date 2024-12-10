#pragma once

#include <imgui-cocos.hpp>
#include "../Defines/Platform.h"

#include "Types/InputModule.hpp"
#include "Types/ColourPickModule.hpp"
#include "Types/DropdownModule.hpp"
#include "Types/SliderModule.hpp"
#include "Types/TransitionCustomizerUI.hpp"
#include "Types/SmartStartposUI.hpp"
#include "Types/SetValueModule.hpp"
#include "Types/FontModule.hpp"
#include "../Labels/LabelModule.hpp"

#include "idkwhattocallthis.hpp"
#include "../UI/BlurLayer.hpp"
#include "Window.h"
#include "../Utils/SimpleINI.hpp"

enum class WindowTransitionType
{
    None,
    UserSelected, // Only used for calling the function
    Fade,
    Vertical,
};

class Client
{
public:
    static inline Client* instance = nullptr;
    static inline Mod* mod = nullptr;

    std::vector<Window*> windows;
    static inline CCPoint tileSize = CCPoint(42, 9);
    float animStatus = 0;
    float delta = 0;
    
    bool isWindowOpen = true;
    ccColor4B accentColour = ccc4(207, 67, 115, 255);

    Module* hoveredModule = nullptr;
    Module* optionsModule = nullptr;
    ImFont* font = nullptr;
    Ref<BlurLayer> blurLayer;
    bool over = false;
    SimpleINI* ini = nullptr;
    ImVec2 widgetSize = ImVec2(215, 25);

    Client();

    static Client* get();

    bool handleKeybinds(enumKeyCodes key, bool isDown, bool isRepeatedKey);
    
    bool useImGuiUI();
    void initImGui();
    void drawImGui();
    void sortWindows(bool instant);
    void toggleWindowVisibility(WindowTransitionType type, bool instant = false);

    void loadImGuiTheme(std::string theme);

    ccColor4B getThemeColour(std::string key, ccColor4B def);

    void setUIScale(float scale);

    void setLanguage(std::string langFile);

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