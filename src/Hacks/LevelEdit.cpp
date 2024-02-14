#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PauseLayer)
{
    virtual TodoReturn customSetup()
    {
        auto a = PlayLayer::get()->m_level->m_levelType;

        if (Client::GetModuleEnabled("level-edit"))
            PlayLayer::get()->m_level->m_levelType = GJLevelType::Editor;

        PauseLayer::customSetup();

        PlayLayer::get()->m_level->m_levelType = a;
    }

    void onEdit(cocos2d::CCObject* sender)
    {
        auto a = PlayLayer::get()->m_level->m_levelType;

        if (Client::GetModuleEnabled("level-edit"))
            PlayLayer::get()->m_level->m_levelType = GJLevelType::Editor;

        PauseLayer::onEdit(sender);

        PlayLayer::get()->m_level->m_levelType = a;
    }
};