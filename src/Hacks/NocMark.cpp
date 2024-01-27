#include <Geode/Geode.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify(EditLevelLayer) {

    void onShare(CCObject* sender) {

        if (Client::GetModuleEnabled("noc-hack"))
        {
            this->m_level->m_originalLevel = true;
        }
        
        EditLevelLayer::onShare(sender);
    }

};