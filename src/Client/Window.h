#include "Module.h"

class Client;

class Window
{
public:
    std::string name;
    std::string id;
    bool excludeAndroid = false;
    std::vector<Module*> modules;
    ImVec2 windowPos = ImVec2(100, 100);

    bool dragging = false;
    ImVec2 offset = ImVec2(0, 0);

    bool draw(ImVec2 tileSize, float anim = 1)
    {
        ImVec2 wp = DrawUtils::addImVec2(windowPos, getOffsetForTime(anim));

        #ifndef GEODE_IS_ANDROID

        DrawUtils::drawRect(wp, tileSize, ColourUtility::GetColour(DrawUtils::mouseWithinRect(ImVec4(wp.x, wp.y, tileSize.x, tileSize.y)) ? (ImGui::IsMouseDown(ImGuiMouseButton_Left) ? ColourUtility::ClientColour::Pressed : ColourUtility::ClientColour::Hovered) : ColourUtility::ClientColour::WindowBG));
        DrawUtils::drawRect(DrawUtils::addImVec2(wp, ImVec2(0, tileSize.y)), ImVec2(tileSize.x, 3), ColourUtility::GetColour(ColourUtility::ClientColour::Accent));

        ImGui::SetCursorPos(DrawUtils::addImVec2(ImVec2(wp.x + (tileSize.x / 2) - (ImGui::CalcTextSize(name.c_str()).x / 2), wp.y + (tileSize.y / 2) - (ImGui::CalcTextSize(name.c_str()).y / 2)), ImVec2(1, 1)));
        ImGui::TextColored(ImVec4(0, 0, 0, 50.0f), name.c_str());

        ImGui::SetCursorPos(ImVec2(wp.x + (tileSize.x / 2) - (ImGui::CalcTextSize(name.c_str()).x / 2), wp.y + (tileSize.y / 2) - (ImGui::CalcTextSize(name.c_str()).y / 2)));
        ImGui::Text(name.c_str());

        for (size_t i = 0; i < modules.size(); i++)
        {
            ImGui::SetCursorPos(DrawUtils::addImVec2(wp, ImVec2(0, (tileSize.y * (i + 1)) + 3)));

            modules[i]->Draw(tileSize);
        }

        #endif

        bool o = over(tileSize);

        move(o, tileSize);

        return o;
    }

    void move(bool o, ImVec2 tileSize)
    {
        if (o)
        {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !dragging)
            {
                if (ImGui::GetIO().MousePos.y < windowPos.y + tileSize.y)
                {
                    dragging = true;

                    offset = ImVec2(windowPos.x - ImGui::GetIO().MousePos.x, windowPos.y - ImGui::GetIO().MousePos.y);
                }
            }
        }

        if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            dragging = false;
        }

        if (dragging)
        {
            windowPos = DrawUtils::addImVec2(ImGui::GetIO().MousePos, offset);
        }

        if (windowPos.x < 0)
            windowPos.x = 0;
        if (windowPos.y < 0)
            windowPos.y = 0;

        if (windowPos.x > ImGui::GetIO().DisplaySize.x - tileSize.x)
            windowPos.x = ImGui::GetIO().DisplaySize.x - tileSize.x;

        if (windowPos.y > ImGui::GetIO().DisplaySize.y - (tileSize.y * (modules.size() + 1) + 3))
            windowPos.y = ImGui::GetIO().DisplaySize.y - (tileSize.y * (modules.size() + 1) + 3);
    }

    bool over(ImVec2 tileSize)
    {
        auto mp = ImGui::GetIO().MousePos;

        if (mp.x > windowPos.x && mp.y > windowPos.y)
        {
            if (mp.x < windowPos.x + tileSize.x && mp.y < windowPos.y + (tileSize.y * (modules.size() + 1) + 3))
            {
                return true;
            }
        }

        return false;
    }

    int getIndex(std::vector<float> v, float K) 
    { 
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

    bool a = false;
    int i = 0;

    /// <summary>
    /// a time value of 1 is where we desire for the window to be at
    /// </summary>
    ImVec2 getOffsetForTime(float time)
    {
        ImVec2 wndSize = ImGui::GetIO().DisplaySize;
        auto v = std::vector<float>{};

        switch (DrawUtils::animationIn)
        {
            case DrawUtils::animType::SlideUp:
                return ImVec2(0, wndSize.y * (1 - time));

            case DrawUtils::animType::v6:
                return ImVec2(0, (wndSize.y * ((i % 2 == 0) ? 1 : -1)) * (1 - time));

            default:
                break;
        }

        return ImVec2(0, 0);
    }

    virtual void cocosCreate(CCMenu* menu)
    {
        auto back = CCScale9Sprite::create("square02_small.png");
        back->setContentSize(menu->getContentSize() / 0.5f);
        back->setPosition(ccp(0, 0));
        back->setAnchorPoint(ccp(0, 0));
        back->setScale(0.5f);
        back->setOpacity(100);

        menu->addChild(back);

        for (size_t m = 0; m < modules.size(); m++)
        {
            modules[m]->makeAndroid(menu, {20, (menu->getContentSize().height - 20 - 20) - 15 - (35.0f * (m - 1.0f))});
        }
    }
};

class Credits : public Window
{
    public:
        Credits()
        {
            name = "Credits";
            id = "credits-window";
        }

        void onName(CCObject* sender)
        {
            ProfilePage::create(7107344, false)->show();
        }

        void cocosCreate(CCMenu* menu)
        {
            auto back = CCScale9Sprite::create("square02_small.png");
            back->setContentSize(menu->getContentSize() / 0.5f);
            back->setPosition(ccp(0, 0));
            back->setAnchorPoint(ccp(0, 0));
            back->setScale(0.5f);
            back->setOpacity(100);

            auto credsLeft = CCLabelBMFont::create("Name:", "bigFont.fnt");
            credsLeft->setAnchorPoint(ccp(0, 1));
            credsLeft->setPosition(ccp(10, menu->getContentSize().height - 10));
            credsLeft->setScale(0.65f * 0.75f);

            auto btn = CCMenuItemSpriteExtra::create(CCLabelBMFont::create("PrometheusSV", "goldFont.fnt"), menu, menu_selector(Credits::onName));
            btn->setPositionY(credsLeft->getPositionY() - 5 - 2);
            btn->setPositionX(135);
            btn->setScale(0.75f);
            btn->m_baseScale = 0.75f;

            menu->addChild(back);
            menu->addChild(credsLeft);
            menu->addChild(btn);
        }
};