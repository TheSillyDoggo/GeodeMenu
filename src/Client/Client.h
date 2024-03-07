#pragma once

#include "Window.h"

class Client
{
public:
    static inline Client* instance = nullptr;

    std::vector<Window*> windows;
    ImVec2 tileSize = ImVec2(150, 30);
    ImVec2 tileSizeCompact = ImVec2(150, 25.5f);
    float animStatus = 0;

    bool over = false;

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
            windows[i]->a = a;
            windows[i]->i = windows[i]->getIndex(v, windows[i]->windowPos.x);
            a = !a;

            if (windows[i]->draw(CompactMode::instance->enabled ? tileSizeCompact : tileSize, animStatus))
                over = true;
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

    [[deprecated("GetModuleEnabled has been deprecated due to lag, please rember to cache the module :3")]]
    static bool GetModuleEnabled(std::string id)
    {
        //if (CCScene::get())
            //CCScene::get()->addChild(TextAlertPopup::create("request to get '" + id + "'", 0.5f, 0.6f, 150, ""), 9999999);

        //log::info("get module: {}", id);

        for (size_t w = 0; w < instance->windows.size(); w++)
        {
            for (size_t m = 0; m < instance->windows[w]->modules.size(); m++)
            {
                if (!instance->windows[w]->modules[m]->id.compare(id))
                {
                    return instance->windows[w]->modules[m]->enabled;
                }
            }
        }
        
        //geode::prelude::log::info("missing module :( {}", id);

        return false;
    }

    static Module* GetModule(std::string id)
    {
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