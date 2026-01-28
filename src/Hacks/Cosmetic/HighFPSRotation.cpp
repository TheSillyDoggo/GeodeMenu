#include <Module.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class HighFPSRotation : public Module
{
    public:
        MODULE_SETUP(HighFPSRotation)
        {
            setID("high-fps-rotation");
            setCategory("Cosmetic");
        }
};

SUBMIT_HACK(HighFPSRotation);

class $modify (PlayerObject)
{
    void updateRotation(float dt)
    {
        if (HighFPSRotation::get()->getRealEnabled())
        {
            if (m_isShip || m_isDart || m_isSwing || m_isBird)
            {
                dt *= 6.7f;
            }
        }

        PlayerObject::updateRotation(dt);
    }
};