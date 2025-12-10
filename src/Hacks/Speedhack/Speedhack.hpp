#pragma once

#include <vector>

struct SpeedhackPreset
{
    float value = 0.0f;
};

class Speedhack
{
    friend class SpeedhackScheduler;

    protected:
        bool enabled = false;
        bool musicEnabled = true;
        bool gameplayEnabled = false;
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