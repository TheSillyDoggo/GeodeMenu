#include "../../Client/InputModule.hpp"
#include <Geode/modify/GameObject.hpp>
#include "../Utils/PlayLayer.hpp"

using namespace geode::prelude;

class HitboxMultipler : public Module
{
    public:
        MODULE_SETUP(HitboxMultipler)
        {
            setName("Hitbox Multipler");
            setID("hitbox-multiplier");
            setCategory("Level");
            setDescription("Change the size of hitboxes");
            setSafeModeTrigger(SafeModeTrigger::Attempt);
        }
};

class MultiplierPlayer : public InputModule
{
    public:
        MODULE_SETUP(MultiplierPlayer)
        {
            setName("Player");
            setID("hitbox-multiplier/player");
            setDescription("");

            setDefaultString("1");
            setPlaceholderString("Multiplier");

            setStringFilter("1234567890.");
            setMaxCharCount(5);
        }
};

class MultiplierSolid : public InputModule
{
    public:
        MODULE_SETUP(MultiplierSolid)
        {
            setName("Solids");
            setID("hitbox-multiplier/solid");
            setDescription("");

            setDefaultString("1");
            setPlaceholderString("Multiplier");

            setStringFilter("1234567890.");
            setMaxCharCount(5);
        }
};

class MultiplierHazard : public InputModule
{
    public:
        MODULE_SETUP(MultiplierHazard)
        {
            setName("Hazards");
            setID("hitbox-multiplier/hazard");
            setDescription("");

            setDefaultString("1");
            setPlaceholderString("Multiplier");

            setStringFilter("1234567890.");
            setMaxCharCount(5);
        }
};

SUBMIT_HACK(HitboxMultipler);
SUBMIT_OPTION(HitboxMultipler, MultiplierPlayer);
SUBMIT_OPTION(HitboxMultipler, MultiplierSolid);
SUBMIT_OPTION(HitboxMultipler, MultiplierHazard);

class $modify (GameObject)
{
    virtual cocos2d::CCRect getObjectRect(float p0, float p1)
    {
        if (HitboxMultipler::get()->getRealEnabled())
        {
            if (typeinfo_cast<PlayerObject*>(this))
            {
                p0 *= MultiplierPlayer::get()->getStringFloat();
                p1 *= p0 *= MultiplierPlayer::get()->getStringFloat();
            }
            else
            {
                if (m_objectType == GameObjectType::Solid || m_objectType == GameObjectType::Slope)
                {
                    p0 *= MultiplierSolid::get()->getStringFloat();
                    p1 *= MultiplierSolid::get()->getStringFloat();
                }
                
                if (m_objectType == GameObjectType::Hazard || m_objectType == GameObjectType::AnimatedHazard)
                {
                    p0 *= MultiplierHazard::get()->getStringFloat();
                    p1 *= MultiplierHazard::get()->getStringFloat();
                }
            }   
        }

        return GameObject::getObjectRect(p0, p1);
    }
};