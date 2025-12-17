#include "../../Client/Module.hpp"
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/MenuGameLayer.hpp>

using namespace geode::prelude;

class Suicide : public Module
{
    public:
        MODULE_SETUP(Suicide)
        {
            setName("Suicide");
            setID("suicide");
            setCategory("Level");
            setDescription("Kills your player immediately");
        }
};

SUBMIT_HACK(Suicide);

class $modify (MenuGameLayer)
{
    struct Fields
    {
        float killDelta;
    };

    virtual void update(float dt)
    {
        MenuGameLayer::update(dt);

        if (!Suicide::get()->getRealEnabled())
            return;

        m_fields->killDelta += dt;

        if (m_fields->killDelta > 0.3f)
        {
            m_fields->killDelta = 0;

            destroyPlayer();
        }
    }
};

class $modify (PlayLayer)
{
    virtual void postUpdate(float dt)
    {
        PlayLayer::postUpdate(dt);

        if (!Suicide::get()->getRealEnabled())
            return;

        if (m_player1 && !m_player1->m_isDead)
        {    
            this->PlayLayer::destroyPlayer(m_player1, nullptr);
        }
    }
};