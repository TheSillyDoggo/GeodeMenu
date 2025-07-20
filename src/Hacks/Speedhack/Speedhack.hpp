#pragma once

class Speedhack
{
    protected:
        bool enabled = false;
        bool musicEnabled = true;
        bool gameplayEnabled = false;
        std::string str = "";
        float value = 1.0f;

    public:
        static Speedhack* get();

        void setup();

        bool getEnabled();
        void setEnabled(bool value);

        bool getMusicEnabled();
        void setMusicEnabled(bool enabled);

        bool getGameplayEnabled();
        void setGameplayEnabled(bool enabled);

        FMOD::ChannelGroup* getMasterChannel();

        std::string getText();
        void setText(std::string text);

        bool gameplayOnlyCheck();
        float getRealValue();
        float getValue();
};