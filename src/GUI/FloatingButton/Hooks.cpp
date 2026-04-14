#include "Hooks.hpp"
#include "FloatingUIManager.hpp"
#include "../AndroidUI.hpp"

$on_game(Loaded)
{
    FloatingUIManager::get()->updateSprites();
}