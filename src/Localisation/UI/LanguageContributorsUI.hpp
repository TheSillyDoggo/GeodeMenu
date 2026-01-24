#pragma once

#include <Geode/Geode.hpp>
#include "../CLanguage.hpp"
#include "../../GUI/PopupBase.hpp"

using namespace geode::prelude;

class LanguageContributorsUI : public PopupBase
{
    protected:
        std::string lang;
        CLanguage* language = nullptr;
        CCSprite* nodeBG;
        GJGroundLayer* ground;

    public:
        static LanguageContributorsUI* create(std::string lang);

        SimplePlayer* getPlayer(CLanguageContributor obj);

        void onPlayerProfile(CCObject* sender);

        virtual bool setup();
        virtual void update(float dt);
};