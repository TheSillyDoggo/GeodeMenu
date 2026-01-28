#include "MacroManager.hpp"

MacroManager* MacroManager::get()
{
    static MacroManager* instance = nullptr;

    if (!instance)
        instance = new MacroManager();

    return instance;
}

void MacroManager::storeInput(bool player2, bool down, unsigned int tick, double levelTime, double cbfOffset)
{
    static unsigned int uniqueID = 0;
    uniqueID++;

    MacroInput input;
    input.uniqueID = uniqueID;
    input.down = down;
    input.player2 = player2;
    input.tick = tick;
    input.levelTime = levelTime;
    input.cbfOffset = cbfOffset;

    inputs.push_back(input);
}

void MacroManager::markPlayed(unsigned int uniqueID, bool played)
{
    inputsPlayed[uniqueID] = played;
}

void MacroManager::markAllUnplayed()
{
    inputsPlayed.clear();
}

bool MacroManager::hasPlayed(unsigned int uniqueID)
{
    if (inputsPlayed.contains(uniqueID))
        return inputsPlayed[uniqueID];

    return false;
}

void MacroManager::clearInputs()
{
    inputs.clear();
}

void MacroManager::removeInput(unsigned int uniqueID)
{

}

std::vector<MacroInput> MacroManager::getInputs()
{
    return inputs;
}

std::vector<MacroInput> MacroManager::getUnplayedInputs()
{
    std::vector<MacroInput> inps = {};

    for (auto input : inputs)
    {
        if (!hasPlayed(input.uniqueID))
            inps.push_back(input);
    }
    
    return inps;
}

MacroState MacroManager::getState()
{
    return state;
}

void MacroManager::setState(MacroState state)
{
    this->state = state;
}