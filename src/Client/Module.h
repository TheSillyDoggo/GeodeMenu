#pragma once

#include "ColourUtility.h"
#include "DrawUtils.h"


#include <Geode/ui/TextInput.hpp>

#define public_cast(value, member) [](auto* v) { \
	class FriendClass__; \
	using T = std::remove_pointer<decltype(v)>::type; \
	class FriendeeClass__: public T { \
	protected: \
		friend FriendClass__; \
	}; \
	class FriendClass__ { \
	public: \
		auto& get(FriendeeClass__* v) { return v->member; } \
	} c; \
	return c.get(reinterpret_cast<FriendeeClass__*>(v)); \
}(value)


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

        std::vector<Module*> options = {};
        std::vector<geode::Hook*> hooks = {};

        std::string name;
        std::string id;
        std::string description;
        std::string alert;
        std::string inCompAlert;
        bool enabled;
        bool useAlert;
        bool onceAlert;
        bool isInComp;

        bool vAlert;

        bool def;
        float value = 1.0f;

        ModuleChangeDelegate* delegate = nullptr;

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
            enabled = _def;
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
            enabled = geode::prelude::Mod::get()->getSavedValue<bool>(id + "_enabled", enabled);
            save();
        }

        void onInfoAndroid(CCObject* sender);
        void onOptionsAndroid(CCObject* sender);
        void onToggleAndroid(CCObject* sender);

        void setIncompatible(std::string str);
        
        virtual void makeAndroid(CCNode* menu, CCPoint pos);
};

class InputModule : public Module, public TextInputDelegate
{
    public:
        float speedhackV = 1.0f;
        static inline InputModule* selected = nullptr;

        std::string text = "";
        std::string format = "%";
        std::string allowedChars = "0987654321.";

        std::string lastCheckedText = "";
        float floatValue = 1;
        int intValue = 0;

        int maxSize = 5;

        TextInput* inputField = nullptr;

        InputModule()
        {
            this->load();
        }

        InputModule(std::string name, std::string id, std::string def)
        {
            this->name = name;
            this->id = id;
            this->text = def;

            this->load();
        }

        void updateValue();

        float getFloatValue();
        int getIntValue();

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
            
            if (this->text.size() > 12)
                this->text = this->text.substr(0, 12);
        }

        void makeAndroid(CCNode* menu, CCPoint pos)
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

            this->inputField = input;
        }

        virtual void textChanged(CCTextInputNode* p0)
        {
            //log::info(p0->getString().c_str());
            text = p0->getString();

            this->save();

            if (delegate)
                delegate->onModuleChanged(false);
        }
};

class ColourDelegate : public geode::ColorPickPopupDelegate
{
    public:
        Module* mod = nullptr;

        virtual void updateColor(cocos2d::ccColor4B const& color);
};

class ColourModule : public Module
{
    public:
        static inline ColourModule* selected = nullptr;

        ccColor3B colour = ccc3(255, 255, 255);
        ccColor3B def = ccc3(255, 255, 255);

        ColourDelegate* del = nullptr;

        CCSprite* btnSpr = nullptr;

        ColourModule(std::string name, std::string id, ccColor3B def)
        {
            this->name = name;
            this->id = id;
            colour = def;
            this->def = def;

            this->load();
        }

        bool Draw(ImVec2 tileSize)
        {
            ImVec2 pos = ImGui::GetCursorPos();

            auto v = name;
            //name = "";            
            auto res = Module::Draw(tileSize);
            //name = v;

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

        void onPickColourAndroid(CCObject* sender)
        {
            auto v = as<ColourModule*>(as<CCNode*>(sender)->getUserData());

            if (!v->del)
            {
                v->del = new ColourDelegate();
                v->del->mod = v;
            }

            auto men = geode::ColorPickPopup::create(v->colour);

            //public_cast(men, m_originalColor) = ccc4BFromccc4F(ccc4FFromccc3B(v->def));

            //if (v->colour != v->def)
                //public_cast(men, m_resetBtn)->setVisible(true);

            men->setDelegate(v->del);
            men->show();
        }

        void makeAndroid(CCNode* menu, CCPoint pos)
        {
            auto label = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
            label->setAnchorPoint(ccp(0, 0.5f));
            label->setScale(0.575f);
            label->setPosition(pos + ccp(-15, 0));
            label->limitLabelWidth(110, 0.575f, 0.1f);

            btnSpr = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
            btnSpr->setColor(colour);
            btnSpr->setScale(0.75f);
            auto btn = CCMenuItemSpriteExtra::create(btnSpr, menu, menu_selector(ColourModule::onPickColourAndroid));

            btn->setUserData(this);
            btn->setPosition(pos + ccp(135, 0) + ccp(-15, 0));
            menu->addChild(btn);

            //menu->addChild(input);
            
            menu->addChild(label);
        }
};

class DropdownModule : public Module
{
    public:
        std::vector<std::string> content = {};
        int index = 0;

        DropdownModule(std::vector<std::string> stuff, std::string id, int def)
        {
            this->name = name;
            this->id = id;
            index = def;
            this->content = stuff;

            this->load();
        }

        void save()
        {
            geode::prelude::Mod::get()->setSavedValue<int>(id + "_index", index);
        }

        void load()
        {
            index = geode::prelude::Mod::get()->getSavedValue<int>(id + "_index", index);
        }

        void makeAndroid(CCNode* menu, CCPoint pos);

        void onDropdownSelectionChanged(CCObject* sender);
};

class SliderModule : public Module
{
    public:
        Slider* slider = nullptr;
        float value = 1.0f;

        SliderModule(std::string name, std::string id, float def)
        {
            this->name = name;
            this->id = id;
            value = def;

            this->load();
        }

        void save()
        {
            geode::prelude::Mod::get()->setSavedValue<float>(id + "_value", value);
        }

        void load()
        {
            value = geode::prelude::Mod::get()->getSavedValue<float>(id + "_value", value);
        }

        void onSliderChanged(CCObject* sender)
        {
            auto x = as<SliderModule*>(as<SliderThumb*>(sender)->getUserData());

            x->value = as<SliderThumb*>(sender)->getValue();
            
            x->save();
        }

        void makeAndroid(CCNode* menu, CCPoint pos)
        {
            auto label = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
            label->setAnchorPoint(ccp(0, 0.5f));
            label->setScale(0.575f);
            label->setPosition(pos + ccp(-15, 0));
            label->limitLabelWidth(80, 0.575f, 0.1f);

            slider = Slider::create(menu, menu_selector(SliderModule::onSliderChanged));
            slider->getThumb()->setValue(value);
            slider->updateBar();
            slider->getThumb()->setUserData(this);

            slider->setAnchorPoint(ccp(0, 0));
            slider->setScale(0.5f);
            slider->setScaleX(0.35f);
            slider->setPosition(pos + ccp(135, 0) + ccp(-15 - 14, 0));
            
            as<CCSprite*>(slider->getThumb()->getChildren()->objectAtIndex(0))->setScaleX(0.5f / 0.35f);
            as<CCSprite*>(slider->getThumb()->getChildren()->objectAtIndex(1))->setScaleX(0.5f / 0.35f);
            
            menu->addChild(slider);
            menu->addChild(label);
        }
};

class TransCustomizerModule : public Module
{
    public:
        static inline CCLabelBMFont* label = nullptr;
        static inline CCMenuItemSpriteExtra* left = nullptr;
        static inline CCMenuItemSpriteExtra* right = nullptr;

        static inline const std::vector<std::string> transNames = {"Fade", "Cross Fade", "Fade Bottom Left", "Fade Top Right", "Fade Up", "Fade Down", "Flip Angular", "Flip X", "Flip Y", "Zoom Flip Angular", "Zoom Flip X", "Zoom Flip Y", "Jump Zoom", "Move In Top", "Move In Bottom", "Move In Left", "Move In Right", "Rotate Zoom", "Shrink Grow", "Slide In Top", "Slide In Bottom", "Slide In Left", "Slide In Right", "Split Rows", "Split Columns", "Tiles"};

        TransCustomizerModule()
        {

        }

        void save()
        {

        }

        void load()
        {

        }

        void makeAndroid(CCNode* menu, CCPoint pos);

        void onLeft(CCObject*);
        void onRight(CCObject*);
};

class SetValueModule : public Module
{
    public:
        TextInput* inp;
        TextInput* inpX;
        TextInput* inpY;
        std::string allowedChars = "1234567890-.";

        SetValueModule(std::string name, std::string id)
        {
            this->name = name;
            this->id = id;
        }

        void save()
        {

        }

        void load()
        {

        }

        void makeAndroid(CCNode* menu, CCPoint pos);
        void onSet(CCObject* sender);
        void onScaleToggle(CCObject* sender);
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
            enabled = true;

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

class SpeedhackTransFix : public Module
{
    public:
        static inline SpeedhackTransFix* instance = nullptr;

        SpeedhackTransFix()
        {
            id = "trans-fix";
            name = "Transition Fix";
            description = "Scene Transitions run at normal speed instead of your speedhack value";
            enabled = true;

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
