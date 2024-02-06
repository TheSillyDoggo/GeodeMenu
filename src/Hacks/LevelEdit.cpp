#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PauseLayer)
{
    virtual TodoReturn customSetup()
    {
        if (Client::GetModuleEnabled("level-edit"))
            PlayLayer::get()->m_level->m_levelType = GJLevelType::Editor;

        PauseLayer::customSetup();
    }
};