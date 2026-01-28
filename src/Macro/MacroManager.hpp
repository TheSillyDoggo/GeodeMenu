#pragma once

#include <Geode/Geode.hpp>

struct MacroInput
{
    unsigned int tick;
    double levelTime;
    double cbfOffset;
    bool down;
    bool player2;
    unsigned int uniqueID;
};

enum class MacroState
{
    None = 0,
    Recording = 1,
    Playback = 2,
};

class MacroManager
{
    protected:
        std::vector<MacroInput> inputs = {};
        std::unordered_map<unsigned int, bool> inputsPlayed = {};
        MacroState state = MacroState::None;
    
    public:
        static MacroManager* get();

        MacroState getState();
        void setState(MacroState state);

        void storeInput(bool player2, bool down, unsigned int tick, double levelTime, double cbfOffset);

        std::vector<MacroInput> getInputs();
        std::vector<MacroInput> getUnplayedInputs();

        void clearInputs();
        void removeInput(unsigned int uniqueID);

        void markAllUnplayed();
        void markPlayed(unsigned int uniqueID, bool played);
        bool hasPlayed(unsigned int uniqueID);
};