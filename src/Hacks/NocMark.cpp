#include <Geode/Geode.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify(EditLevelLayer) {

    void onShare(CCObject* sender) {

        if (Module::get("noc-hack")->enabled)
        {
            this->m_level->m_originalLevel = 0;
        }
        
        EditLevelLayer::onShare(sender);
    }

};