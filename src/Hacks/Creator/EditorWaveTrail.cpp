#include "../../Client/Module.hpp"
#include <Geode/modify/HardStreak.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class EditorWaveTrail : public Module
{
    public:
        MODULE_SETUP(EditorWaveTrail)
        {
            setID("editor-wave-trail");
            setCategory("Creator");
        }
};

SUBMIT_HACK(EditorWaveTrail);

class $modify (HardStreak)
{
    void updateStroke(float p0)
    {
        if (LevelEditorLayer::get())
        {
            if (EditorWaveTrail::get()->getRealEnabled())
            {
                m_drawStreak = true;
            }
            else
            {
                if (m_nBufferCount > 0)
                    clear();
                
                return;
            }
        }

        HardStreak::updateStroke(p0);
    }
};

class $modify (PlayerObject)
{
    void placeStreakPoint()
    {
        if (LevelEditorLayer::get() && m_isDart)
            m_waveTrail->addPoint(this->getPosition());
        else
            PlayerObject::placeStreakPoint();
    }

    virtual void update(float dt)
    {
        PlayerObject::update(dt);

        if (LevelEditorLayer::get() && m_isDart)
            m_waveTrail->m_currentPoint = this->getPosition();
    }

    void fadeOutStreak2(float p0)
    {
        PlayerObject::fadeOutStreak2(p0);

        if (LevelEditorLayer::get())
            m_waveTrail->runAction(CCFadeTo::create(p0, 0));
    }
};