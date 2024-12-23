#include "Client.h"
#include "../Utils/LaunchArgs.hpp"
#include "../Utils/TranslationManager.hpp"
#include "../UI/TransLabelBMFont.hpp"
#include <Geode/modify/CCEGLView.hpp>
#include <regex>

Client* Client::get()
{
    return instance;
}

Client::Client()
{
    mod = Mod::get();

    setLanguage(Mod::get()->getSavedValue<std::string>("loaded-translation", "none"));
}

bool Client::handleKeybinds(enumKeyCodes key, bool isDown, bool isRepeatedKey)
{
    if (!isDown || key == enumKeyCodes::KEY_Unknown)
        return false;

    bool shouldPropogate = true;

    for (auto window : windows)
    {
        for (auto module : window->modules)
        {
            if (isRepeatedKey ? module->keybind.canRepeat : true)
            {
                bool shouldSend = true;

                if (module->keybind.key == enumKeyCodes::KEY_Unknown)
                    shouldSend = false;

                if (module->keybind.key != key)
                    shouldSend = false;

                if (module->keybind.shift && !CCKeyboardDispatcher::get()->getShiftKeyPressed())
                    shouldSend = false;

                if (module->keybind.control && !CCKeyboardDispatcher::get()->getControlKeyPressed())
                    shouldSend = false;

                if (module->keybind.alt && !CCKeyboardDispatcher::get()->getAltKeyPressed())
                    shouldSend = false;

                if (module->keybind.command && !CCKeyboardDispatcher::get()->getCommandKeyPressed())
                    shouldSend = false;

                if (shouldSend)
                {
                    module->onToggleAndroid(nullptr);

                    log::info("toggled: {}", module->id);

                    if (!shouldPropogate)
                        return true;
                }
            }
        }
    }

    return false;
}

bool Client::useImGuiUI()
{
    if (LaunchArgs::get()->hasLaunchArg("--qolmod:use-imgui-ui"))
        return true;

    if (LaunchArgs::get()->hasLaunchArg("--qolmod:no-imgui-ui"))
        return false;

    return Mod::get()->getSavedValue<bool>("use-imgui-ui");
}

void Client::initImGui()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImGuiIO* io = &ImGui::GetIO();

    io->ConfigWindowsMoveFromTitleBarOnly = true;

    style->FramePadding = ImVec2(4, 4);
    style->ItemSpacing = ImVec2(0, 0);
    style->WindowPadding = ImVec2(0, 0);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(20.0f / 255, 20.0f / 255, 20.0f / 255, 1);
    style->ScrollbarSize = 8;

    setUIScale(mod->getSavedValue<float>("imgui-ui-scale", 1.0f));

    accentColour = ccc4(238, 119, 98, 255);

    // loadImGuiTheme("catppuccin-frappe.ini");
    loadImGuiTheme("midgahack.ini");

    sortWindows(true);

    toggleWindowVisibility(WindowTransitionType::Vertical, true);

    blurLayer = BlurLayer::create();

    blurLayer->onEnter();
    blurLayer->onEnterTransitionDidFinish();

    for (auto window : windows)
    {
        window->onEnter();
        window->onEnterTransitionDidFinish();
    }

    Loader::get()->queueInMainThread([this]
    {
        toggleWindowVisibility(WindowTransitionType::Vertical, true);
    });
}

void Client::drawImGui()
{
    if (Client::GetModuleEnabled("menu-bg-blur") && !LaunchArgs::get()->hasLaunchArg("--qolmod:no-blur"))
        blurLayer->visit();

    // ImGui::GetStyle().Alpha = bgOpacity->getDisplayedOpacity() / 255.0f;

    hoveredModule = nullptr;

    for (auto window : windows)
    {
        if (window->isVisible())
            window->drawImGui();
    }

    if (optionsModule)
    {
        ImGui::SetNextWindowPos(ImVec2(optionsModule->lastRenderedPosition.x + widgetSize.y, optionsModule->lastRenderedPosition.y));
        ImGui::SetNextWindowSize(ImVec2(widgetSize.x, widgetSize.y * optionsModule->options.size()));

        ImGui::Begin("Module Options", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

        for (auto module : optionsModule->options)
        {
            ImGui::PushItemWidth(widgetSize.x);
            module->drawImGui();
        }

        ImGui::End();
    }

    ImGui::ShowStyleEditor();

    if (hoveredModule && !hoveredModule->description.empty())
    {
        // // ImGui::SetNextWindowPos(hoveredModule->lastRenderedPosition);

        // ImGui::SetNextWindowPos(ImVec2(ImGui::GetMousePos().x + 12.5f, ImGui::GetMousePos().y));

        // ImGui::Begin("Description Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_NoBackground);

        // ImGui::SetCursorPos(ImVec2(ImGui::GetMousePos().x + 12.5f, ImGui::GetMousePos().y));

        ImGuiExt::colouredText(hoveredModule->description);

        // ImGui::End();
    }
}

void Client::setLanguage(std::string langFile)
{
    auto path = Mod::get()->getResourcesDir() / langFile;

    if (std::filesystem::exists(path))
    {
        TranslationManager::get()->loadTranslationFromJson(file::readJson(path).unwrapOr("{ }"));
    }
    else
    {
        TranslationManager::get()->unloadTranslation();
    }

    TransLabelBMFont::updateAllLabels();

    Mod::get()->setSavedValue<std::string>("loaded-translation", langFile);
}

std::vector<std::filesystem::path> Client::getLanguages()
{
    std::vector<std::filesystem::path> files;

    for (auto file : std::filesystem::directory_iterator(Mod::get()->getResourcesDir()))
    {
        auto p = file.path().filename();

        if (p.has_extension() && p.extension().string() == ".json")
        {
            files.push_back(file);
        }
    }

    return files;
}

void Client::sortWindows(bool instant)
{
    float offset = ini->getKeyValueFloat("Offsets::WindowDistance", "15");

    float x = offset;
    std::map<float, float> yMap;
    bool stacking = false;

    std::sort(Client::instance->windows.begin(), Client::instance->windows.end(), [](Window* a, Window* b)
    {
        return a->priority < b->priority;
    });

    for (auto window : windows)
    {
        if (!yMap.contains(x))
            yMap[x] = offset;
        
        if (x + window->getDesiredWindowSize().x > ImGui::GetIO().DisplaySize.x)
        {
            x = offset;
            stacking = true;
        }

        ImVec2 wndPos = ImVec2(x, yMap[x + window->getDesiredWindowSize().x + offset]);

        for (size_t i = 0; i < 8; i++)
        {
            if (wndPos.y + window->getDesiredWindowSize().y > ImGui::GetIO().DisplaySize.y)
            {
                x += window->getDesiredWindowSize().x + offset;

                wndPos = ImVec2(x, yMap[x + window->getDesiredWindowSize().x + offset]);
            }
        }
        
        x += window->getDesiredWindowSize().x + offset;
        yMap[x] += window->getDesiredWindowSize().y + offset;

        wndPos.y += offset;

        if (instant)
        {
            window->setPosition(ccp(wndPos.x, wndPos.y));
            window->actualWindowPos = wndPos;
        }
        else
        {
            if (window->getActionByTag(69))
                window->stopActionByTag(69);
            
            auto action = CCEaseInOut::create(CCMoveTo::create(instant ? 0 : 0.5f, ccp(wndPos.x, wndPos.y)), 2);
            action->setTag(69);

            window->runAction(action);
            window->actualWindowPos = wndPos;
        }
    }

    isWindowOpen = true;
}

void Client::toggleWindowVisibility(WindowTransitionType type, bool instant)
{
    ImGui::SetWindowFocus(nullptr);

    isWindowOpen = !isWindowOpen;

    if (blurLayer)
        blurLayer->runAction(CCFadeTo::create(!isWindowOpen ? 0.15f : 0.5f, isWindowOpen ? 255 : 0));

    std::vector<Window*> windows = this->windows;

    std::sort(windows.begin(), windows.end(), [](Window* a, Window* b)
    {
        return a->getPosition().x < b->getPosition().x;
    });

    bool verticalUp;
    float lastX = 0;

    for (auto window : windows)
    {
        CCAction* fade;
        CCAction* verticalMove;

        switch (type)
        {
            default:
                break;

            case WindowTransitionType::Vertical:
                if (window->getActionByTag(69))
                    window->stopActionByTag(69);

                bool verticalUp = window->getPosition().y > ImGui::GetIO().DisplaySize.y / 2;

                if (window->windowPos.x == window->actualWindowPos.x && window->windowPos.y == window->actualWindowPos.y)
                {
                    window->setPosition(ccp(window->actualWindowPos.x, window->actualWindowPos.y + (!isWindowOpen ? 0 : (ImGui::GetIO().DisplaySize.y + window->getDesiredWindowSize().y) * (verticalUp ? 1 : -1))));
                }
                
                verticalMove = CCEaseInOut::create(CCMoveTo::create(instant ? 0 : 0.5f, ccp(window->actualWindowPos.x, window->actualWindowPos.y + (isWindowOpen ? 0 : (ImGui::GetIO().DisplaySize.y + window->getDesiredWindowSize().y) * (verticalUp ? 1 : -1)))), 2);
                verticalMove->setTag(69);

                window->runAction(verticalMove);

                lastX = window->getPosition().x;
                break;
        }
    }
}

void Client::setUIScale(float scale)
{
    float oldScale = ImGuiCocos::get().getUIScale();

    scale = clamp<float>(abs(scale), 0.3f, 3);

    mod->setSavedValue<float>("imgui-ui-scale", scale);

    ImGuiCocos::get().setUIScale(scale);

    if (!font)
    {
        font = ImGui::GetIO().Fonts->AddFontFromFileTTF((Mod::get()->getResourcesDir() / "Poppins-Regular.ttf").string().c_str(), 16.0f * scale);
        ImGui::GetIO().FontDefault = font;
    }

    font->FontSize = 16 * scale;
    font->Scale = 1.0f / scale;

    for (auto window : windows)
    {
        window->setPosition(window->getPosition() / (scale / oldScale));
    }

    Loader::get()->queueInMainThread([this, oldScale, scale]
    {
        Loader::get()->queueInMainThread([this, oldScale, scale]
        {
            sortWindows(false);
        });
    });
}

#define THEME_COLOUR(__name__) \
style->Colors[ImGuiCol_##__name__] = ccc4ToVec(getThemeColour(#__name__, vecToCCC4(style->Colors[ImGuiCol_##__name__])))

ccColor4B Client::getThemeColour(std::string key, ccColor4B def)
{
    if (!ini->hasKey(fmt::format("Colors::{}", key)))
        return def;
    
    auto res = cc4bFromHexString(ini->getKeyValue(fmt::format("Colors::{}", key), ""), false, false);

    if (res.isOk())
        return res.unwrapOr(def);

    if (res.isErr())
        log::error("Failed to get colour {}, {}", key, res.err());

    return def;
}

void Client::loadImGuiTheme(std::string theme)
{
    if (ini)
    {
        CC_SAFE_DELETE(ini);
        ini = nullptr;
    }

    ini = SimpleINI::createWithFile((Mod::get()->getResourcesDir() / theme).string());

    ini->addVariable("accent_colour", fmt::format("#{}", cc4bToHexString(accentColour)));

    widgetSize = ImVec2(ini->getKeyValueFloat("WidgetSize::Width", "215"), ini->getKeyValueFloat("WidgetSize::Height", "25"));

    auto style = &ImGui::GetStyle();

    THEME_COLOUR(Text);
    THEME_COLOUR(TextDisabled);
    THEME_COLOUR(WindowBg);
    THEME_COLOUR(ChildBg);
    THEME_COLOUR(PopupBg);
    THEME_COLOUR(Border);
    THEME_COLOUR(BorderShadow);
    THEME_COLOUR(FrameBg);
    THEME_COLOUR(FrameBgHovered);
    THEME_COLOUR(FrameBgActive);
    THEME_COLOUR(TitleBg);
    THEME_COLOUR(TitleBgActive);
    THEME_COLOUR(TitleBgCollapsed);
    THEME_COLOUR(MenuBarBg);
    THEME_COLOUR(ScrollbarBg);
    THEME_COLOUR(ScrollbarGrab);
    THEME_COLOUR(ScrollbarGrabHovered);
    THEME_COLOUR(ScrollbarGrabActive);
    THEME_COLOUR(CheckMark);
    THEME_COLOUR(SliderGrab);
    THEME_COLOUR(SliderGrabActive);
    THEME_COLOUR(Button);
    THEME_COLOUR(ButtonHovered);
    THEME_COLOUR(ButtonActive);
    THEME_COLOUR(Header);
    THEME_COLOUR(HeaderHovered);
    THEME_COLOUR(HeaderActive);
    THEME_COLOUR(Separator);
    THEME_COLOUR(SeparatorHovered);
    THEME_COLOUR(SeparatorActive);
    THEME_COLOUR(ResizeGrip);
    THEME_COLOUR(ResizeGripHovered);
    THEME_COLOUR(ResizeGripActive);
    THEME_COLOUR(Tab);
    THEME_COLOUR(TabHovered);
    THEME_COLOUR(TabActive);
    THEME_COLOUR(TabUnfocused);
    THEME_COLOUR(TabUnfocusedActive);
    THEME_COLOUR(PlotLines);
    THEME_COLOUR(PlotLinesHovered);
    THEME_COLOUR(PlotHistogram);
    THEME_COLOUR(PlotHistogramHovered);
    THEME_COLOUR(TableHeaderBg);
    THEME_COLOUR(TableBorderStrong);
    THEME_COLOUR(TableBorderLight);
    THEME_COLOUR(TableRowBg);
    THEME_COLOUR(TableRowBgAlt);
    THEME_COLOUR(TextSelectedBg);
    THEME_COLOUR(DragDropTarget);
    THEME_COLOUR(NavHighlight);
    THEME_COLOUR(NavWindowingHighlight);
    THEME_COLOUR(NavWindowingDimBg);
    THEME_COLOUR(ModalWindowDimBg);
}

bool Client::GetModuleEnabled(std::string id)
{
    if (!mod)
        mod = Mod::get();

    return mod->getSavedValue<bool>(fmt::format("{}_enabled", id));
}

Module* Client::GetModule(std::string id)
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