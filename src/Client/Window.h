#include <Geode/Geode.hpp>
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

class Speedhack : public Window
{
    public:
        Speedhack()
        {
            name = "Speedhack";
            id = "speedhack-window";
        }

        void clear(CCObject* sender)
        {
            SpeedhackTop::instance->text = "";
            reinterpret_cast<geode::InputNode*>(static_cast<CCNode*>(sender)->getParent()->getChildByID("speedhack-top"))->setString("");
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

            for (size_t m = 0; m < modules.size(); m++)
            {
                modules[m]->makeAndroid(menu, {20, (menu->getContentSize().height - 20 - 20) - 7 - (28.0f * (m - 1.0f))});
            }

            auto trash = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png"), menu, menu_selector(Speedhack::clear));
            trash->m_baseScale = 0.725f;
            trash->setScale(0.725f);
            trash->setPosition(ccp(210, menu->getContentSize().height - 20));
            menu->addChild(trash);
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

            return;
            switch (static_cast<CCNode*>(sender)->getTag())
            {
                case 0:
                    ProfilePage::create(7107344, false)->show();
                    return;

                case 1:
                    ProfilePage::create(14467409, false)->show();
                    return;

                case 2:
                    CCApplication::sharedApplication()->openURL("https://gist.github.com/absoIute/657a4c95bb92755f96e20772adbf5f32");
                    return;

                case 3:
                    ProfilePage::create(6253758, false)->show();
                    //CCApplication::sharedApplication()->openURL("https://github.com/FireMario211");
                    return;

                case 4:
                    ProfilePage::create(16778880, false)->show();
                    return;

                case 5:
                    ProfilePage::create(7236822, false)->show();
                    return;

                default: return;
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


            auto kmImg = SimplePlayer::create(1);
            kmImg->setColor({125, 255, 0});
            kmImg->setSecondColor({0, 255, 255});
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

            modules[0]->makeAndroid(menu, ccp(132, menu->getContentSize().height - 90));
            modules[1]->makeAndroid(menu, ccp(132, menu->getContentSize().height - 90 - 28));

            menu->addChild(lNormal);
            menu->addChild(normal);
            menu->addChild(lGP);
            menu->addChild(GP);
            menu->addChild(lED);
            menu->addChild(ED);
        }
};