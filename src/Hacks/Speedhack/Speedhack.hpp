#pragma once

#include <vector>
#include "../../Keybinds/KeyConfigStruct.hpp"
#include "../../Client/Module.hpp"
#include "../../SafeMode/SafeMode.hpp"

struct SpeedhackPreset
{
    float value = 0.0f;
    KeyConfigStruct keyConfig = {};
};

class SpeedhackEnabled : public Module
{
    public:
        MODULE_SETUP(SpeedhackEnabled)
        {
            setName("Enabled");
            setID("speedhack/enabled");
            setDescription("Is the speedhack enabled?");
        }

        virtual void onToggle()
        {
            SafeMode::get()->onSpeedhackChanged();
        }
};

class SpeedhackMusic : public Module
{
    public:
        MODULE_SETUP(SpeedhackMusic)
        {
            setName("Speedhack Music");
            setID("speedhack/music");
            setDescription("Speedhack all sound by your speed modifier");
            setDefaultEnabled(true);
        }

        virtual void onToggle()
        {
            SafeMode::get()->onSpeedhackChanged();
        }
};

class SpeedhackGameplay : public Module
{
    public:
        MODULE_SETUP(SpeedhackGameplay)
        {
            setName("Speedhack Gameplay");
            setID("speedhack/gameplay");
            setDescription("Only enables the speedhack in gameplay and editor");
        }

        virtual void onToggle()
        {
            SafeMode::get()->onSpeedhackChanged();
        }
};

class Speedhack
{
    friend class SpeedhackScheduler;

    protected:
        std::string str = "";
        float value = 1.0f;
        std::vector<SpeedhackPreset> presets = {};
        float realDeltatime = 0;

    public:
        static Speedhack* get();

        void setup();

        bool getEnabled();
        void setEnabled(bool value);

        bool getMusicEnabled();
        void setMusicEnabled(bool enabled);

        bool getGameplayEnabled();
        void setGameplayEnabled(bool enabled);

        void loadPresets();
        void savePresets();

        void setPresets(std::vector<SpeedhackPreset> presets);
        std::vector<SpeedhackPreset> getPresets();
        bool hasPreset(float value);

        FMOD::ChannelGroup* getMasterChannel();

        std::string getText();
        void setText(std::string text);

        float getRealDeltaTime();

        bool gameplayOnlyCheck();
        float getRealValue();
        float getValue();
};