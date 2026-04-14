#include "../../Client/Module.hpp"
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class MenuSnow : public Module
{
    public:
        MODULE_SETUP(MenuSnow)
        {
            setID("menu-snow");
            setCategory("Universal");
            setDescription("Adds snow to the main menu");
            setDefaultEnabled(true);
        }
};

class DecemberOnly : public Module
{
    public:
        MODULE_SETUP(DecemberOnly)
        {
            setID("december-snow");
            setDefaultEnabled(true);
        }
};

SUBMIT_OPTION(MenuSnow, DecemberOnly);

class $modify (MenuLayer)
{
    virtual bool init()
    {
        if (!MenuLayer::init())
            return false;

        if (!MenuSnow::get()->getRealEnabled())
            return true;

        std::time_t currentTime = std::time(nullptr);
        std::tm* localTime = std::localtime(&currentTime);
        auto mon = localTime->tm_mon + 1;

        if (DecemberOnly::get()->getRealEnabled())
            if (!(mon == 12 || mon == 1))
                return true;

        auto snow = CCParticleSnow::create();
        snow->setID("snow"_spr);

        static bool first = false;

        if (!first)
        {
            first = true;
        }
        else
        {
            snow->update(15);
        }

        this->addChild(snow, 420);
        return true;
    }
};