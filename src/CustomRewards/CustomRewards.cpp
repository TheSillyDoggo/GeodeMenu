#include "CustomRewards.hpp"
#include "../GUI/BetterAlertLayer.hpp"
#include "UI/CustomRewardSetupUI.hpp"

void CreateCustomReward::onClick()
{
    CustomRewardSetupUI::create()->show();

    if (!Mod::get()->setSavedValue<bool>("create-custom-reward/shown-first-use-warning", true))
    {
        BetterAlertLayer::createWithLocalisation("names/create-custom-reward", "create-custom-reward/first-use-warning", "ui/ok-button")->show();
    }
}