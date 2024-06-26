#pragma once

#include <Geode/Geode.hpp>
#include "Module.h"
#include "Dropdown.h"
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "Replay.h"

#include "../Layers/SaveMacroPopup.h"
#include "../Layers/LoadMacroPopup.h"
#include "../Layers/EditStatusPositionLayer.h"
#include "../Layers/ConfirmFLAlertLayer.h"
#include "../Layers/ReplayOptionsLayer.h"

#include "../Utils/Utils.hpp"

class Client;

class Window
{
public:
    std::string name;
    std::string id;
    bool excludeAndroid = false;
    std::vector<Module*> modules;
    CCPoint windowPos = CCPoint(100, 100);

    bool dragging = false;
    CCPoint offset = CCPoint(0, 0);

    bool isClosed = false;
    float v = 1.0f;

    bool touchBegan(CCPoint point, CCTouch* touch);
    bool touchMoved(CCPoint point, CCTouch* touch);
    bool touchEndedOrCancelled(CCPoint point, CCTouch* touch, bool cancelled);

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

        /*ImVec2 wp = DrawUtils::addImVec2(windowPos, getOffsetForTime(anim));

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

        return o;*/
        return false;
    }

    void preDraw();
    void drawWindow();
    void postDraw();

    void move(bool o, ImVec2 tileSize)
    {
        if (o)
        {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !dragging)
            {
                if (ImGui::GetIO().MousePos.y < windowPos.y + tileSize.y)
                {
                    dragging = true;

                    //offset = ImVec2(windowPos.x - ImGui::GetIO().MousePos.x, windowPos.y - ImGui::GetIO().MousePos.y);
                }
            }
        }

        if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            dragging = false;
        }

        if (dragging)
        {
            //windowPos = DrawUtils::addImVec2(ImGui::GetIO().MousePos, offset);
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

    CCPoint offsetForTime(float time);

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

            //case DrawUtils::animType::v6:
                //return ImVec2(0, (wndSize.y * ((i % 2 == 0) ? 1 : -1)) * (1 - time));

            default:
                break;
        }

        return ImVec2(0, wndSize.y * (1 - quadraticEaseInOut(time)));
    }

    virtual void cocosCreate(CCMenu* menu)
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
        
        auto scroll = geode::ScrollLayer::create(menu->getContentSize());
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
        scroll->enableScrollWheel();

        //auto scrollbar = geode::Scrollbar::create(scroll);
        //scrollbar->setPositionY(menu->getContentHeight() / 2);
        //scrollbar->setPositionX(menu->getContentWidth() + 0.5f);
        //menu->addChild(scrollbar);
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
            SpeedhackTop::instance->onChange();
        }

        void onPreset(CCObject* sender)
        {
            float value = numFromString<float>(as<CCNode*>(sender)->getID(), 2).value();

            SpeedhackTop::instance->text = as<CCNode*>(sender)->getID();
            auto inp = getChildOfType<TextInput>(static_cast<CCNode*>(sender)->getParent()->getParent(), 0);
            inp->setString(as<CCNode*>(sender)->getID());
            slider->setValue(unscaleValue(value));
            
            SpeedhackTop::instance->save();
            SpeedhackTop::instance->onChange();
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
            
            auto back = CCScale9Sprite::create("square02b_small.png");
            back->setContentSize(menu->getContentSize() / 0.5f);
            back->setPosition(ccp(0, 0));
            back->setAnchorPoint(ccp(0, 0));
            back->setScale(0.5f);
            back->setColor(ccc3(0, 0, 0));
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
            //PlayLayer::get()->m_player1->m_position += ccp(30, 150);

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
            
        }

        void cocosCreate(CCMenu* menu)
        {
            Window::cocosCreate(menu);

            auto pos = ccp(menu->getContentSize().width, 0) + ccp(-58, 22);

            CCMenuItemSpriteExtra* btn = nullptr;

            auto btnUnc = ButtonSprite::create("Uncomplete\nLevel", 70, false, "bigFont.fnt", "GJ_button_05.png", 35, 0.75f);
            
            as<CCNode*>(btnUnc->getChildren()->objectAtIndex(0))->setScale(0.375f);
            as<CCLabelBMFont*>(btnUnc->getChildren()->objectAtIndex(0))->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
            as<CCLabelBMFont*>(btnUnc->getChildren()->objectAtIndex(0))->updateLabel();

            btn = CCMenuItemSpriteExtra::create(btnUnc, menu, menu_selector(Level::onUnc));
            btn->setSizeMult(1.15f);
            btn->setPosition(pos);
            menu->addChild(btn);
        }
};