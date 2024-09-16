#include "Client.h"

Client* Client::get()
{
    return instance;
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
    return Mod::get()->getSettingValue<bool>("use-imgui-ui");
}

void Client::initImGui()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImGuiIO* io = &ImGui::GetIO();

    io->ConfigWindowsMoveFromTitleBarOnly = true;

    style->FramePadding = ImVec2(3, 6);
    style->ItemSpacing = ImVec2(0, 0);
    style->WindowPadding = ImVec2(0, 0);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(20.0f / 255, 20.0f / 255, 20.0f / 255, 1);
    style->ScrollbarSize = 8;

    auto font = ImGui::GetIO().Fonts->AddFontFromFileTTF((Mod::get()->getResourcesDir() / "Poppins-Regular.ttf").string().c_str(), 20.0f);
    io->FontDefault = font;

    sortWindows(true);
    toggleWindowVisibility(WindowTransitionType::None);
}

void Client::drawImGui()
{
    ImGui::GetStyle().Alpha = bgOpacity->getDisplayedOpacity() / 255.0f;

    for (auto window : windows)
    {
        if (window->isVisible())
            window->drawImGui();
    }

    ImGui::ShowStyleEditor();

    if (CCKeyboardDispatcher::get()->getShiftKeyPressed() && !windows[0]->getActionByTag(69))
        sortWindows(false);
}

void Client::sortWindows(bool instant)
{
    ImVec2 configSize;

    for (auto window : windows)
    {
        ImVec2 posOffset = ImVec2(15, 15);

        if (window->id == "cosmetic-window")
            posOffset.x = (225 + 15) * 1;

        if (window->id == "universal-window")
            posOffset.x = (225 + 15) * 2;

        if (window->id == "creator-window")
        {
            posOffset.x = (225 + 15) * 3;
            configSize = window->getDesiredWindowSize();
        }

        if (window->id == "speedhack-window")
        {
            posOffset.x = (225 + 15) * 3;
            posOffset.y = 15 * 2 + configSize.y;
        }

        if (window->id == "labels-window")
            posOffset.x = (225 + 15) * 4;

        if (window->id == "icon-effects")
            posOffset.x = (225 + 15) * 5;

        if (window->id == "config-window")
            posOffset.x = (225 + 15) * 6;

        if (instant)
        {
            window->setPosition(ccp(posOffset.x, posOffset.y));
            window->actualWindowPos = posOffset;
        }
        else
        {
            if (window->getActionByTag(69))
                window->stopActionByTag(69);
            
            auto action = CCEaseInOut::create(CCMoveTo::create(0.5f, ccp(posOffset.x, posOffset.y)), 2);
            action->setTag(69);

            window->runAction(action);
            window->actualWindowPos = posOffset;
        }
    }
}

void Client::toggleWindowVisibility(WindowTransitionType type)
{
    isWindowOpen = !isWindowOpen;

    for (auto window : windows)
    {
        CCAction* fade;
        CCAction* verticalMove;

        switch (type)
        {
            default:
                bgOpacity->setOpacity(isWindowOpen ? 255 : 0);
                break;

            case WindowTransitionType::Vertical:
                bgOpacity->setOpacity(255);

                if (window->getActionByTag(69))
                    window->stopActionByTag(69);

                bool up = ((as<int>(window->getPosition().x) - 15) % 235 * 2) == 0 ? true : false;

                if (window->windowPos.x == window->actualWindowPos.x && window->windowPos.y == window->actualWindowPos.y)
                    window->setPosition(ccp(window->actualWindowPos.x, window->actualWindowPos.y + (!isWindowOpen ? 0 : (ImGui::GetIO().DisplaySize.y + window->getDesiredWindowSize().y) * (up ? 1 : -1))));
                
                verticalMove = CCEaseInOut::create(CCMoveTo::create(0.5f, ccp(window->actualWindowPos.x, window->actualWindowPos.y + (isWindowOpen ? 0 : (ImGui::GetIO().DisplaySize.y + window->getDesiredWindowSize().y) * (up ? 1 : -1)))), 2);
                verticalMove->setTag(69);

                window->runAction(verticalMove);

                CCScene::get()->addChild(window);
                break;
        }
    }
}
