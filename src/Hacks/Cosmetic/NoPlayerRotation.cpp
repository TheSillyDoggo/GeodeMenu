#include "../../Client/Module.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

class NoPlayerRotation : public Module
{
    public:
        MODULE_SETUP(NoPlayerRotation)
        {
            setName("No Rotation");
            setID("no-rot");
            setCategory("Cosmetic");
            setDescription("Disables rotation on players");
        }
};

SUBMIT_HACK(NoPlayerRotation);

class $modify (GJBaseGameLayer)
{
    virtual void update(float dt)
    {
        GJBaseGameLayer::update(dt);

        if (NoPlayerRotation::get()->getRealEnabled())
        {
            if (m_player1)
            {
                m_player1->setRotation(0);
                m_player1->m_iconSprite->setRotation(0);
            }

            if (m_player2)
            {
                m_player2->setRotation(0);
                m_player2->m_iconSprite->setRotation(0);
            }
        }
    }
};