/*#include <Geode/Geode.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (CCKeyboardDispatcher)
{
    bool dispatchKeyboardMSG(enumKeyCodes key, bool p1, bool p2)
    {
        //log::info("k");

        if (key == enumKeyCodes::KEY_L)
        {
            log::info("RobTop");

            GJAccountManager::get()->m_username = "RobTop";
            //GJAccountManager::get()->m_accountID = 16;
        }
        else if (key == enumKeyCodes::KEY_K)
        {
            log::info("TheSillyDoggo");

            GJAccountManager::get()->m_username = "TheSillyDoggo";
            //GJAccountManager::get()->m_accountID = 16778880;
        }

        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, p1, p2);
    }
};*/