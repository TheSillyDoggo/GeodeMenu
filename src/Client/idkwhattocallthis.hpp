#pragma once

#include "Client.h"

#include "Types/InputModule.hpp"

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

        static float getAdjustedValue()
        {
            float v = 1;

            if (SpeedhackTop::instance)
            {
                if (SpeedhackEnabled::instance->enabled)
                {
                    v = SpeedhackTop::instance->getFloatValue();

                    if (SpeedhackGameplay::instance->enabled)
                        if (!(PlayLayer::get() || GameManager::sharedState()->getEditorLayer())) { v = 1.0f; }

                    v /= CCDirector::get()->getScheduler()->getTimeScale();
                }
            }

            return v;
        }
};