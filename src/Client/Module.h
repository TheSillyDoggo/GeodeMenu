#include "ColourUtility.h"
#include "DrawUtils.h"

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

        Module(std::string n, std::string i, std::string d)
        {
            name = n;
            id = i;
            description = d;
            enabled = false;

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
        bool enabled;
        float value = 1.0f;

        virtual bool Draw(ImVec2 tileSize)
        {
            ImVec2 pos = ImGui::GetCursorPos();

            DrawUtils::drawRect(pos, tileSize, ColourUtility::GetColour(DrawUtils::mouseWithinRect(ImVec4(pos.x, pos.y, tileSize.x, tileSize.y)) ? (ImGui::IsMouseDown(ImGuiMouseButton_Left) ? ColourUtility::ClientColour::Pressed : ColourUtility::ClientColour::Hovered) : ColourUtility::ClientColour::WindowBG));

            if (moduleType == kModuleType::Float)
            {
                DrawUtils::drawRect(pos, ImVec2(tileSize.x * value, tileSize.y), ColourUtility::GetColour(ColourUtility::ClientColour::Accent));
            }

            DrawUtils::anchoredText(pos, tileSize, name.c_str(), ColourUtility::GetColour(enabled ? ColourUtility::Accent : ColourUtility::Text));

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

        }

        virtual void save()
        {
            geode::prelude::Mod::get()->setSavedValue<bool>(id + "_enabled", enabled);
        }

        virtual void load()
        {
            enabled = geode::prelude::Mod::get()->getSavedValue<bool>(id + "_enabled", false);
        }
};

class InputModule : public Module
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
            description = "Speed modifier (*)";

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
