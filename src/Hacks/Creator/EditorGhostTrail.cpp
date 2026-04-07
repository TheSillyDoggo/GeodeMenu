/*#include "../../Client/Module.hpp"
#include <Geode/modify/HardStreak.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class EditorGhostTrail : public Module
{
    public:
        MODULE_SETUP(EditorGhostTrail)
        {
            setID("editor-ghost-trail");
            setCategory("Creator");
        }
};

SUBMIT_HACK(EditorGhostTrail);

class $modify (PlayerObject)
{
    static void onModify(auto& self)
    {
        (void)self.setHookPriority("PlayerObject::toggleGhostEffect", Priority::Stub);
    }

    void toggleGhostEffect(GhostType state)
    {
        log::error("state: {}", (int)state);
        state = GhostType::Enabled;

        if (!PlayLayer::get())
        {
            if (m_ghostType != state)
            {
                m_ghostType = state;

                if (m_ghostTrail)
                {
                    m_ghostTrail->stopTrail();
                    m_ghostTrail = nullptr;
                }

                if (state == GhostType::Enabled)
                {
                    m_ghostTrail = GhostTrailEffect::create();
                    m_ghostTrail->m_playerScale = m_vehicleSize;
                    m_ghostTrail->m_opacity = 200;
                    m_ghostTrail->doBlendAdditive();
                    m_ghostTrail->m_color = m_playerColor1;
                    m_ghostTrail->runWithTarget(m_iconSprite, 0.05f, 0.04f, 0.0f, 0.6f, false);
                    m_parentLayer->addChild(m_ghostTrail);
                }
            }

            return;
        }

        PlayerObject::toggleGhostEffect(state);
    }
};*/