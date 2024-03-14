#pragma once

#include "ColourUtility.h"
#include "DrawUtils.h"

#include "../Layers/ColouredAlertLayer.h"

#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

class ModuleChangeDelegate
{
    public:
        virtual void onModuleChanged(bool enabled)
        {

        }
};

class Module
{
    public:

        static inline std::string descMod = "";

        enum kModuleType
        {
            Toggle,
            Button,
            Float
        };

        kModuleType moduleType = kModuleType::Toggle;

        Module()
        {

        }

        Module(std::string n, std::string i)
        {
            name = n;
            id = i;
            enabled = false;

            this->load();
        }

        Module(std::string n, std::string i, std::string d, bool _def = false, std::string a = "", bool as = false)
        {
            name = n;
            id = i;
            description = d;
            enabled = false;
            def = _def;
            useAlert = as;
            alert = a;

            this->load();
        }

        Module(std::string n, std::string i, kModuleType type)
        {
            name = n;
            id = i;
            moduleType = type;

            this->load();
        }

        std::string name;
        std::string id;
        std::string description;
        std::string alert;
        bool enabled;
        bool useAlert;
        bool onceAlert;

        bool vAlert;

        bool def;
        float value = 1.0f;

        ModuleChangeDelegate* delegate = nullptr;

        virtual bool Draw(ImVec2 tileSize)
        {
            ImVec2 pos = ImGui::GetCursorPos();

            DrawUtils::drawRect(pos, tileSize, ColourUtility::GetColour(DrawUtils::mouseWithinRect(ImVec4(pos.x, pos.y, tileSize.x, tileSize.y)) ? (ImGui::IsMouseDown(ImGuiMouseButton_Left) ? ColourUtility::ClientColour::Pressed : ColourUtility::ClientColour::Hovered) : ColourUtility::ClientColour::WindowBG));

            if (moduleType == kModuleType::Float)
            {
                DrawUtils::drawRect(pos, ImVec2(tileSize.x * value, tileSize.y), ColourUtility::GetColour(ColourUtility::ClientColour::Accent));
            }

            DrawUtils::anchoredText(DrawUtils::addImVec2(pos, ImVec2(3, 0)), tileSize, name.c_str(), ColourUtility::GetColour(enabled ? ColourUtility::Accent : ColourUtility::Text), ImVec2(0, 0.5f));

            ImGui::SetCursorPos(pos);
            if (DrawUtils::mouseWithinRect(ImVec4(pos.x, pos.y, tileSize.x, tileSize.y)))
            {
                descMod = description;

                if (moduleType == kModuleType::Float)
                {
                    if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
                    {
                        value = (ImGui::GetIO().MousePos.x - pos.x) / tileSize.x;
                    }
                }
                else
                {
                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    {
                        enabled = !enabled;

                        this->save();
                        return true;
                    }
                }
            }

            return false;
        }

        virtual void OnChange()
        {
            if (delegate)
                delegate->onModuleChanged(this->enabled);
        }

        virtual void save()
        {
            geode::prelude::Mod::get()->setSavedValue<bool>(id + "_enabled", enabled);
        }

        virtual void load()
        {
            enabled = geode::prelude::Mod::get()->getSavedValue<bool>(id + "_enabled", def);
        }

        void onInfoAndroid(CCObject* sender)
        {
            auto dat = static_cast<Module*>(static_cast<CCNode*>(sender)->getUserData());

            //fucking force priority kill yourself
            auto al = FLAlertLayer::create(dat->name.c_str(), dat->description.c_str(), "OK");
            al->show();

            CCTouchDispatcher::get()->addTargetedDelegate(al, -514, true);
            al->setTouchPriority(-514);

            //al->m_buttonMenu->setTouchPriority(-513);
        }


        void onToggleAndroid(CCObject* sender)
        {
            auto dat = static_cast<Module*>(static_cast<CCNode*>(sender)->getUserData());

            log::info("alert: {}", dat->alert.c_str());

            if (dat->useAlert && !dat->vAlert)
            {
                //create(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale)
                //ColouredAlertLayer::addToScene(dat->name, dat->alert);
                FLAlertLayer::create(dat->name.c_str(), dat->alert.c_str(), "OK")->show();

                dat->vAlert = true;

                as<CCMenuItemToggler*>(sender)->toggle(!as<CCMenuItemToggler*>(sender)->isToggled());

                return;
            }

            dat->enabled = !dat->enabled;
            dat->save();
            dat->OnChange();

            log::info("Toggling {}", dat->id);
            log::info("enabled status: {}", dat->enabled);
        }

        virtual void makeAndroid(CCMenu* menu, CCPoint pos)
        {
            auto btn = CCMenuItemToggler::createWithStandardSprites(menu, menu_selector(Module::onToggleAndroid), 0.75f);
            btn->setUserData(this);
            btn->setID(id);
            btn->toggle(enabled);
            btn->setPosition(pos);

            auto label = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
            label->setAnchorPoint(ccp(0, 0.5f));
            label->setScale(0.575f);
            label->setPosition(pos + ccp(15, 0));
            label->limitLabelWidth(110, 0.575f, 0.1f);

            auto info = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"), menu, menu_selector(Module::onInfoAndroid));
            info->setScale(0.55f);
            info->m_baseScale = info->getScale();
            info->setPosition(pos + ccp(label->getScaledContentSize().width + 30, 0) + ccp(-5, 5));
            info->setContentSize(info->getContentSize() * 2.5f);
            static_cast<CCSprite*>(info->getChildren()->objectAtIndex(0))->setPosition(info->getContentSize() / 2);
            info->setUserData(this);

            menu->addChild(btn);
            menu->addChild(info);
            menu->addChild(label);
        }

};

class InputModule : public Module, public TextInputDelegate
{
    public:
        float speedhackV = 1.0f;
        static inline InputModule* selected = nullptr;

        std::string text = "";
        std::string format = "%";
        std::string allowedChars = "0987654321.";

        int maxSize = 5;

        InputModule()
        {
            this->load();
        }

        bool Draw(ImVec2 tileSize)
        {
            ImVec2 pos = ImGui::GetCursorPos();

            DrawUtils::drawRect(pos, tileSize, ColourUtility::GetColour(ColourUtility::ClientColour::WindowBG));
            DrawUtils::drawRect(ImVec2(pos.x + 4, pos.y + 4), ImVec2(tileSize.x - 4 - 4, tileSize.y - 4 - 4), ColourUtility::GetColour(ColourUtility::ClientColour::InputField));

            if (selected == this)
            {
                if ((int)(round(ImGui::GetTime() / 0.5f)) % 2 == 0)
                {
                    DrawUtils::drawRect(ImVec2(pos.x + 4 + 2 + (ImGui::CalcTextSize(text.c_str()).x), pos.y + 4 + 4), ImVec2(2, tileSize.y - 4 - 4 - 4 - 4), ColourUtility::GetColour(ColourUtility::ClientColour::InputCarot));
                }
            }

            std::stringstream s;
            
            for (size_t i = 0; i < format.size(); i++)
            {
                if (format[i] == '%')
                {
                    s << text;
                }
                else
                {
                    s << format[i];
                }
            }

            DrawUtils::anchoredText(ImVec2(pos.x + 4 + 2, pos.y + 4), ImVec2(tileSize.x - (4 + 4) - (2 + 2), tileSize.y - 4 - 4), s.str(), ImColor(255, 255, 255), ImVec2(0.0f, 0.5f));

            ImGui::SetCursorPos(pos);

            if (DrawUtils::mouseWithinRect(ImVec4(pos.x + 4, pos.y + 4, tileSize.x - 4 - 4, tileSize.y - 4 - 4)))
            {
                if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
                {
                    selected = this;
                }
            }

            if (selected == this)
                this->save();

            OnChange();

            if (DrawUtils::mouseWithinRect(ImVec4(pos.x, pos.y, tileSize.x, tileSize.y)))
                Module::descMod = description;

            return false;
        }

        void save()
        {
            geode::prelude::Mod::get()->setSavedValue<std::string>(id + "_value", text);
        }

        void load()
        {
            text = geode::prelude::Mod::get()->getSavedValue<std::string>(id + "_value", text);
        }

        void makeAndroid(CCMenu* menu, CCPoint pos)
        {
            auto label = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
            label->setAnchorPoint(ccp(0, 0.5f));
            label->setScale(0.575f);
            label->setPosition(pos - ccp(10, 0));
            label->limitLabelWidth(70, 0.575f, 0.1f);


            auto input = TextInput::create(100, name.c_str());
            input->setMaxCharCount(maxSize);
            input->getInputNode()->setAllowedChars(allowedChars);
            input->setPosition(pos + ccp(70, 0));
            input->setAnchorPoint(ccp(0, 0.5f));
            input->setDelegate(this);
            input->getInputNode()->setID("IGNOREBYPASSES"_spr);
            input->setString(text);


            menu->addChild(input);
            
            menu->addChild(label);
        }

        virtual void textChanged(CCTextInputNode* p0)
        {
            //log::info(p0->getString().c_str());
            text = p0->getString();

            this->save();
        }
};

class ColourModule : public Module, public TextInputDelegate
{
    public:
        static inline ColourModule* selected = nullptr;

        ccColor3B colour = ccc3(255, 255, 255);

        ColourModule(std::string name, std::string id, ccColor3B def)
        {
            colour = def;

            this->load();
        }

        bool Draw(ImVec2 tileSize)
        {
            ImVec2 pos = ImGui::GetCursorPos();

            auto v = name;
            name = "";            
            auto res = Module::Draw(tileSize);
            name = v;

            float height = tileSize.y - 10;

            DrawUtils::drawRect(DrawUtils::addImVec2(DrawUtils::addImVec2(pos, tileSize), ImVec2(-(5 + height), -(5 + height))), ImVec2(height, height), DrawUtils::imColorFromccColor3B(colour));

            return res;
        }

        void save()
        {
            geode::prelude::Mod::get()->setSavedValue<ccColor3B>(id + "_value", colour);
        }

        void load()
        {
            colour = geode::prelude::Mod::get()->getSavedValue<ccColor3B>(id + "_value", colour);
        }

        void makeAndroid(CCMenu* menu, CCPoint pos)
        {
            auto label = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
            label->setAnchorPoint(ccp(0, 0.5f));
            label->setScale(0.575f);
            label->setPosition(pos - ccp(10, 0));
            label->limitLabelWidth(70, 0.575f, 0.1f);

            //menu->addChild(input);
            
            menu->addChild(label);
        }
};

class InfoModule : public Module
{
    public:

        InfoModule(std::string n, std::string d)
        {
            name = n;
            description = d;
        }

        bool Draw(ImVec2 tileSize)
        {
            ImVec2 pos = ImGui::GetCursorPos();

            DrawUtils::drawRect(pos, tileSize, ColourUtility::GetColour(ColourUtility::ClientColour::WindowBG));
            DrawUtils::anchoredText(pos, tileSize, name.c_str(), ColourUtility::GetColour(ColourUtility::Text));

            if (DrawUtils::mouseWithinRect(ImVec4(pos.x, pos.y, tileSize.x, tileSize.y)))
                Module::descMod = description;

            return false;
        }

        void save()
        {
        }
        void load()
        {
        }
};

class SpeedhackTop : public InputModule
{
    public:
        static inline SpeedhackTop* instance = nullptr;

        SpeedhackTop()
        {
            id = "speedhack-top";
            text = "1.0";
            name = "Speed";
            description = "Speed modifier (*)";

            instance = this;

            this->load();
        }
};

class SpeedhackEnabled : public Module
{
    public:
        static inline SpeedhackEnabled* instance = nullptr;

        SpeedhackEnabled()
        {
            id = "speedhack-enabled";
            name = "Enabled";
            description = "Is the speedhack enabled?";

            instance = this;

            this->load();
        }
};

class SpeedhackMus : public Module
{
    public:
        static inline SpeedhackMus* instance = nullptr;

        SpeedhackMus()
        {
            id = "speedhack-mus";
            name = "Speedhack Music";
            description = "Speedhack all sound by your speed modifier";

            instance = this;

            this->load();
        }
};

class SpeedhackGameplay : public Module
{
    public:
        static inline SpeedhackGameplay* instance = nullptr;

        SpeedhackGameplay()
        {
            id = "speedhack-gameplay";
            name = "Gameplay Only";
            description = "Only enables the speedhack in gameplay and editor";

            instance = this;

            this->load();
        }
};

class StatusOpacity : public InputModule
{
    public:
        static inline StatusOpacity* instance = nullptr;

        StatusOpacity()
        {
            id = "status-op";
            text = "0.9";
            maxSize = 4;
            name = "Opacity";
            format = "Opacity: %";

            instance = this;

            this->load();
        }
};

class StatusScale : public InputModule
{
    public:
        static inline StatusScale* instance = nullptr;

        StatusScale()
        {
            id = "status-sc";
            text = "1.0";
            maxSize = 4;
            name = "Scale";
            format = "Scale: %";

            instance = this;

            this->load();
        }
};

class StatusMessage : public InputModule
{
    public:
        static inline StatusMessage* instance = nullptr;

        StatusMessage()
        {
            id = "status-msg";
            text = "";
            maxSize = 69;
            allowedChars = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
            format = "%";

            instance = this;

            this->load();
        }
};

class AltModuleLocation : public Module
{
    public:
        static inline AltModuleLocation* instance = nullptr;

        AltModuleLocation()
        {
            id = "alt-desc";
            name = "Alt Desc Location";
            description = "Places the module description in the bottom left corner of the screen like 3Hack";

            instance = this;

            this->load();
        }
};

class CompactMode : public Module
{
    public:
        static inline CompactMode* instance = nullptr;

        CompactMode()
        {
            id = "compact-mode";
            name = "Compact Mode";
            description = "Uses a smaller module size";

            instance = this;

            this->load();
        }
};
