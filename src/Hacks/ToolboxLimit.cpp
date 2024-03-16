#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/CCArray.hpp>
#include <Geode/modify/CCDictionary.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

bool vascsdaf = false;

class $modify (CCArray)
{
    unsigned int count()
    {
        if (Client::GetModuleEnabled("custom-obj-limit") && vascsdaf)
        {
            auto v = CCArray::count();

            return v == 0 ? 0 : 1;
        }
        else
        {
            return CCArray::count();
        }
    }
};

class $modify (CCDictionary)
{
    unsigned int count()
    {
        if (Client::GetModuleEnabled("custom-obj-limit") && vascsdaf)
        {
            return 1;
        }
        else
        {
            return CCDictionary::count();
        }
    }
};

class $modify (EditorUI)
{
    void onNewCustomItem(cocos2d::CCObject* sender)
    {
        vascsdaf = true;

        EditorUI::onNewCustomItem(sender);

        vascsdaf = false;

        EditorUI::reloadCustomItems();
    }
};