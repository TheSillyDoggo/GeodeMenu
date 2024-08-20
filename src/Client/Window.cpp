#include "Client.h"
#include "Types/SetValueModule.hpp"

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