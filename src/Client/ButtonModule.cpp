#include "ButtonModule.hpp"
#include "ButtonModuleNode.hpp"
#include <LocalisationManager.hpp>
#include <NotificationManager.hpp>

ModuleNode* ButtonModule::getNode()
{
    return ButtonModuleNode::create(this);
}

void ButtonModule::onKeybindActivated(KeyState state)
{
    onClick();
    NotificationManager::get()->notifyToast(getNotificationString());
}

std::string ButtonModule::getNotificationString()
{
    return utils::string::replace(LocalisationManager::get()->getLocalisedString("ui/notification-button-pressed"), "%s", getName());
}