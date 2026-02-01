#pragma once

#include <Geode/Geode.hpp>
#include "../CLanguage.hpp"
#include "../../GUI/PopupBase.hpp"

using namespace geode::prelude;

class LanguageContributorsUI : public PopupBase, public FLAlertLayerProtocol
{
    protected:
        std::string lang;
        CLanguage* language = nullptr;
        CCSprite* nodeBG;
        GJGroundLayer* ground;

        virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);

    public:
        static LanguageContributorsUI* create(std::string lang);

        SimplePlayer* getPlayer(CLanguageContributor obj);

        void onPlayerProfile(CCObject* sender);
        void onInfo(CCObject* sender);

        virtual bool setup();
        virtual void update(float dt);
};