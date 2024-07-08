#include "Client.h"
#include "../UI/PCDrawUtils.hpp"

void Window::drawWindow()
{
    if (modules.size() == 0)
        return;

    auto pos = windowPos + offsetForTime(Client::instance->animStatus);

    PCDrawUtils::drawRect(pos, Client::tileSize, ccc4(255, 0, 0, 255));

    int i = 0;

    for (auto module : modules)
    {
        auto point = pos + ccp(0, Client::tileSize.y * (i + 1));

        module->drawModule(point);

        i++;
    }
}

// do clipping stuff:
void Window::preDraw() {}
void Window::postDraw() {}

CCPoint Window::offsetForTime(float time)
{
    CCPoint wndSize = CCDirector::get()->getWinSize();

    return ccp(0, wndSize.y * (1 - quadraticEaseInOut(time)));
}

bool Window::touchBegan(CCPoint point, CCTouch* touch)
{
    auto pos = windowPos + offsetForTime(Client::instance->animStatus);
    auto rect = CCRectMake(pos.x, pos.y, Client::tileSize.x, Client::tileSize.y);
    auto wndRect = CCRectMake(pos.x, pos.y, Client::tileSize.x, Client::tileSize.y * (modules.size() + 1));

    if (rect.containsPoint(PCDrawUtils::getMousePosition()))
    {
        dragging = true;
        offset = rect.origin - PCDrawUtils::getMousePosition();

        return true;
    }
    else
    {
        int i = 0;

        for (auto mod : modules)
        {
            if (mod->touchBegan((pos - point) + ccp(0, Client::tileSize.y * i), touch))
                return true;

            i++;
        }
    }

    return false;
}

bool Window::touchMoved(CCPoint point, CCTouch* touch)
{
    auto pos = windowPos + offsetForTime(Client::instance->animStatus);
    auto wndRect = CCRectMake(pos.x, pos.y, Client::tileSize.x, Client::tileSize.y * (modules.size() + 1));

    if (dragging)
    {
        windowPos = PCDrawUtils::getMousePosition() + offset;
        windowPos.x = clampf(windowPos.x, 0, CCDirector::get()->getWinSize().width - Client::tileSize.x);
        windowPos.y = clampf(windowPos.y, 0, CCDirector::get()->getWinSize().height - (Client::tileSize.y * (modules.size() + 1)));

        return true;
    }

    if (true)
    {
        int i = 0;

        for (auto mod : modules)
        {
            if (mod->touchMoved((pos - point) + ccp(0, Client::tileSize.y * i), touch))
                return true;

            i++;
        }
    }

    return false;
}

bool Window::touchEndedOrCancelled(CCPoint point, CCTouch* touch, bool cancelled)
{
    dragging = false;

    return false;
}


/*
bool draw(ImVec2 tileSize, float anim = 1)
{
    if (modules.size() == 0)
        return false;

    ImVec2 wp = DrawUtils::addImVec2(windowPos, getOffsetForTime(anim));

    #ifdef GEODE_IS_WINDOWS

    ImVec2 clipRect;
    clipRect.x = wp.x + tileSize.x;
    clipRect.y = wp.y + tileSize.y * (1 + (modules.size() * quadraticEaseInOut(v)));

    ImGui::PushClipRect(wp, clipRect, true);

    //DrawUtils::drawRect(wp, tileSize, ColourUtility::GetColour(DrawUtils::mouseWithinRect(ImVec4(wp.x, wp.y, tileSize.x, tileSize.y)) ? (ImGui::IsMouseDown(ImGuiMouseButton_Left) ? ColourUtility::ClientColour::Pressed : ColourUtility::ClientColour::Hovered) : ColourUtility::ClientColour::WindowBG));
    DrawUtils::drawGradient(wp, tileSize, ColourUtility::GetColour(ColourUtility::ClientColour::GradientLeft), ColourUtility::GetColour(ColourUtility::ClientColour::GradientRight));

    ImGui::PushFont(DrawUtils::title);

    ImGui::SetCursorPos(DrawUtils::addImVec2(ImVec2(wp.x + (tileSize.x / 2) - (ImGui::CalcTextSize(name.c_str()).x / 2), wp.y + (tileSize.y / 2) - (ImGui::CalcTextSize(name.c_str()).y / 2)), ImVec2(1, 1)));
    ImGui::TextColored(ImVec4(0, 0, 0, 50.0f), name.c_str());

    ImGui::SetCursorPos(ImVec2(wp.x + (tileSize.x / 2) - (ImGui::CalcTextSize(name.c_str()).x / 2), wp.y + (tileSize.y / 2) - (ImGui::CalcTextSize(name.c_str()).y / 2)));
    ImGui::Text(name.c_str());

    ImGui::PopFont();

    if (mouseIn(ImVec4(windowPos.x + tileSize.x - 25, windowPos.y, 25, tileSize.y)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        isClosed = !isClosed;
    }

    if (isClosed)
    {
        DrawUtils::drawRect(DrawUtils::addImVec2(wp, ImVec2(tileSize.x - 20, tileSize.y / 2 - (1.75f / 2) - 6)), ImVec2(15, 1.75f), ImColor(255, 255, 255, 255));

        DrawUtils::drawRect(DrawUtils::addImVec2(wp, ImVec2(tileSize.x - 20, tileSize.y / 2 - (1.75f / 2) + 6)), ImVec2(15, 1.75f), ImColor(255, 255, 255, 255));
    }

    DrawUtils::drawRect(DrawUtils::addImVec2(wp, ImVec2(tileSize.x - 20, tileSize.y / 2 - (1.75f / 2))), ImVec2(15, 1.75f), ImColor(255, 255, 255, 255));

    if (v != 0)
    {
        for (size_t i = 0; i < modules.size(); i++)
        {
            ImGui::SetCursorPos(DrawUtils::addImVec2(wp, ImVec2(0, (tileSize.y * (i + 1)))));

            modules[i]->Draw(tileSize);
        }
    }

    ImGui::PopClipRect();

    #endif

    bool o = over(tileSize);

    move(o, tileSize);

    v += (ImGui::GetIO().DeltaTime * (isClosed ? -1 : 1)) / 0.2f;
    v = clampf(v, 0, 1);

    return o;
}
*/