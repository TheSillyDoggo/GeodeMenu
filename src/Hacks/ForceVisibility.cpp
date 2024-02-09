/*#include <Geode/Geode.hpp>
#include <Geode/modify/CCNode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (MenuLayer)
{
    virtual bool init()
    {
        if (!MenuLayer::init())
            return false;

        GJAccountManager::get()->m_accountID = 2795;
        GJAccountManager::get()->m_username = "ViPriN";

        GameManager::get()->m_playerColor = 11;
        GameManager::get()->m_playerColor2 = 13;
        GameManager::get()->m_playerGlowColor = 47;

        GameManager::get()->m_playerFrame = 133;
        GameManager::get()->m_playerShip = 42;
        GameManager::get()->m_playerBall = 32;
        GameManager::get()->m_playerBird = 29;
        GameManager::get()->m_playerDart = 30;
        GameManager::get()->m_playerRobot = 16;
        GameManager::get()->m_playerSpider = 11;
        GameManager::get()->m_playerJetpack = 5;

        return true;
    }
};*/