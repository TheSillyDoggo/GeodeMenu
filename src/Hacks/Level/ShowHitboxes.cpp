#include "../../Client/Module.hpp"
#include <Geode/modify/PlayLayer.hpp>
#include "HitboxUtils.hpp"

using namespace geode::prelude;

class ShowHitboxes : public Module
{
    public:
        MODULE_SETUP(ShowHitboxes)
        {
            setName("Show Hitboxes");
            setID("show-hitboxes");
            setCategory("Level");
            setDescription("a");
            setSafeModeTrigger(SafeModeTrigger::Attempt);
        }
};

class ShowHitboxesOnDeath : public Module
{
    public:
        MODULE_SETUP(ShowHitboxesOnDeath)
        {
            setName("Show Hitboxes On Death");
            setID("show-hitboxes-on-death");
            setCategory("Level");
            setDescription("a");
        }
};

SUBMIT_HACK(ShowHitboxes);

SUBMIT_HACK(ShowHitboxesOnDeath);

class $modify (PlayLayer)
{
    virtual void postUpdate(float dt)
    {
        PlayLayer::postUpdate(dt);

        m_debugDrawNode->setVisible(HitboxUtils::shouldHitboxesBeVisible() || (ShowHitboxesOnDeath::get()->getRealEnabled() ? m_player1->m_isDead : false) || ShowHitboxes::get()->getRealEnabled());

        if (m_debugDrawNode->isVisible() != HitboxUtils::shouldHitboxesBeVisible())
            updateDebugDraw();
    }
};