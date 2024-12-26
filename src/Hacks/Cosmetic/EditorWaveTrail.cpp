#include <Geode/Geode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/HardStreak.hpp>
#include <../../../build/bindings/bindings/Geode/modify/PlayerObject.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

#ifdef QOLMOD_EDITOR_WAVE_TRAIL

class $modify (HardStreak)
{
    void updateStroke(float p0)
    {
        if (auto lel = LevelEditorLayer::get())
            m_drawStreak = true;

        HardStreak::updateStroke(p0);
    }

    QOLMOD_MOD_ALL_HOOKS("editor-wave-trail")
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

    QOLMOD_MOD_ALL_HOOKS("editor-wave-trail")
};

#endif