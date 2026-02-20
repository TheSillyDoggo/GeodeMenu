#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include "../IconicPlayerHook.hpp"

using namespace geode::prelude;

class $modify (IconicProfilePage, ProfilePage)
{
    void loadPageFromUserInfo(GJUserScore* score)
    {
        ProfilePage::loadPageFromUserInfo(score);

        if (m_ownProfile)
        {
            if (auto player = m_mainLayer->getChildByID("player-menu"))
            {
                for (auto child : CCArrayExt<CCNode*>(player->getChildren()))
                {
                    handlePlayer(child->getChildByType<SimplePlayer>(0));
                }
            }
        }
    }

    void handlePlayer(SimplePlayer* simple)
    {
        if (!simple)
            return;

        auto iconic = IconicPlayerHook::create(simple);
        simple->addChild(iconic);
    }
};