#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class LanguageContributorsUI : public geode::Popup<>
{
    protected:
        std::string lang;
        matjson::Value json;
        CCSprite* nodeBG;
        GJGroundLayer* ground;

    public:
        static LanguageContributorsUI* create(std::string lang);

        SimplePlayer* getPlayer(matjson::Value obj);

        void onPlayerProfile(CCObject* sender);

        virtual bool setup();
        virtual void update(float dt);
};