#include "Client.h"
#include "Types/SetValueModule.hpp"

void Window::drawImGui()
{
    ImGui::SetNextWindowPos(ImVec2(getPosition().x, getPosition().y));
    ImGui::SetNextWindowSize(getDesiredWindowSize());

    ImGui::Begin(this->name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            dragOffset = windowPos;
        }        

        setPosition(ccp(dragOffset.x + ImGui::GetMouseDragDelta().x, dragOffset.y + ImGui::GetMouseDragDelta().y));
        actualWindowPos = ImVec2(dragOffset.x + ImGui::GetMouseDragDelta().x, dragOffset.y + ImGui::GetMouseDragDelta().y);
    }

    for (auto module : modules)
    {
        ImGui::PushItemWidth(215);
        module->drawImGui();
    }

    closedTimer += (ImGui::GetIO().DeltaTime * (ImGui::IsWindowCollapsed() ? -1.0f : 1.0f)) / 0.5f;
    closedTimer = std::clamp<float>(closedTimer, 0, 1);
    ImGui::End();
}

ImVec2 Window::getDesiredWindowSize()
{
    return ImVec2(215, 25 * ((std::min<int>(modules.size(), 40) * closedTimer) + 1));
}

const CCPoint& Window::getPosition()
{
    return CCNode::getPosition();
    //return ccp(windowPos.x, windowPos.y);
}

void Window::setPosition(const CCPoint &position)
{
    CCNode::setPosition(position);

    windowPos = ImVec2(position.x, position.y);
}

CCPoint Window::offsetForTime(float time)
{
    CCPoint wndSize = CCDirector::get()->getWinSize();

    return ccp(0, wndSize.y * (1 - quadraticEaseInOut(time)));
}

float Window::quadraticEaseInOut(float t) {
    if (t < 0.5f)
        return 2 * t * t;
    else
        return (-2 * t * t) + (4 * t) - 1;
}

float Window::clampf(float v, float min, float max) {
    if (v < min)
        v = min;

    if (v > max)
        v = max;

    return v;
}

int Window::getIndex(std::vector<float> v, float K) { 
    auto it = std::find(v.begin(), v.end(), K); 

    // If element was found 
    if (it != v.end())  
    { 
    
        // calculating the index 
        // of K 
        int index = it - v.begin(); 
        return index;
    } 
    else { 
        // If the element is not 
        // present in the vector 
        return -1;
    } 
}

void Window::cocosCreate(CCMenu* menu)
{
    auto back = CCScale9Sprite::create("square02b_small.png");
    back->setContentSize(menu->getContentSize() / 0.5f);
    back->setPosition(ccp(0, 0));
    back->setAnchorPoint(ccp(0, 0));
    back->setScale(0.5f);
    back->setColor(ccc3(0, 0, 0));
    back->setOpacity(100);

    menu->addChild(back);

    int y = 0;

    float gap = 28;
    float extraGap = 9.69f;
    float height = gap * roundUpToMultipleOf2((modules.size() - 1) / 2);
    height += gap;

    height = std::max<float>(menu->getContentHeight(), height + extraGap);
    
    scroll = geode::ScrollLayer::create(menu->getContentSize());
    scroll->m_peekLimitTop = 15;
    scroll->m_peekLimitBottom = 15;
    menu->addChild(scroll);

    auto btnMenu = CCMenu::create();
    btnMenu->setContentSize(ccp(menu->getContentWidth(), height));
    btnMenu->setPosition(ccp(0, 0));
    btnMenu->setAnchorPoint(ccp(0, 0));
    scroll->m_contentLayer->addChild(btnMenu);

    int v = 0;

    for (size_t m = 0; m < modules.size(); m++)
    {
        float x = 20;

        if (!(v % 2 == 0))
            x = 188;

        if (modules[m])
            modules[m]->makeAndroid(btnMenu, {x, height - (gap * y) - (gap / 2) - (extraGap / 2)});

        if (dynamic_cast<SetValueModule*>(modules[m]))
        {
            y++;

            if (x == 20)
                v++;
        }
        else
        {
            if ((v - 1) % 2 == 0 && v != 0)
                y++;
        }

        v++;
    }

    scroll->m_contentLayer->setContentHeight(height);
    scroll->moveToTop();
}

#ifndef GEODE_IS_IOS

bool WindowMouseDispatcher::dispatchScrollMSG(float y, float x)
{
    for (auto window : Client::instance->windows)
    {
        if (window->scroll && nodeIsVisible(window->scroll))
            window->scroll->scrollLayer(y);
    }

    return CCMouseDispatcher::dispatchScrollMSG(y, x);
}

#endif