#include "Hooks.hpp"
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"

void LocalisationLoadingLayer::loadAssets()
{
    LoadingLayer::loadAssets();

    CCLabelTTFCache::get()->preloadTextures();
}

void LocalisationGameManager::reloadAllStep5()
{
    GameManager::reloadAllStep5();

    CCLabelTTFCache::get()->flushTextures();
}