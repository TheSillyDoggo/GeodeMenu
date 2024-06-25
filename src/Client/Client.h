#pragma once

#include "Window.h"
#include "../Defines/Platform.h"

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

    void draw()
    {
        Module::descMod = "";

        over = false;

        style();
        instance = this;

        ImGui::Begin("draw", 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMouseInputs);
        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::SetWindowSize(ImGui::GetIO().DisplaySize);

        auto v = std::vector<float> {};
        for (size_t i = 0; i < windows.size(); i++)
        {
            v.push_back(windows[i]->windowPos.x);
        }
        std::sort(v.begin(), v.end(), std::greater<float>());
        std::reverse(v.begin(), v.end());

        bool a = false;
        for (size_t i = 0; i < windows.size(); i++)
        {
            //windows[i]->a = a;
            //windows[i]->i = windows[i]->getIndex(v, windows[i]->windowPos.x);
            //a = !a;

            //if (windows[i]->draw(false/*CompactMode::instance->enabled*/ ? tileSizeCompact : tileSize, animStatus))
                //over = true;
        }

        ImGui::SetNextWindowFocus();

        ImGui::GetIO().WantCaptureMouse = over;
    }

    void style()
    {
        ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

        ImGuiStyle* style = &ImGui::GetStyle();

        style->FramePadding = ImVec2(0, 0);
        style->WindowRounding = 0.0f;
        style->WindowPadding = ImVec2(0, 0);
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

    void onPostSetup();
};