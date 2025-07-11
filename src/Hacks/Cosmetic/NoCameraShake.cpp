#include "../../Client/Module.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

class NoCameraShake : public Module
{
    public:
        MODULE_SETUP(NoCameraShake)
        {
            setName("No Camera Shake");
            setID("no-shake");
            setCategory("Cosmetic");
            setDescription("Disables camera shaking");
        }
};

SUBMIT_HACK(NoCameraShake);

class $modify (GJBaseGameLayer)
{
    void updateCamera(float delta)
    {
        if (NoCameraShake::get()->getRealEnabled())
        {
            m_gameState.m_cameraShakeFactor = 0;
            m_gameState.m_cameraShakeEnabled = false;
        }

        GJBaseGameLayer::updateCamera(delta);
    }
};