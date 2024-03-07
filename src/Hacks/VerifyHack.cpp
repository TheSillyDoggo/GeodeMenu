#include <Geode/Geode.hpp>
#include <Geode/modify/ShareLevelLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify(ShareLevelLayer)
{
    GJGameLevel* m_level = nullptr;

    bool init(GJGameLevel* p0)
    {
        m_fields->m_level = p0;

        return ShareLevelLayer::init(p0);
    }

    void onShare(cocos2d::CCObject* sender)
    {
        if (Client::GetModuleEnabled("verify-hack"))
        {
            auto pop = UploadPopup::create(m_fields->m_level);
            CCScene::get()->addChild(pop, CCScene::get()->getHighestChildZ() + 1);

            this->setKeypadEnabled(false);
            this->removeFromParent();

            return;
        }

        ShareLevelLayer::onShare(sender);
    }
};