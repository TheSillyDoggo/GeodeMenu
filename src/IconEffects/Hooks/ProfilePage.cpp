/*#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/modify/SimplePlayer.hpp>
#include "../IconicPlayerHook.hpp"

using namespace geode::prelude;

class $modify (SimplePlayerExt, SimplePlayer)
{
    struct Fields
    {
        IconType type;
    };

    void updatePlayerFrame(int id, IconType type)
    {
        SimplePlayer::updatePlayerFrame(id, type);

        m_fields->type = type;
    }

    IconicGamemodeType getType()
    {
        switch ((int)m_fields->type)
        {
            default:
                return IconicGamemodeType::Cube;

            case (int)IconType::Ship:
                return IconicGamemodeType::Ship;

            case (int)IconType::Ball:
                return IconicGamemodeType::Ball;

            case (int)IconType::Ufo:
                return IconicGamemodeType::Bird;

            case (int)IconType::Wave:
                return IconicGamemodeType::Dart;

            case (int)IconType::Robot:
                return IconicGamemodeType::Robot;

            case (int)IconType::Spider:
                return IconicGamemodeType::Spider;

            case (int)IconType::Swing:
                return IconicGamemodeType::Swing;

            case (int)IconType::Jetpack:
                return IconicGamemodeType::Jetpack;
        }
    }
};

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
        iconic->setGamemode(static_cast<SimplePlayerExt*>(simple)->getType(), false);
        simple->addChild(iconic);
    }

    void toggleShip(cocos2d::CCObject* sender)
    {
        ProfilePage::toggleShip(sender);

        if (auto hk = static_cast<CCNode*>(sender)->getChildByType<SimplePlayer>(0)->getChildByType<IconicPlayerHook>(0))
        {
            hk->setGamemode(static_cast<SimplePlayerExt*>(static_cast<CCNode*>(sender)->getChildByType<SimplePlayer>(0))->getType(), false);
        }
    }
};*/