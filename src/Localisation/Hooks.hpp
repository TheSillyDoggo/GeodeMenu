#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/modify/GameManager.hpp>

class $modify (LocalisationLoadingLayer, LoadingLayer)
{
    void loadAssets();
};

class $modify (LocalisationGameManager, GameManager)
{
    void reloadAllStep5();
};