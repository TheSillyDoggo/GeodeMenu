#include "../../Client/Module.hpp"
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class DecemberSnow : public Module
{
    public:
        MODULE_SETUP(DecemberSnow)
        {
            setName("December Snow");
            setID("december-snow");
            setCategory("Universal");
            setDescription("Adds snow to the main menu during <cc>december</c>.");
            setDefaultEnabled(true);
        }
};

SUBMIT_HACK(DecemberSnow)

class $modify (MenuLayer)
{
    virtual bool init()
    {
        if (!MenuLayer::init())
            return false;

        if (!DecemberSnow::get()->getRealEnabled())
            return true;

        std::time_t currentTime = std::time(nullptr);
        std::tm* localTime = std::localtime(&currentTime);

        if (localTime->tm_mon + 1 == 12)
        {
            auto snow = CCParticleSnow::create();
            snow->setID("snow"_spr);

            this->addChild(snow, 420);
        }

        return true;
    }
};