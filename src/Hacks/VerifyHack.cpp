#include <Geode/Geode.hpp>
#include <Geode/modify/ShareLevelLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

#ifdef QOLMOD_VERIFYHACK

class $modify(ShareLevelLayer)
{
    struct Fields {
        GJGameLevel* m_level = nullptr;
    };

    bool init(GJGameLevel* p0)
    {
        m_fields->m_level = p0;

        return ShareLevelLayer::init(p0);
    }

    void onShare(cocos2d::CCObject* sender)
    {
        #ifdef QOLMOD_GOODVERIFYHACK
        if (Client::GetModuleEnabled("verify-hack"))
        {
            auto pop = UploadPopup::create(m_fields->m_level);
            CCScene::get()->addChild(pop, CCScene::get()->getHighestChildZ() + 1);

            this->setKeypadEnabled(false);
            this->removeFromParent();

            return;
        }

        ShareLevelLayer::onShare(sender);
        #else
        auto level = m_fields->m_level;
		auto p1 = level->m_isVerifiedRaw;
		auto p2 = level->m_isVerified.value();

        if (Client::GetModuleEnabled("verify-hack"))
        {
		    level->m_isVerifiedRaw = true;
		    level->m_isVerified = true;
        }

		ShareLevelLayer::onShare(sender);

		level->m_isVerifiedRaw = p1;
		level->m_isVerified = p2;
        #endif
    }
};

#endif