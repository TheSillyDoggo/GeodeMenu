#pragma once

#include <Geode/Geode.hpp>
#include "Module.h"
#include "Dropdown.h"
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "Replay.h"

#include "../Layers/SetupFadeSetting.h"
#include "../Layers/SetupFPSBypass.h"
#include "../Layers/SetupTransitionCustomizer.h"
#include "../Layers/SaveMacroPopup.h"
#include "../Layers/LoadMacroPopup.h"
#include "../Layers/EditStatusPositionLayer.h"
#include "../Layers/ConfirmFLAlertLayer.h"
#include "../Layers/ReplayOptionsLayer.h"

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

    bool isClosed = false;
    float v = 1.0f;

    float quadraticEaseInOut(float t) {
        if (t < 0.5f)
            return 2 * t * t;
        else
            return (-2 * t * t) + (4 * t) - 1;
    }

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

    bool mouseIn(ImVec4 pos)
    {
        auto mp = ImGui::GetIO().MousePos;

        if (mp.x > pos.x && mp.y > pos.y)
        {
            if (mp.x < pos.x + pos.z && mp.y < pos.y + pos.w)
            {
                return true;
            }
        }

        return false;
    }

    bool over(ImVec2 tileSize)
    {
        auto mp = ImGui::GetIO().MousePos;

        if (mp.x > windowPos.x && mp.y > windowPos.y)
        {
            if (mp.x < windowPos.x + tileSize.x && mp.y < windowPos.y + (tileSize.y * (modules.size() + 1)))
            {
                return true;
            }
        }

        return false;
    }

    //stolen from stack overflow
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

    int roundUpToMultipleOf2(float num) { // def not stolen
        //float roundedNum = std::ceil(num / 2.0f) * 2.0f;
        //return roundedNum;
        return static_cast<int>(std::ceil(num));
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

        int y = 0;

        float gap = 28;
        float extraGap = 9.69f;
        float height = gap * roundUpToMultipleOf2(modules.size() / 2);
        height += gap;

        height = std::max<float>(menu->getContentHeight(), height + extraGap);
        
        auto scroll = geode::ScrollLayer::create(menu->getContentSize());
        scroll->m_peekLimitTop = 15;
        scroll->m_peekLimitBottom = 15;
        menu->addChild(scroll);

        auto btnMenu = CCMenu::create();
        btnMenu->setContentSize(ccp(menu->getContentWidth(), height));
        btnMenu->setPosition(ccp(0, 0));
        btnMenu->setAnchorPoint(ccp(0, 0));
        scroll->m_contentLayer->addChild(btnMenu);

        for (size_t m = 0; m < modules.size(); m++)
        {
            float x = 20;

            if (!(m % 2 == 0))
                x = 188;

            modules[m]->makeAndroid(btnMenu, {x, height - (gap * y) - (gap / 2) - (extraGap / 2)});

            if ((m - 1) % 2 == 0 && m != 0)
                y++;
        }

        scroll->m_contentLayer->setContentHeight(height);
        scroll->moveToTop();
        scroll->enableScrollWheel();
    }
};

class Speedhack : public Window//, public TextInputDelegate
{
    public:
        Speedhack()
        {
            name = "Speedhack";
            id = "speedhack-window";
        }

        static inline Slider* slider = nullptr;
        std::vector<float> presets = 
        {
            0.1f, 0.25f, 0.3f, 0.50f, 0.75f, 1, 1.50f, 2
        };

        void clear(CCObject* sender)
        {
            SpeedhackTop::instance->text = "";
            auto inp = getChildOfType<TextInput>(static_cast<CCNode*>(sender)->getParent(), 0);
            inp->setString("");
            slider->setValue(unscaleValue(1));
        }

        float scaleValue(float originalValue) {
            float minValue = 0.1;
            float maxValue = 3.0;
            float scaledValue = (maxValue - minValue) * originalValue + minValue;
            return scaledValue;
        }

        float unscaleValue(float scaledValue) {
            float minValue = 0.1;
            float maxValue = 3.0;
            float originalValue = (scaledValue - minValue) / (maxValue - minValue);
            return originalValue;
        }

        float clampf(float v, float min, float max)
        {
            if (v < min)
                v = min;

            if (v > max)
                v = max;

            return v;
        }

        void sliderChanged(CCObject* sender)
        {
            float v = ((slider->getThumb()->getPositionX() + 100) / 200.0f);

            std::stringstream ss;
            ss << round(scaleValue(v) * 100.0) / 100.0;

            auto inp = getChildOfType<TextInput>(static_cast<CCNode*>(sender)->getParent()->getParent()->getParent(), 0);
            inp->setString(ss.str().c_str());
            SpeedhackTop::instance->text = ss.str();

            SpeedhackTop::instance->save();
        }

        void onDebug(CCObject*)
        {
            float v = 1.0f;

            auto x = numFromString<float>(SpeedhackTop::instance->text);

            if (x.isOk())
            {
                v = x.value();
            }

            if (v < 0.01f)
                v = 0.01f;

            if (v > 99999)
                v = 99999;

            bool m = SpeedhackMus::instance->enabled;

            if (SpeedhackGameplay::instance->enabled)
                if (!(PlayLayer::get() || GameManager::sharedState()->getEditorLayer())) { v = 1.0f; }

            v /= CCDirector::get()->getScheduler()->getTimeScale();

            std::stringstream ss;

            ss << "String: <ca>";
            ss << SpeedhackTop::instance->text;
            ss << "</c>\n";

            ss << "Value: <ca>";
            ss << v;
            ss << "</c>\n";
            
            ss << "Value UN-TS: <cy>";
            ss << v * CCDirector::get()->getScheduler()->getTimeScale();
            ss << "</c>\n";
            
            ss << "isOk: <cg>";
            ss << x.isOk();
            ss << "</c>\n";
            
            ss << "isErr: <cb>";
            ss << x.isErr();
            ss << "</c>\n";

            if (x.isErr())
            {
                ss << "Error: <cp>";
                ss << x.err().value();
                ss << "</c>\n";
            }

            ss << "Has Value: <cl>";
            ss << x.has_value();
            ss << "</c>\n";

            //ss << "Value RES: <cr>";
            //ss << x.value();
            //ss << "</c>\n";

            FLAlertLayer::create("Macro Info", ss.str(), "OK")->show();
        }

        void onPreset(CCObject* sender)
        {
            float value = numFromString<float>(as<CCNode*>(sender)->getID(), 2).value();

            SpeedhackTop::instance->text = as<CCNode*>(sender)->getID();
            auto inp = getChildOfType<TextInput>(static_cast<CCNode*>(sender)->getParent()->getParent(), 0);
            inp->setString(as<CCNode*>(sender)->getID());
            slider->setValue(unscaleValue(value));
        }

        void cocosCreate(CCMenu* menu)
        {
            float v = 1.0f;

            auto x = numFromString<float>(SpeedhackTop::instance->text);

            if (x.isOk())
            {
                v = x.value();
            }

            if (v < 0.01f)
                v = 0.01f;

            if (v > 99999)
                v = 99999;            
            
            auto back = CCScale9Sprite::create("square02_small.png");
            back->setContentSize(menu->getContentSize() / 0.5f);
            back->setPosition(ccp(0, 0));
            back->setAnchorPoint(ccp(0, 0));
            back->setScale(0.5f);
            back->setOpacity(100);
            menu->addChild(back);

            slider = Slider::create(menu, menu_selector(Speedhack::sliderChanged));
            slider->setPosition(ccp(menu->getContentSize().width / 2, menu->getContentSize().height - 80));
            slider->setScale(0.875f);
            slider->setContentSize(ccp(0, 0));
            slider->ignoreAnchorPointForPosition(false);
            slider->setValue(clampf(unscaleValue(v), 0, 1));
            menu->addChild(slider, 2);

            modules[0]->makeAndroid(menu, ccp(menu->getContentSize().width / 2, menu->getContentSize().height - 50) - ccp(180 / 2, 0) + ccp(10, 0));

            for (size_t i = 1; i < modules.size(); i++)
            {
                modules[i]->makeAndroid(menu, (ccp(menu->getContentSize().width / 2, menu->getContentSize().height - 110 - (30 * i)) - ccp(180 / 2, 0) + ccp(20, 0)) + ccp(0, 27.5f));
            }

            //static_cast<geode::InputNode*>(menu->getChildByID("speedhack-top"))->getInput()->setDelegate(this);

            auto trash = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png"), menu, menu_selector(Speedhack::clear));
            trash->m_baseScale = 0.725f;
            trash->setScale(0.725f);
            trash->setPosition(ccp((menu->getContentSize().width / 2) + (180 / 2) + 20, menu->getContentSize().height - 50));
            menu->addChild(trash);

            auto presetMenu = CCMenu::create();
            presetMenu->setScale(0.41f);
            presetMenu->setAnchorPoint(ccp(0.5f, 0.5f));
            presetMenu->setPosition(menu->getContentWidth() / 2, 13);
            presetMenu->setContentWidth(6969);
            presetMenu->setLayout(RowLayout::create()->setGap(15)->setAutoScale(false));

            for (auto preset : presets)
            {
                auto btn = ButtonSprite::create(utils::numToString(preset, 2).c_str(), "bigFont.fnt", "GJ_button_05.png");
                auto act = CCMenuItemSpriteExtra::create(btn, menu, menu_selector(Speedhack::onPreset));
                act->setID(numToString(preset, 2));

                presetMenu->addChild(act);
            }           

            presetMenu->updateLayout();
            menu->addChild(presetMenu);

            return;

            auto pos = ccp(menu->getContentSize().width, 0) + ccp(-58, 22) * 0.5f;

            auto btnS = ButtonSprite::create("Debug", 90, false, "bigFont.fnt", "GJ_button_05.png", 35, 0.75f);
            //as<CCNode*>(btnS->getChildren()->objectAtIndex(0))->setScale(0.375f);
            as<CCLabelBMFont*>(btnS->getChildren()->objectAtIndex(0))->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
            as<CCLabelBMFont*>(btnS->getChildren()->objectAtIndex(0))->updateLabel();
            auto btn = CCMenuItemSpriteExtra::create(btnS, menu, menu_selector(Speedhack::onDebug));
            btn->m_baseScale = 0.5f;
            btn->setScale(btn->m_baseScale);
            btn->setSizeMult(1.15f);
            btn->setPosition(pos);
            menu->addChild(btn);
        }

        virtual void textChanged(CCTextInputNode* p0)
        {
            SpeedhackTop::instance->text = p0->getString();

            float v = 1.0f;

            if (SpeedhackTop::instance->text.size() != 0 && !SpeedhackTop::instance->text.ends_with("."))
            {
                v = std::stof(SpeedhackTop::instance->text);
            }

            if (v < 0.01f)
                v = 0.01f;

            if (v > 99999)
                v = 99999;

            slider->setValue(clampf(unscaleValue(v), 0, 1));

            modules[0]->save();
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

        void onCredit(CCObject* sender)
        {
            auto tag = static_cast<CCNode*>(sender)->getID();

            if (tag.starts_with("gd"))
            {
                log::info("open gd profile: {}", std::stoi(tag.c_str() + 2));
                ProfilePage::create(std::stoi(tag.c_str() + 2), false)->show();
            }
            else
            {
                CCApplication::sharedApplication()->openURL(tag.c_str());
            }
        }

        void createPanel(CCMenu* menu, CCPoint pos, char const * title, char const * name, CCNode* img, float scale, std::string v)
        {
            auto t = CCLabelBMFont::create(title, "bigFont.fnt");
            t->setPosition(pos + ccp(0, 28));
            t->limitLabelWidth(110, 0.5f, 0.1f);
            menu->addChild(t);

            auto n = CCLabelBMFont::create(name, "goldFont.fnt");
            menu->addChild(n);
            n->limitLabelWidth(110, 0.5f, 0.1f);
            n->setPosition(pos + ccp(0, -28));

            auto btn = CCMenuItemSpriteExtra::create(img, menu, menu_selector(Credits::onCredit));
            btn->setID(v);
            btn->setPosition(pos);
            btn->setContentSize(ccp(34, 34));
            menu->addChild(btn);

            img->setPosition(btn->getContentSize() / 2);
            img->setScale(scale);
        }

        void cocosCreate(CCMenu* menu)
        {
            auto back = CCScale9Sprite::create("square02_small.png");
            back->setContentSize(menu->getContentSize() / 0.5f);
            back->setPosition(ccp(0, 0));
            back->setAnchorPoint(ccp(0, 0));
            back->setScale(0.5f);
            back->setOpacity(100);
            menu->addChild(back);

            auto promImg = SimplePlayer::create(233);
            promImg->setColor({255, 75, 0});
            promImg->setSecondColor({255, 185, 0});
            promImg->setGlowOutline({255, 185, 0});
            promImg->updateColors();

            createPanel(menu, ccp(55, menu->getContentSize().height - 45), "Name", "PrometheusSV", promImg, 1.0f, "gd7107344");


            createPanel(menu, ccp(55 + (230 / 2) * 1, menu->getContentSize().height - 45), "Pastel", "Absolllute", CCSprite::create("absolllute.png"_spr), 0.45f * CCDirector::get()->getContentScaleFactor() * 0.5f * 0.5f, "https://gist.github.com/absoIute/657a4c95bb92755f96e20772adbf5f32");


            auto fireeImg = SimplePlayer::create(98);
            fireeImg->setColor({125, 0, 255});
            fireeImg->setSecondColor({255, 255, 255});
            fireeImg->setGlowOutline({255, 255, 255});
            fireeImg->updateColors();

            createPanel(menu, ccp(55 + (230 / 2) * 2, menu->getContentSize().height - 45), "Copy Hack", "FireeDev", fireeImg, 1.0f, "gd6253758");


            auto catImg = SimplePlayer::create(98);
            catImg->setColor({255, 0, 0});
            catImg->setSecondColor({255, 255, 255});
            catImg->setGlowOutline({0, 255, 255});
            catImg->updateColors();

            createPanel(menu, ccp(55 + (230 / 2) * 0, menu->getContentSize().height - 45 - (85 * 1)), "", "CatXus", catImg, 1.0f, "gd14467409");


            auto kmImg = SimplePlayer::create(233);
            kmImg->setColor({255, 125, 0});
            kmImg->setSecondColor({0, 125, 255});
            kmImg->setGlowOutline({255, 255, 255});
            kmImg->updateColors();

            createPanel(menu, ccp(55 + (230 / 2) * 1, menu->getContentSize().height - 45 - (85 * 1)), "Beta Testers", "km7dev", kmImg, 1.0f, "gd7236822");


            auto mkrImg = SimplePlayer::create(242);
            mkrImg->setColor({125, 0, 255});
            mkrImg->setSecondColor({185, 0, 255});
            mkrImg->updateColors();

            createPanel(menu, ccp(55 + (230 / 2) * 2, menu->getContentSize().height - 45 - (85 * 1)), "", "MrMkr", mkrImg, 1.0f, "gd21449475");


            auto jaidImg = SimplePlayer::create(373);
            jaidImg->setColor({90, 90, 90});
            jaidImg->setSecondColor({182, 0, 255});
            jaidImg->updateColors();

            createPanel(menu, ccp(55 + (230 / 2) * 0, menu->getContentSize().height - 45 - (85 * 2)), "", "Jaid", jaidImg, 1.0f, "gd7669473");


            auto zephImg = SimplePlayer::create(482);
            zephImg->setColor({77, 77, 90});
            zephImg->setSecondColor({0, 200, 255});
            zephImg->setGlowOutline({0, 200, 255});
            zephImg->updateColors();

            createPanel(menu, ccp(55 + (230 / 2) * 1, menu->getContentSize().height - 45 - (85 * 2)), "", "imzeph", zephImg, 1.0f, "gd23948408");


            auto devImg = SimplePlayer::create(5);
            devImg->setColor({125, 125, 255});
            devImg->setSecondColor({125, 255, 175});
            devImg->setGlowOutline({255, 255, 255});
            devImg->updateColors();

            createPanel(menu, ccp(55 + (230 / 2) * 2, menu->getContentSize().height - 45 - (85 * 2)), "Developer", "TheSillyDoggo", devImg, 1.0f, "gd16778880");
        }
};

class Variables : public Window
{
    public:
        Variables()
        {
            name = "Variables";
            id = "vars-window";
        }

        void onChangeMode(CCObject* sender)
        {
            log::info("button");

            //PlayLayer::get()->m_player1->toggleDartMode(true, true);
            PlayLayer::get()->m_player1->m_position += ccp(30, 150);

            //PlayLayer::get()->m_player1->playerDestroyed(false);

            //PlayLayer::get()->destroyPlayer(PlayLayer::get()->m_player1, GameObject::createWithKey(0));
            /*
            //why robert

            bool m_isShip;
            bool m_isBall;
            bool m_isBird;
            bool m_isDart;
            bool m_isRobot;
            bool m_isSpider;
            bool m_isSwing;
            */

            //if (!PlayLayer::get() || !PlayLayer::get()->m_player1)
                //return;

            //auto plr = PlayLayer::get()->m_player1;

            //int selMode = 5;

            //plr->m_isPlatformer = true;

            //plr->switchedToMode(GameObjectType::BallPortal);

            //plr->toggleRollMode(true, true);
            //plr->m_isBall = selMode == 2;
            //plr->m_isBird = selMode == 3;
            //plr->m_isDart = selMode == 4;
            //plr->m_isRobot = selMode == 5;
            //plr->m_isSpider = selMode == 6;
            //plr->m_isSwing = selMode == 7;

            //plr->updatePlayerFrame();
            //plr->updatePlayerShipFrame();
        }

        void cocosCreate(CCMenu* menu)
        {
            auto back = CCScale9Sprite::create("square02_small.png");
            back->setContentSize(ccp(110, menu->getContentSize().height) / 0.5f);
            back->setPosition(ccp(0, 0));
            back->setAnchorPoint(ccp(0, 0));
            back->setScale(0.5f);
            back->setOpacity(100);

            auto btn = CCMenuItemSpriteExtra::create(back, menu, menu_selector(Variables::onChangeMode));
            menu->addChild(btn);
        }
};

class Uncomplete : public FLAlertLayerProtocol
{
    public:
        
        virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1)
        {
            if (p1)
            {
                log::info("uncomplete");

                GJGameLevel* level = nullptr;

                if (PlayLayer::get())
                    level = PlayLayer::get()->m_level;

                if (getChildOfType<LevelInfoLayer>(CCScene::get(), 0))
                    level = getChildOfType<LevelInfoLayer>(CCScene::get(), 0)->m_level;

                if (!level)
                    return;

                level->m_normalPercent = 0;
                level->m_practicePercent = 0;
                level->m_attempts = 0;
                level->m_jumps = 0;
            }
        }
        
        void onToggle(CCObject* sender)
        {
            auto btn = as<CCMenuItemToggler*>(sender);

            log::info("dont show: {}", !btn->isToggled());

            Mod::get()->setSavedValue<bool>("uncomplete-dont-show", !btn->isToggled());
        }
};

class Level : public Window
{
    public:
        void onFix(CCObject*)
        {
            if (PlayLayer::get())
            {
                PlayLayer::get()->m_level->m_levelType = GJLevelType::Saved;

                // fucking disgusting
                if ((PlayLayer::get()->m_level->m_levelID > 0 && PlayLayer::get()->m_level->m_levelID < 127) || PlayLayer::get()->m_level->m_levelID == 3001 || PlayLayer::get()->m_level->m_levelID == 5001 || PlayLayer::get()->m_level->m_levelID == 5002 || PlayLayer::get()->m_level->m_levelID == 5003 || PlayLayer::get()->m_level->m_levelID == 5004)
                {
                    PlayLayer::get()->m_level->m_levelType = GJLevelType::Local;
                }

                if (PlayLayer::get()->m_level->m_levelID == 0)
                    PlayLayer::get()->m_level->m_levelType = GJLevelType::Editor;
            }
        }

        void onFPS(CCObject*)
        {
            SetupFPSBypass::addToScene();
        }

        void onUnc(CCObject* sender)
        {
            if (!(PlayLayer::get() || getChildOfType<LevelInfoLayer>(CCScene::get(), 0)))
                return FLAlertLayer::create("Uncomplete Level", "You must be in a level or on a level page to uncomplete a level", "OK")->show();
            
            auto protocol = new Uncomplete();

            if (Mod::get()->getSavedValue<bool>("uncomplete-dont-show", false) && !CCKeyboardDispatcher::get()->getShiftKeyPressed())
            {
                protocol->FLAlert_Clicked(nullptr, true);
            }
            else
            {
                auto alert = ConfirmFLAlertLayer::create(protocol, "Uncomplete Level", "Don't show this popup again", false, menu_selector(Uncomplete::onToggle), "Are you sure you want to <cr>uncomplete</c> this <cg>level</c>? Uncompleting a level will:\n<cl>- Reset Attempts</c>\n<cl>- Reset Percentage</c>\n<cl>- Reset Jumps</c>", "Cancel", "Reset", 350, false, 310, 1.0f);
                alert->show();
            }
        }

        void cocosCreate(CCMenu* menu)
        {
            Window::cocosCreate(menu);

            /*auto btnS = ButtonSprite::create("FPS\nBypass", 60, false, "bigFont.fnt", "GJ_button_05.png", 35, 0.75f);
            as<CCNode*>(btnS->getChildren()->objectAtIndex(0))->setScale(0.375f);
            as<CCLabelBMFont*>(btnS->getChildren()->objectAtIndex(0))->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
            as<CCLabelBMFont*>(btnS->getChildren()->objectAtIndex(0))->updateLabel();
            auto btn = CCMenuItemSpriteExtra::create(btnS, menu, menu_selector(Level::onFPS));
            btn->setSizeMult(1.15f);
            btn->setPosition(pos);
            menu->addChild(btn);*/
        }
};

class Universal : public Window
{
    public:
        void onTransCustomizer(CCObject*)
        {
            SetupTransCustom::addToScene();
        }

        void cocosCreate(CCMenu* menu)
        {
            Window::cocosCreate(menu);

            auto pos = ccp(menu->getContentSize().width, 0) + ccp(-58, 22);

            auto btnS = ButtonSprite::create("Transition\nCustomizer", 90, false, "bigFont.fnt", "GJ_button_05.png", 35, 0.75f);
            as<CCNode*>(btnS->getChildren()->objectAtIndex(0))->setScale(0.375f);
            as<CCLabelBMFont*>(btnS->getChildren()->objectAtIndex(0))->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
            as<CCLabelBMFont*>(btnS->getChildren()->objectAtIndex(0))->updateLabel();
            auto btn = CCMenuItemSpriteExtra::create(btnS, menu, menu_selector(Universal::onTransCustomizer));
            btn->setSizeMult(1.15f);
            btn->setPosition(pos);
            menu->addChild(btn);


            pos = ccp(menu->getContentSize().width, 0) + ccp(-28 - 18 - 2.5f, 22);
            pos += ccp(-110, 0);

            auto btnUnc = ButtonSprite::create("Uncomplete\nLevel", 70, false, "bigFont.fnt", "GJ_button_05.png", 35, 0.75f);
            
            as<CCNode*>(btnUnc->getChildren()->objectAtIndex(0))->setScale(0.375f);
            as<CCLabelBMFont*>(btnUnc->getChildren()->objectAtIndex(0))->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
            as<CCLabelBMFont*>(btnUnc->getChildren()->objectAtIndex(0))->updateLabel();

            btn = CCMenuItemSpriteExtra::create(btnUnc, menu, menu_selector(Level::onUnc));
            btn->setSizeMult(1.15f);
            btn->setPosition(pos);
            menu->addChild(btn);

            pos = pos + ccp(-30 - 27 - 2.5f, -5);

            auto btnS2 = ButtonSprite::create("?", 10, false, "bigFont.fnt", "GJ_button_05.png", 25, 0.75f);
            as<CCNode*>(btnS2->getChildren()->objectAtIndex(0))->setScale(0.375f);
            as<CCLabelBMFont*>(btnS2->getChildren()->objectAtIndex(0))->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
            as<CCLabelBMFont*>(btnS2->getChildren()->objectAtIndex(0))->updateLabel();
            auto btn2 = CCMenuItemSpriteExtra::create(btnS2, menu, menu_selector(Level::onFix));
            btn2->setSizeMult(1.15f);
            btn2->setPosition(pos);
            menu->addChild(btn2);
        }
};

class _Replay : public Window
{
    public:
        static inline CCMenuItemToggler* btnRec = nullptr;
        static inline CCMenuItemToggler* btnPlay = nullptr;
        static inline CCLabelBMFont* lbl = nullptr;

        void onSave(CCObject*)
        {
            SaveMacroPopup::addToScene();
        }

        void onLoad(CCObject*)
        {
            LoadMacroPopup::addToScene();
        }

        void onClear(CCObject*)
        {
            auto pop = geode::createQuickPopup(
                "Clear Replay",
                "Are you sure you want to <cr>clear</c> this replay?\nIf you haven't saved the <cg>replay</c>\n it will be <cr>DELETED</c>.",
                "Cancel", "Clear",
                [this](FLAlertLayer* tis, bool btn2) {
                    log::info("click clear");

                    if (btn2) {
                        log::info("right btn");

                        GJReplayManager::replay = MyReplay();

                        CCScene::get()->addChild(TextAlertPopup::create("Cleared Replay", 0.5f, 0.6f, 150, ""), 9999999);
                    }
                }
            );

            pop->m_button2->updateBGImage("GJ_button_06.png");
        }

        void onRecord(CCObject*)
        {
            GJReplayManager::playing = false;
            GJReplayManager::recording = !GJReplayManager::recording;

            //btnRec->toggle(GJReplayManager::recording);
            btnPlay->toggle(GJReplayManager::playing);
        }

        void onPlay(CCObject*)
        {
            GJReplayManager::recording = false;
            GJReplayManager::playing = !GJReplayManager::playing;

            btnRec->toggle(GJReplayManager::recording);
            //btnPlay->toggle(GJReplayManager::playing);
        }

        void onManage(CCObject*)
        {
            FLAlertLayer::create("Macro Manager", "Macro Manager is not added yet as I wanted to get the update out as fast as I could, hope you understand <cl>^w^</c>", "OK")->show();
        }

        void onConfig(CCObject*)
        {
            ReplayOptionsLayer::addToScene();
        }

        void cocosCreate(CCMenu* menu)
        {
            auto back = CCScale9Sprite::create("square02_small.png");
            back->setContentSize(menu->getContentSize() / 0.5f);
            back->setPosition(ccp(0, 0));
            back->setAnchorPoint(ccp(0, 0));
            back->setScale(0.5f);
            back->setOpacity(100);
            menu->addChild(back);

            lbl = CCLabelBMFont::create("Status", "bigFont.fnt");
            lbl->setPosition(ccp(5, menu->getContentSize().height - 2));
            lbl->setScale(0.725f);
            lbl->setAnchorPoint(ccp(0, 1));
            lbl->setOpacity(100);

            auto menuRow = CCMenu::create();
            menuRow->ignoreAnchorPointForPosition(false);
            menuRow->setContentSize(ccp(9999, 0));
            menuRow->setScale(0.625f);
            menuRow->setPosition(menu->getContentSize() / 2 + ccp(0, -30) + ccp(0, -25));

            auto btnRecSpr1 = ButtonSprite::create("Record", 105, false, "bigFont.fnt", "GJ_button_05.png", 40, 0.65f);
            getChildOfType<CCLabelBMFont>(btnRecSpr1, 0)->setOpacity(100);
            getChildOfType<CCLabelBMFont>(btnRecSpr1, 0)->setPositionX(105 / 2);
            getChildOfType<CCScale9Sprite>(btnRecSpr1, 0)->setPositionX(105 / 2);
            
            btnRecSpr1->setContentWidth(105);
            getChildOfType<CCScale9Sprite>(btnRecSpr1, 0)->setContentWidth(105);
            
            auto btnRecSpr2 = ButtonSprite::create("Recording", 105, false, "bigFont.fnt", "GJ_button_01.png", 40, 0.5f);
            btnRecSpr2->setContentWidth(105);
            getChildOfType<CCScale9Sprite>(btnRecSpr2, 0)->setContentWidth(105);
            getChildOfType<CCLabelBMFont>(btnRecSpr2, 0)->setPositionX(105 / 2);
            getChildOfType<CCScale9Sprite>(btnRecSpr2, 0)->setPositionX(105 / 2);

            btnRec = CCMenuItemToggler::create(btnRecSpr1, btnRecSpr2, menu, menu_selector(_Replay::onRecord));
            btnRec->toggle(GJReplayManager::recording);

            auto btnPlaySpr1 = ButtonSprite::create("Play", 105, false, "bigFont.fnt", "GJ_button_05.png", 40, 0.65f);
            getChildOfType<CCLabelBMFont>(btnPlaySpr1, 0)->setOpacity(100);
            getChildOfType<CCLabelBMFont>(btnPlaySpr1, 0)->setPositionX(105 / 2);
            getChildOfType<CCScale9Sprite>(btnPlaySpr1, 0)->setPositionX(105 / 2);

            btnPlaySpr1->setContentWidth(105);
            getChildOfType<CCScale9Sprite>(btnPlaySpr1, 0)->setContentWidth(105);

            auto btnPlaySpr2 = ButtonSprite::create("Playing", 105, false, "bigFont.fnt", "GJ_button_01.png", 40, 0.5f);
            btnPlaySpr2->setContentWidth(105);
            getChildOfType<CCScale9Sprite>(btnPlaySpr2, 0)->setContentWidth(105);
            getChildOfType<CCLabelBMFont>(btnPlaySpr2, 0)->setPositionX(105 / 2);
            getChildOfType<CCScale9Sprite>(btnPlaySpr2, 0)->setPositionX(105 / 2);

            btnPlay = CCMenuItemToggler::create(btnPlaySpr1, btnPlaySpr2, menu, menu_selector(_Replay::onPlay));
            btnPlay->toggle(GJReplayManager::playing);

            menuRow->addChild(CCMenuItemSpriteExtra::create(ButtonSprite::create("Save", "bigFont.fnt", "GJ_button_04.png"), menu, menu_selector(_Replay::onSave)));
            menuRow->addChild(CCMenuItemSpriteExtra::create(ButtonSprite::create("Load", "bigFont.fnt", "GJ_button_04.png"), menu, menu_selector(_Replay::onLoad)));
            menuRow->addChild(CCMenuItemSpriteExtra::create(ButtonSprite::create("Clear", "bigFont.fnt", "GJ_button_06.png"), menu, menu_selector(_Replay::onClear)));
            //menuRow->addChild(CCMenuItemSpriteExtra::create(ButtonSprite::create("More", "bigFont.fnt", "GJ_button_04.png"), menu, nullptr));

            menuRow->setLayout(RowLayout::create()->setAutoScale(false)->setGap(55));

            auto pos = ccp(menu->getContentSize().width, 0) + ccp(-55, 22);

            auto options = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png"), menu, menu_selector(_Replay::onConfig));
            options->m_baseScale = 0.6f;
            options->setScale(options->m_baseScale);
            menu->addChildAtPosition(options, Anchor::TopRight, ccp(-18, -18));

            auto manager = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("folderIcon_001.png"), menu, menu_selector(_Replay::onManage));

            //menu->addChild(lbl);
            menu->addChild(menuRow);
            menu->addChildAtPosition(btnRec, Anchor::Center, ccp(-70, 0));
            menu->addChildAtPosition(btnPlay, Anchor::Center, ccp(70, 0));
            menu->addChildAtPosition(manager, Anchor::BottomLeft, ccp(25, 23));
        }
};

class Status : public Window
{
    public:
        void onEditPositions(CCObject*)
        {
            EditStatusPositionLayer::addToScene();
        }

        void cocosCreate(CCMenu* menu)
        {
            Window::cocosCreate(menu);

            return;

            auto pos = ccp(58, 22);

            auto btnS = ButtonSprite::create("Edit\nPositions", 90, false, "bigFont.fnt", "GJ_button_05.png", 35, 0.75f);
            as<CCNode*>(btnS->getChildren()->objectAtIndex(0))->setScale(0.375f);
            as<CCLabelBMFont*>(btnS->getChildren()->objectAtIndex(0))->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
            as<CCLabelBMFont*>(btnS->getChildren()->objectAtIndex(0))->updateLabel();
            auto btn = CCMenuItemSpriteExtra::create(btnS, menu, menu_selector(Status::onEditPositions));
            btn->setSizeMult(1.15f);
            btn->setPosition(pos);
            menu->addChild(btn);
        }
};


class EffectUI : public CCNode
{
    public:
        std::vector<SimplePlayer*> players;
        std::vector<SimplePlayer*> players2;
        std::vector<CCSprite*> chromas;
        std::vector<CCSprite*> pastels;
        std::vector<CCSprite*> fades;

        bool init()
        {
            if (!CCNode::init())
                return false;

            this->scheduleUpdate();

            return true;
        }

        static EffectUI* create() {
            EffectUI* ret = new EffectUI();
            if (ret && ret->init()) {
                ret->autorelease();
                return ret;
            } else {
                delete ret;
                ret = nullptr;
                return nullptr;
            }
        }

        static ccColor3B getColourForSelected(int mode) // bri`ish
        {
            // 0 : primary, 1 : secondary : 2 : glow

            std::stringstream ss;
            ss << "selColour";
            ss << mode;

            int sel = Mod::get()->getSavedValue<int>(ss.str(), 0);

            if (mode == 0)
            {
                if (sel == 0)
                    return GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value());
            }
            else if (mode == 1)
            {
                if (sel == 0)
                    return GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value());
            }
            else
            {
                if (sel == 0)
                    return GameManager::get()->colorForIdx(GameManager::get()->m_playerGlowColor.value());
            }

            if (sel == 1)
                return ColourUtility::getChromaColour();

            if (sel == 2)
                return ColourUtility::getPastelColour();

            if (sel == 3)
            {
                std::stringstream fadeIn;
                fadeIn << "fadeColour1";
                fadeIn << mode;

                std::stringstream fadeOut;
                fadeOut << "fadeColour2";
                fadeOut << mode;

                ccColor3B in = Mod::get()->getSavedValue<ccColor3B>(fadeIn.str(), {0, 0, 0});
                ccColor3B out = Mod::get()->getSavedValue<ccColor3B>(fadeOut.str(), {255, 255, 255});

                return ColourUtility::lerpColour(in, out, (sinf(ColourUtility::va * 3) + 1) / 2);
                //fade
            }

            if (sel == 4)
            {
                //custom colour
            }

            return {0, 0, 0};
        }

        void update(float delta) {
            for (size_t i = 0; i < players.size(); i++)
            {
                players[i]->setColor(getColourForSelected(0));
                players[i]->setSecondColor(getColourForSelected(1));

                players[i]->enableCustomGlowColor(getColourForSelected(2));
                players[i]->m_hasGlowOutline = GameManager::get()->m_playerGlow;
                players[i]->updateColors();
            }

            for (size_t i = 0; i < players2.size(); i++)
            {
                if (Mod::get()->getSavedValue<bool>("same-dual"))
                {
                    players2[i]->setColor(getColourForSelected(0));
                    players2[i]->setSecondColor(getColourForSelected(1));
                }
                else
                {
                    players2[i]->setColor(getColourForSelected(1));
                    players2[i]->setSecondColor(getColourForSelected(0));
                }

                players2[i]->enableCustomGlowColor(getColourForSelected(2));
                players2[i]->m_hasGlowOutline = GameManager::get()->m_playerGlow;
                players2[i]->updateColors();
            }
            
            for (size_t i = 0; i < chromas.size(); i++)
            {
                chromas[i]->setColor(ColourUtility::getChromaColour());
            }

            for (size_t i = 0; i < chromas.size(); i++)
            {
                pastels[i]->setColor(ColourUtility::getPastelColour());
            }

            for (size_t i = 0; i < fades.size(); i++)
            {
                std::stringstream fadeIn;
                fadeIn << "fadeColour1";
                fadeIn << i;

                std::stringstream fadeOut;
                fadeOut << "fadeColour2";
                fadeOut << i;

                ccColor3B in = Mod::get()->getSavedValue<ccColor3B>(fadeIn.str(), {0, 0, 0});
                ccColor3B out = Mod::get()->getSavedValue<ccColor3B>(fadeOut.str(), {255, 255, 255});

                fades[i]->setColor(ColourUtility::lerpColour(in, out, (sinf(ColourUtility::va * 3) + 1) / 2));
            }
        }
};

class $modify (GJBaseGameLayer)
{
    virtual void update(float p0)
    {
        GJBaseGameLayer::update(p0);

        if (m_player1)
        {
            m_player1->setColor(EffectUI::getColourForSelected(0));
            m_player1->setSecondColor(EffectUI::getColourForSelected(1));
            m_player1->m_glowColor = EffectUI::getColourForSelected(2);
            m_player1->updateGlowColor();
        }

        if (m_player2)
        {
            if (!Mod::get()->getSavedValue<bool>("same-dual"))
            {
                m_player2->setColor(EffectUI::getColourForSelected(1));
                m_player2->setSecondColor(EffectUI::getColourForSelected(0));
                m_player2->m_glowColor = EffectUI::getColourForSelected(2);
                m_player2->updateGlowColor();
            }
            else
            {
                m_player2->setColor(EffectUI::getColourForSelected(0));
                m_player2->setSecondColor(EffectUI::getColourForSelected(1));
                m_player2->m_glowColor = EffectUI::getColourForSelected(2);
                m_player2->updateGlowColor();
            }
        }
    }
};

class IconEffects : public Window
{
    public:
        IconEffects()
        {
            name = "Icon Effects";
            id = "icon-effects";
        }

        EffectUI* ui = nullptr;        

        SimplePlayer* getPlayer(IconType type, EffectUI* ui, bool second = false)
        {
            int id = GameManager::get()->m_playerFrame.value();

            if (type == IconType::Ship)
                id = GameManager::get()->m_playerShip.value();
            if (type == IconType::Ball)
                id = GameManager::get()->m_playerBall.value();
            if (type == IconType::Ufo)
                id = GameManager::get()->m_playerBird.value();
            if (type == IconType::Wave)
                id = GameManager::get()->m_playerDart.value();
            if (type == IconType::Robot)
                id = GameManager::get()->m_playerRobot.value();
            if (type == IconType::Spider)
                id = GameManager::get()->m_playerSpider.value();
            if (type == IconType::Swing)
                id = GameManager::get()->m_playerSwing.value();
            if (type == IconType::Jetpack)
                id = GameManager::get()->m_playerJetpack.value();


            auto plr = SimplePlayer::create(id);
            plr->updatePlayerFrame(id, type);
            plr->setScale(1.4f);
            if (second)
                ui->players2.push_back(plr);
            else
                ui->players.push_back(plr);

            return plr;
        }

        void onFadeSettings(CCObject* sender)
        {
            CCScene::get()->addChild(SetupFadeSetting::create(as<CCNode*>(sender)->getTag()), 99999);
        }

        void changeDual(CCObject*)
        {
            Mod::get()->setSavedValue<bool>("same-dual", !Mod::get()->getSavedValue<bool>("same-dual"));
        }

        void changeColour(CCObject* sender)
        {
            std::stringstream ss;
            ss << "selColour";
            ss << as<CCNode*>(sender)->getID();

            log::info("changing colour: {}", ss.str());

            Mod::get()->setSavedValue<int>(ss.str(), as<CCNode*>(sender)->getTag());
        }

        void generateType(CCMenu* menu, int type)
        {
            CCPoint pos = ccp((menu->getContentSize().width / 3) * type, 0) + ccp(10, -40);
            float width = menu->getContentSize().width / 4;

            auto defSpr = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
            
            if (type == 0)
                defSpr->setColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));
            else if (type == 1)
                defSpr->setColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value()));
            else
                defSpr->setColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerGlowColor.value()));

            auto d = CCLabelBMFont::create("D", "bigFont.fnt");
            d->setOpacity(100);
            defSpr->addChild(d);
            d->setPosition(defSpr->getContentSize() / 2);
            d->setAnchorPoint(ccp(0.45f, 0.45f));

            defSpr->setScale(0.7f);

            auto dBtn = CCMenuItemSpriteExtra::create(defSpr, menu, menu_selector(IconEffects::changeColour));
            dBtn->setID(std::to_string(type));
            dBtn->setPosition(pos + ccp(width, menu->getContentSize().height - 80));
            menu->addChild(dBtn);
            dBtn->setTag(0);

            auto chSpr = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
            chSpr->setScale(defSpr->getScale());
            ui->chromas.push_back(chSpr);
            auto chBtn = CCMenuItemSpriteExtra::create(chSpr, menu, menu_selector(IconEffects::changeColour));
            chBtn->setID(std::to_string(type));
            chBtn->setPosition(dBtn->getPosition() + ccp(0, -30));
            //chBtn->addChild(CCSprite::createWithSpriteFrameName("GJ_select_001.png"));
            menu->addChild(chBtn);
            chBtn->setTag(1);

            auto paSpr = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
            paSpr->setScale(defSpr->getScale());
            ui->pastels.push_back(paSpr);
            auto paBtn = CCMenuItemSpriteExtra::create(paSpr, menu, menu_selector(IconEffects::changeColour));
            paBtn->setID(std::to_string(type));
            paBtn->setPosition(chBtn->getPosition() + ccp(0, -30));
            menu->addChild(paBtn);
            paBtn->setTag(2);

            auto faSpr = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
            faSpr->setScale(paSpr->getScale());
            ui->fades.push_back(faSpr);
            auto faBtn = CCMenuItemSpriteExtra::create(faSpr, menu, menu_selector(IconEffects::changeColour));
            faBtn->setID(std::to_string(type));
            faBtn->setPosition(paBtn->getPosition() + ccp(0, -30));
            menu->addChild(faBtn);
            faBtn->setTag(3);

            auto faSSpr = CCSprite::createWithSpriteFrameName("accountBtn_settings_001.png");
            auto faSBtn = CCMenuItemSpriteExtra::create(faSSpr, menu, menu_selector(IconEffects::onFadeSettings));
            faSBtn->setTag(type);
            faSBtn->setPosition(faBtn->getPosition() + ccp(-25, 0));
            faSSpr->setScale(0.45f);
            menu->addChild(faSBtn);

            auto def = CCLabelBMFont::create("Default:", "bigFont.fnt");
            def->setPosition(pos + ccp(-6, dBtn->getPositionY() + 40));
            def->setOpacity(100);
            def->setAnchorPoint(ccp(0, 0.5f));
            def->setScale(0.5f);
            menu->addChild(def);

            auto ch = CCLabelBMFont::create("Chroma:", "bigFont.fnt");
            ch->setPosition(pos + ccp(-6, chBtn->getPositionY() + 40));
            ch->setOpacity(100);
            ch->setAnchorPoint(ccp(0, 0.5f));
            ch->setScale(0.5f);
            menu->addChild(ch);

            auto pa = CCLabelBMFont::create("Pastel:", "bigFont.fnt");
            pa->setPosition(pos + ccp(-6, paBtn->getPositionY() + 40));
            pa->setOpacity(100);
            pa->setAnchorPoint(ccp(0, 0.5f));
            pa->setScale(0.5f);
            menu->addChild(pa);

            auto fa = CCLabelBMFont::create("Fade:", "bigFont.fnt");
            fa->setPosition(pos + ccp(-6, faBtn->getPositionY() + 40));
            fa->setOpacity(100);
            fa->setAnchorPoint(ccp(0, 0.5f));
            fa->setScale(0.5f);
            menu->addChild(fa);
        }

        float x = 0;
        int selectedTab = 0;

        void onTabChanged(CCObject* sender)
        {
            auto v = as<CCMenuItemToggler*>(sender);
            selectedTab = v->getTag();

            for (size_t i = 0; i < v->getParent()->getChildrenCount(); i++)
            {
                if (auto obj = as<CCMenuItemToggler*>(v->getParent()->getChildren()->objectAtIndex(i)))
                {
                    if (obj != v)
                    {
                        obj->toggle(false);
                    }

                    if (!obj->isToggled() && selectedTab == i)
                    {
                        obj->toggle(true);
                        obj->setScale(1);
                    }
                }
            }

            log::info("ta{}", selectedTab);
        }

        CCMenuItemToggler* createBtn(const char* text)
        {
            auto v = CCMenuItemToggler::create(ButtonSprite::create(text, "goldFont.fnt", "GJ_button_04.png"), ButtonSprite::create(text, "goldFont.fnt", "GJ_button_05.png"), nullptr, menu_selector(IconEffects::onTabChanged));
            v->setPositionX(x);
            v->setAnchorPoint(ccp(0, 0.5f));
            v->setTag(selectedTab);

            x += v->getContentWidth() + 10;

            if (std::string(text) == std::string("Primary 1"))
                v->toggle(true);

            selectedTab++;

            return v;
        }

        void cocosCreate(CCMenu* menu)
        {
            x = 0;
            selectedTab = 0;

            ui = EffectUI::create();
            menu->addChild(ui);

            auto back = CCScale9Sprite::create("square02_small.png");
            back->setContentSize(ccp(menu->getContentSize().width, 80) / 0.5f);
            back->setPosition(ccp(0, menu->getContentSize().height));
            back->setAnchorPoint(ccp(0, 1));
            back->setScale(0.5f);
            back->setOpacity(100);
            menu->addChild(back);

            auto back2 = CCScale9Sprite::create("square02_small.png");
            back2->setContentSize(ccp(menu->getContentSize().width, menu->getContentSize().height - 80 - 6) / 0.5f);
            back2->setPosition(ccp(0, 0));
            back2->setAnchorPoint(ccp(0, 0));
            back2->setScale(0.5f);
            back2->setOpacity(100);
            menu->addChild(back2);

            #pragma region Normal

            auto m = CCMenu::create();
            m->setAnchorPoint(ccp(0, 0));
            m->ignoreAnchorPointForPosition(false);
            m->setPosition(ccp(10, 10));
            m->setPositionY(back->getContentSize().height - 40);
            m->setContentSize(back->getContentSize() - ccp(120, 20));
            back->addChild(m);

            m->addChild(getPlayer(IconType::Cube, ui));
            m->addChild(getPlayer(IconType::Ship, ui));
            m->addChild(getPlayer(IconType::Ball, ui));
            m->addChild(getPlayer(IconType::Ufo, ui));
            m->addChild(getPlayer(IconType::Wave, ui));
            m->addChild(getPlayer(IconType::Robot, ui));
            m->addChild(getPlayer(IconType::Spider, ui));
            m->addChild(getPlayer(IconType::Swing, ui));
            m->addChild(getPlayer(IconType::Jetpack, ui));

            m->setLayout(RowLayout::create()->setGap(64)->setAutoScale(false)->setGrowCrossAxis(false));
            m->updateLayout();

            #pragma endregion

            #pragma region Dual

            auto m2 = CCMenu::create();
            m2->setAnchorPoint(ccp(0, 0));
            m2->ignoreAnchorPointForPosition(false);
            m2->setPosition(ccp(10, 10));
            m2->setPositionY(40);
            m2->setContentSize(back->getContentSize() - ccp(120, 20));
            back->addChild(m2);

            m2->addChild(getPlayer(IconType::Cube, ui, true));
            m2->addChild(getPlayer(IconType::Ship, ui, true));
            m2->addChild(getPlayer(IconType::Ball, ui, true));
            m2->addChild(getPlayer(IconType::Ufo, ui, true));
            m2->addChild(getPlayer(IconType::Wave, ui, true));
            m2->addChild(getPlayer(IconType::Robot, ui, true));
            m2->addChild(getPlayer(IconType::Spider, ui, true));
            m2->addChild(getPlayer(IconType::Swing, ui, true));
            m2->addChild(getPlayer(IconType::Jetpack, ui, true));

            m2->setLayout(RowLayout::create()->setGap(64)->setAutoScale(false)->setGrowCrossAxis(false));
            m2->updateLayout();

            #pragma endregion

            auto sameTitle = CCLabelBMFont::create("Same\nDual\nColour", "bigFont.fnt", 110, CCTextAlignment::kCCTextAlignmentCenter);
            sameTitle->setPosition(menu->getContentSize() + ccp(-25, -20));
            sameTitle->setScale(0.3f);
            menu->addChild(sameTitle);

            auto sameDual = CCMenuItemToggler::createWithStandardSprites(menu, menu_selector(IconEffects::changeDual), 0.6f);
            sameDual->setPosition(menu->getContentSize() + ccp(-25, -50));
            sameDual->toggle(Mod::get()->getSavedValue<bool>("same-dual"));
            menu->addChild(sameDual);

            for (size_t i = 0; i < 2; i++)
            {
                auto split = CCSprite::createWithSpriteFrameName("floorLine_001.png");
                split->setPosition(ccp(back2->getContentSize().width / 3 * (i + 1), back2->getContentSize().height / 2));
                split->setScaleY(2);
                split->setScaleX(0.7f);
                split->setRotation(-90);
                split->setOpacity(100);
                split->setAnchorPoint(ccp(0.5f, 0));

                auto blend = ccBlendFunc({GL_ONE, GL_ONE_MINUS_CONSTANT_ALPHA});
                split->setBlendFunc(blend);

                back2->addChild(split);
            }

            for (size_t i = 0; i < 3; i++)
            {
                auto title = CCLabelBMFont::create(i == 0 ? "Primary" : (i == 1 ? "Secondary" : "Glow"), "bigFont.fnt");
                title->setPosition(ccp((back2->getContentSize().width / 3) * i + ((back2->getContentSize().width / 3) / 2), back2->getContentSize().height - 20));

                back2->addChild(title);

                generateType(menu, i);
            }

            return;

            auto menu2 = CCMenu::create();
            menu2->setContentHeight(0);
            menu2->setAnchorPoint(ccp(0, 0.5f));
            menu2->setScale(0.5f);
            menu2->setPosition(ccp(10, 160));

            menu2->addChild(createBtn("Primary"));
            menu2->addChild(createBtn("Secondary"));
            menu2->addChild(createBtn("Glow"));
            menu2->addChild(createBtn("Trail"));
            menu2->addChild(createBtn("Wave Trail"));

            menu->addChild(menu2);

            selectedTab = 0;
        }
};