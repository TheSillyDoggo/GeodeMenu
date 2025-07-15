#include "../../Client/Module.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

class JumpHack : public Module
{
    public:
        MODULE_SETUP(JumpHack)
        {
            setName("Jump Hack");
            setID("jump-hack");
            setCategory("Level");
            setDescription("a");
        }
};

SUBMIT_HACK(JumpHack);

class $modify (GJBaseGameLayer)
{
    virtual void update(float dt)
    {
        if (JumpHack::get()->getRealEnabled())
        {
            if (m_player1)
                m_player1->m_isOnGround = true;

            if (m_player2)
                m_player2->m_isOnGround = true;
        }

        GJBaseGameLayer::update(dt);
    }
};