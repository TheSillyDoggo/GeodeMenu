#ifndef __APPLE__

#include <Geode/Geode.hpp>
#include <Geode/modify/SecretLayer2.hpp>
#include "Client/AndroidBall.h"
#include <random>

using namespace geode::prelude;

class $modify (SecretLayer2)
{
    std::string getColonThreeLabel()
    {
        if (!AndroidBall::get()->isColonThreeEnabled())
            return "No more :3 :(";

        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> distr(1, 9);

        switch (distr(gen))
        {
            default:
                return "";

            case 1:
                return ":3-olmod has been enabled :3";
            case 2:
                return fmt::format("{} the cute one :3", GameManager::get()->m_playerName);
            case 3:
                return "meow";
            case 4:
                return "purr";
            case 5:
                return "mrrow";
            case 6:
                return ":3";
            case 7:
                return ";3";
            case 8:
                return "Ultimate catgirl mode enabled :3";
            case 9:
                return "you're cute :3";
        }
    }

    void onSubmit(cocos2d::CCObject* sender)
    {
        bool inp = false;

        if (auto input = getChildOfType<CCTextInputNode>(this, 0))
        {
            if (input->getString() != ":3")
                return SecretLayer2::onSubmit(sender);

            inp = true;
            
            if (auto ball = AndroidBall::get())
                ball->setColonThreeEnabled();
        }

        SecretLayer2::onSubmit(sender);

        updateMessageLabel(getColonThreeLabel());
    }
};

#endif