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

        for (size_t m = 0; m < modules.size(); m++)
        {
            float x = 20;

            if (!(m % 2 == 0))
                x = 188;

            modules[m]->makeAndroid(menu, {x, (menu->getContentSize().height - 20 - 20) - 7 - (28.0f * (y - 1.0f)) });

            if ((m - 1) % 2 == 0 && m != 0)
                y++;
        }
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

        void clear(CCObject* sender)
        {
            SpeedhackTop::instance->text = "";
            reinterpret_cast<geode::InputNode*>(static_cast<CCNode*>(sender)->getParent()->getChildByID("IGNOREBYPASSES"_spr))->setString("");
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

        void sliderChanged(CCObject* sender)
        {
            float v = ((slider->getThumb()->getPositionX() + 100) / 200.0f);

            std::stringstream ss;
            ss << round(scaleValue(v) * 100.0) / 100.0;

            auto inp = static_cast<geode::InputNode*>(static_cast<CCNode*>(sender)->getParent()->getParent()->getParent()->getChildByID("IGNOREBYPASSES"_spr));
            inp->setString(ss.str().c_str());
            SpeedhackTop::instance->text = ss.str();

            SpeedhackTop::instance->save();
        }

        void cocosCreate(CCMenu* menu)
        {
            float v = 1.0f;

            if (SpeedhackTop::instance->text.size() != 0 && !SpeedhackTop::instance->text.ends_with("."))
            {
                v = std::stof(SpeedhackTop::instance->text);
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

            modules[1]->makeAndroid(menu, ccp(menu->getContentSize().width / 2, menu->getContentSize().height - 110) - ccp(180 / 2, 0) + ccp(10, 0));
            modules[2]->makeAndroid(menu, ccp(menu->getContentSize().width / 2, menu->getContentSize().height - 110 - (30 * 1)) - ccp(180 / 2, 0) + ccp(10, 0));
            modules[3]->makeAndroid(menu, ccp(menu->getContentSize().width / 2, menu->getContentSize().height - 110 - (30 * 2)) - ccp(180 / 2, 0) + ccp(10, 0));
            //static_cast<geode::InputNode*>(menu->getChildByID("speedhack-top"))->getInput()->setDelegate(this);

            auto trash = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png"), menu, menu_selector(Speedhack::clear));
            trash->m_baseScale = 0.725f;
            trash->setScale(0.725f);
            trash->setPosition(ccp((menu->getContentSize().width / 2) + (180 / 2) + 20, menu->getContentSize().height - 50));
            menu->addChild(trash);
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

class Config : public Window
{
    public:
        Config()
        {
            name = "Config";
            id = "config-window";
        }

        static inline std::vector<CCScale9Sprite*> btns = {};
        static inline std::vector<CCMenuItemSprite*> btnsS = {};
        static inline Slider* normal = nullptr;
        static inline Slider* GP = nullptr;
        static inline Slider* ED = nullptr;
        static inline Dropdown* dd = nullptr;

        void changeTheme(CCObject* sender)
        {
            int v = reinterpret_cast<CCNode*>(sender)->getTag();

            Mod::get()->setSavedValue<int>("theme", v);

            log::info("change theme to {}", v);

            for (size_t i = 0; i < btns.size(); i++)
            {
                if (i == v - 1)
                {
                    btns[i]->setColor({255, 255, 255});
                    btns[i]->setOpacity(255);
                }
                else
                {
                    btns[i]->setColor({150, 150, 150});
                    btns[i]->setOpacity(200);
                }

                btnsS[i]->setEnabled(i != v - 1);
            }
        }

        void createBtn(CCNode* menu, int i)
        {
            std::stringstream ss;
            ss << "GJ_square0";
            ss << i;
            ss << ".png";

            auto spr = CCScale9Sprite::create(ss.str().c_str());
            spr->setColor({150, 150, 150});
            spr->setOpacity(200);
            auto sprSel = CCScale9Sprite::create(ss.str().c_str());
            sprSel->setColor({200, 200, 200});

            auto btn = CCMenuItemSprite::create(spr, sprSel, menu, menu_selector(Config::changeTheme));
            btn->setContentSize(ccp(100, 35) * 2);
            spr->setContentSize(ccp(100, 35) * 2);
            spr->setPosition(ccp(0, 0));
            sprSel->setContentSize(ccp(100, 35) * 2);
            sprSel->setPosition(ccp(0, 0));

            btn->setTag(i);

            btn->setEnabled(i != Mod::get()->getSavedValue<int>("theme", 5));

            if (!btn->isEnabled())
            {
                spr->setColor({255, 255, 255});
                spr->setOpacity(255);
            }

            menu->addChild(btn);

            btns.push_back(spr);
            btnsS.push_back(btn);
        }

        void onSliderChanged(CCObject* sender)
        {
            float ov = (normal->getThumb()->getPositionX() + 50.0f) / 100.0f;
            int v = round(ov * 255.0f);
            if (v < 10)
                v = 10;

            Mod::get()->setSavedValue<int>("normal-opacity", (int)(v));

            float ovgp = (GP->getThumb()->getPositionX() + 50.0f) / 100.0f;
            int vgp = round(ovgp * 255.0f);

            Mod::get()->setSavedValue<int>("gameplay-opacity", (int)(vgp));

            float oved = (ED->getThumb()->getPositionX() + 50.0f) / 100.0f;
            int ved = round(oved * 255.0f);

            Mod::get()->setSavedValue<int>("editor-opacity", (int)(ved));
        }

        void onChangeDraggable(CCObject* sender)
        {

        }

        void onLink(CCObject* sender)
        {
            auto a = geode::createQuickPopup(
                "Hold Up!",
                "Links are spooky! Are you sure you want to go to\n<cy>" + std::string(as<CCNode*>(sender)->getID()) + "</c>?",
                "Cancel", "Yes",
                [](FLAlertLayer* a, bool btn2) {
                    if (btn2) {
                        CCApplication::get()->openURL(a->getID().c_str());
                    }
                }
            );

            a->setID(as<CCNode*>(sender)->getID());
        }

        void onDropdownChanged(CCObject*)
        {
            Mod::get()->setSavedValue<int>("anim-mode", dd->getSelectedIndex());
        }

        void cocosCreate(CCMenu* menu)
        {
            btns.clear();
            btnsS.clear();

            auto back = CCScale9Sprite::create("square02_small.png");
            back->setContentSize(ccp(110, menu->getContentSize().height) / 0.5f);
            back->setPosition(ccp(0, 0));
            back->setAnchorPoint(ccp(0, 0));
            back->setScale(0.5f);
            back->setOpacity(100);

            auto m = CCMenu::create();
            m->setAnchorPoint(back->getAnchorPoint());
            m->setPosition(back->getPosition() + ccp(10, -10));
            m->setContentSize(back->getContentSize());

            createBtn(m, 1);
            createBtn(m, 2);
            createBtn(m, 3);
            createBtn(m, 4);
            createBtn(m, 5);

            m->setLayout(ColumnLayout::create()->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End)->setCrossAxisOverflow(true)->setAutoScale(false)->setGap(10));
            m->updateLayout();
            back->addChild(m);

            menu->addChild(back);

            auto devText = CCLabelBMFont::create("Re-open menu to apply changes", "chatFont.fnt");
            devText->setColor({0, 0, 0});
            devText->setOpacity(100);
            devText->setAnchorPoint(ccp(0.5f, 0));
            devText->setScale(0.45f);
            devText->setPosition(ccp((menu->getContentSize().width / 2) + 65, 1));
            menu->addChild(devText);

            auto lNormal = CCLabelBMFont::create("Normal:", "bigFont.fnt");
            lNormal->setPosition(ccp(120, menu->getContentSize().height - 5));
            lNormal->setAnchorPoint(ccp(0, 1));
            lNormal->setScale(0.5f);

            normal = Slider::create(menu, menu_selector(Config::onSliderChanged), 0.5f);
            normal->setValue((Mod::get()->getSavedValue<int>("normal-opacity", 255) / 255.0f));
            normal->setPosition(ccp(lNormal->getPositionX() + 115, lNormal->getPositionY() - 10));
            normal->setScaleX(0.8f);
            normal->getThumb()->setScaleX((1.0f / 0.8f) * 0.5f);

            auto lGP = CCLabelBMFont::create("Gameplay:", "bigFont.fnt");
            lGP->setPosition(ccp(120, menu->getContentSize().height - 5 - 20));
            lGP->setAnchorPoint(ccp(0, 1));
            lGP->setScale(0.5f);

            GP = Slider::create(menu, menu_selector(Config::onSliderChanged), 0.5f);
            GP->setValue((Mod::get()->getSavedValue<int>("gameplay-opacity", 50) / 255.0f));
            GP->setPosition(ccp(lGP->getPositionX() + 115, lGP->getPositionY() - 10));
            GP->setScaleX(0.8f);
            GP->getThumb()->setScaleX((1.0f / 0.8f) * 0.5f);

            auto lED = CCLabelBMFont::create("Editor:", "bigFont.fnt");
            lED->setPosition(ccp(120, menu->getContentSize().height - 5 - 20 - 20));
            lED->setAnchorPoint(ccp(0, 1));
            lED->setScale(0.5f);

            ED = Slider::create(menu, menu_selector(Config::onSliderChanged), 0.5f);
            ED->setValue((Mod::get()->getSavedValue<int>("editor-opacity", 50) / 255.0f));
            ED->setPosition(ccp(lED->getPositionX() + 115, lED->getPositionY() - 10));
            ED->setScaleX(0.8f);
            ED->getThumb()->setScaleX((1.0f / 0.8f) * 0.5f);

            modules[0]->makeAndroid(menu, ccp(132, menu->getContentSize().height - 90 - 28));
            modules[1]->makeAndroid(menu, ccp(132, menu->getContentSize().height - 90 - 30 - 28));
            modules[2]->makeAndroid(menu, ccp(132, menu->getContentSize().height - 90 - 30 - 28 - 28));

            menu->addChild(lNormal);
            menu->addChild(normal);
            menu->addChild(lGP);
            menu->addChild(GP);
            menu->addChild(lED);
            menu->addChild(ED);

            auto discord = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("gj_discordIcon_001.png"), menu, menu_selector(Config::onLink)); // https://discord.gg/DfQSTEnQKK
            discord->setPosition(ccp(menu->getContentSize().width, 0) + ccp(-10, 12));
            discord->setID("https://discord.gg/DfQSTEnQKK");
            menu->addChild(discord);

            auto yt = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png"), menu, menu_selector(Config::onLink)); // https://www.youtube.com/@TheSillyDoggo
            yt->setPosition(ccp(menu->getContentSize().width, 0) + ccp(-10, 12) + ccp(0, 35));
            yt->setID("https://www.youtube.com/@TheSillyDoggo");
            menu->addChild(yt);

            dd = Dropdown::create({130, 25}, {"None", "From Top", "From Bottom", "From Left", "From Right", "Scale"}, menu_selector(Config::onDropdownChanged), Mod::get()->getSavedValue<int>("anim-mode", 2));
            dd->setPosition(ccp(120.5f, menu->getContentSize().height - 90));
            menu->addChild(dd);
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

class $modify (PlayerObject)
{
    bool init(int p0, int p1, GJBaseGameLayer* p2, cocos2d::CCLayer* p3, bool p4)
    {
        if (!PlayerObject::init(p0, p1, p2, p3, p4))
            return false;

        auto l = as<CCNode*>(this->getChildren()->objectAtIndex(0));
        as<CCSprite*>(l->getChildren()->objectAtIndex(0))->setID("gamemode-frame");
        as<CCSprite*>(as<CCSprite*>(l->getChildren()->objectAtIndex(0))->getChildren()->objectAtIndex(0))->setID("secondary-frame");
        as<CCSprite*>(as<CCSprite*>(l->getChildren()->objectAtIndex(0))->getChildren()->objectAtIndex(1))->setID("highlights-frame");

        as<CCSprite*>(l->getChildren()->objectAtIndex(1))->setID("ship-frame");
        as<CCSprite*>(as<CCSprite*>(l->getChildren()->objectAtIndex(1))->getChildren()->objectAtIndex(0))->setID("secondary-frame");

        as<CCSprite*>(l->getChildren()->objectAtIndex(6))->setID("glow-frame");
        as<CCSprite*>(as<CCSprite*>(l->getChildren()->objectAtIndex(6))->getChildren()->objectAtIndex(0))->setID("dash-frame");
        as<CCSprite*>(as<CCSprite*>(l->getChildren()->objectAtIndex(6))->getChildren()->objectAtIndex(1))->setID("gamemode-glow");
        as<CCSprite*>(as<CCSprite*>(l->getChildren()->objectAtIndex(6))->getChildren()->objectAtIndex(2))->setID("ship-glow");

        return true;
    }

    TodoReturn createRobot(int p0)
    {
        PlayerObject::createRobot(p0);
    }
};

class $modify (GJBaseGameLayer)
{
    virtual void update(float p0)
    {
        GJBaseGameLayer::update(p0);

        if (m_player1)
        {
            auto l = as<CCNode*>(m_player1->getChildren()->objectAtIndex(0));

            as<CCSprite*>(l->getChildByID("glow-frame")->getChildByID("gamemode-glow"))->setColor(EffectUI::getColourForSelected(2));
            as<CCSprite*>(l->getChildByID("glow-frame")->getChildByID("ship-glow"))->setColor(EffectUI::getColourForSelected(2));

            as<CCSprite*>(l->getChildByID("gamemode-frame"))->setColor(EffectUI::getColourForSelected(0));
            as<CCSprite*>(l->getChildByID("ship-frame"))->setColor(EffectUI::getColourForSelected(0));

            as<CCSprite*>(l->getChildByID("gamemode-frame")->getChildByID("secondary-frame"))->setColor(EffectUI::getColourForSelected(1));
            as<CCSprite*>(l->getChildByID("ship-frame")->getChildByID("secondary-frame"))->setColor(EffectUI::getColourForSelected(1));
        }

        if (m_player2)
        {
            auto l = as<CCNode*>(m_player2->getChildren()->objectAtIndex(0));

            if (!Mod::get()->getSavedValue<bool>("same-dual"))
            {
                as<CCSprite*>(l->getChildByID("gamemode-frame"))->setColor(EffectUI::getColourForSelected(1));
                as<CCSprite*>(l->getChildByID("ship-frame"))->setColor(EffectUI::getColourForSelected(1));

                as<CCSprite*>(l->getChildByID("gamemode-frame")->getChildByID("secondary-frame"))->setColor(EffectUI::getColourForSelected(0));
                as<CCSprite*>(l->getChildByID("ship-frame")->getChildByID("secondary-frame"))->setColor(EffectUI::getColourForSelected(0));
            }
            else
            {
                as<CCSprite*>(l->getChildByID("gamemode-frame"))->setColor(EffectUI::getColourForSelected(0));
                as<CCSprite*>(l->getChildByID("ship-frame"))->setColor(EffectUI::getColourForSelected(0));

                as<CCSprite*>(l->getChildByID("gamemode-frame")->getChildByID("secondary-frame"))->setColor(EffectUI::getColourForSelected(1));
                as<CCSprite*>(l->getChildByID("ship-frame")->getChildByID("secondary-frame"))->setColor(EffectUI::getColourForSelected(1));
            }

            as<CCSprite*>(l->getChildByID("glow-frame")->getChildByID("gamemode-glow"))->setColor(EffectUI::getColourForSelected(2));
            as<CCSprite*>(l->getChildByID("glow-frame")->getChildByID("ship-glow"))->setColor(EffectUI::getColourForSelected(2));
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
            ss << as<CCNode*>(sender)->getID(); // adb push TheSillyDoggo.Cheats.geode /storage/emulated/0/Android/media/com.geode.launcher/game/geode/mods/

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

        void cocosCreate(CCMenu* menu)
        {
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
            }
        }

        void onFPS(CCObject*)
        {
            SetupFPSBypass::addToScene();
        }

        void cocosCreate(CCMenu* menu)
        {
            Window::cocosCreate(menu);

            auto pos = ccp(menu->getContentSize().width, 0) + ccp(-28 - 18, 22);

            auto btnS = ButtonSprite::create("FPS\nBypass", 60, false, "bigFont.fnt", "GJ_button_05.png", 35, 0.75f);
            as<CCNode*>(btnS->getChildren()->objectAtIndex(0))->setScale(0.375f);
            as<CCLabelBMFont*>(btnS->getChildren()->objectAtIndex(0))->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
            as<CCLabelBMFont*>(btnS->getChildren()->objectAtIndex(0))->updateLabel();
            auto btn = CCMenuItemSpriteExtra::create(btnS, menu, menu_selector(Level::onFPS));
            btn->setSizeMult(1.15f);
            btn->setPosition(pos);
            menu->addChild(btn);

            pos = pos + ccp(-30 - 27, -5);

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

            return;

            auto btnS = ButtonSprite::create("Transition\nCustomizer", 90, false, "bigFont.fnt", "GJ_button_05.png", 35, 0.75f);
            as<CCNode*>(btnS->getChildren()->objectAtIndex(0))->setScale(0.375f);
            as<CCLabelBMFont*>(btnS->getChildren()->objectAtIndex(0))->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
            as<CCLabelBMFont*>(btnS->getChildren()->objectAtIndex(0))->updateLabel();
            auto btn = CCMenuItemSpriteExtra::create(btnS, menu, menu_selector(Universal::onTransCustomizer));
            btn->setSizeMult(1.15f);
            btn->setPosition(pos);
            menu->addChild(btn);
        }
};

class _Replay : public Window
{
    public:
        void onOnlineMacros(CCObject*)
        {
            FLAlertLayer::create("Coming soon...", "Online macro browser is coming soon...", "OK")->show();
        }

        void onSave(CCObject*)
        {
            //as<OpenMacroModule*>(Client::GetModule("SaveMacroPopup::addToScene"))->onSaveMacro();
            //SaveMacroPopup::addToScene();
        }

        void onSecret(CCObject*)
        {
            auto s = PlayLayer::create(GameLevelManager::get()->getMainLevel(3001, false), false, false);
            s->setScale(0);
            s->runAction(CCScaleTo::create(0.2f, 1.0f));
            CCScene::get()->addChild(s, 99999);
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

            auto l = CCLabelBMFont::create("ill update it later", "bigFont.fnt");
            l->setScale(0.8f);
            l->setPosition(menu->getContentSize() / 2 + ccp(0, 50));
            menu->addChild(l);

            auto spike = GameObject::createWithKey(8);
            auto btna = CCMenuItemSpriteExtra::create(spike, menu, menu_selector(_Replay::onSecret));
            btna->setPosition(menu->getContentSize() / 2 + ccp(0, -40));

            menu->addChild(btna, 1);

            return;

            auto lbl = CCLabelBMFont::create("Record\nPlayback", "bigFont.fnt");
            lbl->setPosition(ccp(10, menu->getContentSize().height - 2));
            lbl->setScale(0.725f);
            lbl->setAnchorPoint(ccp(0, 1));
            lbl->setOpacity(100);

            auto btnP = CCMenuItemToggler::createWithStandardSprites(menu, nullptr, 1.0f);

            auto menuRow = CCMenu::create();
            menuRow->ignoreAnchorPointForPosition(false);
            menuRow->setContentSize(ccp(9999, 0));
            menuRow->setScale(0.625f);
            menuRow->setPosition(menu->getContentSize() / 2 + ccp(0, -30));

            menuRow->addChild(CCMenuItemSpriteExtra::create(ButtonSprite::create("Save", "bigFont.fnt", "GJ_button_04.png"), menu, menu_selector(_Replay::onSave)));
            menuRow->addChild(CCMenuItemSpriteExtra::create(ButtonSprite::create("Load", "bigFont.fnt", "GJ_button_04.png"), menu, nullptr));
            menuRow->addChild(CCMenuItemSpriteExtra::create(ButtonSprite::create("More", "bigFont.fnt", "GJ_button_04.png"), menu, nullptr));

            menuRow->setLayout(RowLayout::create()->setAutoScale(false)->setGap(55));

            auto pos = ccp(menu->getContentSize().width, 0) + ccp(-55, 22);

            auto btnS = ButtonSprite::create("Download\nMacros Online", 90, false, "bigFont.fnt", "GJ_button_05.png", 35, 0.75f);
            as<CCNode*>(btnS->getChildren()->objectAtIndex(0))->setScale(0.3f);
            as<CCLabelBMFont*>(btnS->getChildren()->objectAtIndex(0))->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
            as<CCLabelBMFont*>(btnS->getChildren()->objectAtIndex(0))->updateLabel();
            auto btn = CCMenuItemSpriteExtra::create(btnS, menu, menu_selector(_Replay::onOnlineMacros));
            btn->setSizeMult(1.15f);
            btn->setPosition(pos);

            menu->addChild(lbl);
            menu->addChild(menuRow);
            menu->addChild(btn);
            menu->addChild(btnP);
        }
};
