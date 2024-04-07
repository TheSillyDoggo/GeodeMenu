/*#ifndef GEODE_IS_MACOS

#include "Replay.h"

using namespace geode::prelude;

/*
int actionsIndex = 0;
float dta;
*/
/*class $modify(PlayLayer) {

    void postUpdate(float dt) { //until GJBaseGameLayer::update // virtual void update(float p0) moment
        PlayLayer::postUpdate(dt);

        dta += dt;

        if (asdf) {
            if (actions[actionsIndex].frame <= (dta * 240.f)) {
                handleButton(actions[actionsIndex].down, actions[actionsIndex].button, actions[actionsIndex].player2);
                actionsIndex++;
            }
        }
    }

    void resetLevel() {
		dta = 0;
        PlayLayer::resetLevel();

        actionsIndex = 0;
    }
};* /

/*#ifdef GEODE_IS_WINDOWS

class $modify (CheckpointObjectExt, CheckpointObject)
{
    float dt;
    int frame;

    virtual bool init()
    {
        if (!CheckpointObject::init())
            return false;

        m_fields->dt = GJReplayManager::dt;
        m_fields->frame = GJReplayManager::frame;

        return true;
    }
};

#endif* /

class $modify (CheckpointObjectExt, CheckpointObject)
{
    float dt;
    int frame;
    int frameFix;

    static CheckpointObject* create()
    {
        auto v = CheckpointObject::create();

        as<CheckpointObjectExt*>(v)->m_fields->dt = GJReplayManager::dt;
        as<CheckpointObjectExt*>(v)->m_fields->frame = GJReplayManager::frame;
        as<CheckpointObjectExt*>(v)->m_fields->frameFix = GJReplayManager::frameFix;

        return v;
    }
};

class $modify (PlayLayer)
{
    bool started = false;

    void startGameDelayed()
    {
        PlayLayer::startGameDelayed();

        m_fields->started = true;

        log::info("started");
    }

    void postUpdate(float dt) { //until GJBaseGameLayer::update

        PlayLayer::postUpdate(dt);

        #ifdef GEODE_IS_WINDOWS

        if (!m_started)
            return;
        #endif
        
        #ifdef GEODE_IS_ANDROID32

        if (!m_fields->started)
        {
            m_fields->started = ((*(double*)(((char*)PlayLayer::get()) + 0x320)) != 0);
            return;
        }

        #endif

        #ifdef GEODE_IS_ANDROID64

        if (!m_fields->started)
        {
            m_fields->started = ((*(double*)(((char*)PlayLayer::get()) + 0x3B8)) != 0);
            return;
        }

        #endif

        if (!m_player1->m_isDead)
            GJReplayManager::dt += dt / CCScheduler::get()->getTimeScale();

        if (GJReplayManager::playing && GJReplayManager::frame + 1 < GJReplayManager::replay.inputs.size())
        {

            //log::info("cur: {}, inp: {}", GJReplayManager::dt, GJReplayManager::replay.inputs[GJReplayManager::frame].dt);

            while (GJReplayManager::replay.inputs[GJReplayManager::frame].dt <= GJReplayManager::dt) // SHOULD fix this replay issues :3
            //if (GJReplayManager::replay.inputs[GJReplayManager::frame].dt <= GJReplayManager::dt)
            {

                PlayerObject* plr = m_player1;

                if (GJReplayManager::replay.inputs[GJReplayManager::frame].player2)
                    plr = m_player2;

                if (Mod::get()->getSavedValue<bool>("click-fixes"))
                {
                    plr->m_position = ccp(GJReplayManager::replay.inputs[GJReplayManager::frame].xpos, GJReplayManager::replay.inputs[GJReplayManager::frame].ypos);
                    plr->m_platformerXVelocity = GJReplayManager::replay.inputs[GJReplayManager::frame].xvelocity;
                    plr->m_yVelocity = GJReplayManager::replay.inputs[GJReplayManager::frame].yvelocity;
                    plr->setRotation(GJReplayManager::replay.inputs[GJReplayManager::frame].rotation);
                }

                handleButton(GJReplayManager::replay.inputs[GJReplayManager::frame].down, GJReplayManager::replay.inputs[GJReplayManager::frame].button, GJReplayManager::replay.inputs[GJReplayManager::frame].player2);
                GJReplayManager::frame++;
            }
        }

        /*if (GJReplayManager::playing && Mod::get()->getSavedValue<bool>("frame-fixes", true) && GJReplayManager::frameFix + 1 < GJReplayManager::replay.frames.size())
        {
            while (GJReplayManager::replay.frames[GJReplayManager::frameFix].dt <= GJReplayManager::dt)
            {
                PlayerObject* plr = m_player1;

                if (GJReplayManager::replay.frames[GJReplayManager::frameFix].player2)
                    plr = m_player2;

                plr->m_position = ccp(GJReplayManager::replay.frames[GJReplayManager::frameFix].xpos, GJReplayManager::replay.frames[GJReplayManager::frameFix].ypos);
                plr->m_platformerXVelocity = GJReplayManager::replay.frames[GJReplayManager::frameFix].xvelocity;
                plr->m_yVelocity = GJReplayManager::replay.frames[GJReplayManager::frameFix].yvelocity;
                plr->setRotation(GJReplayManager::replay.frames[GJReplayManager::frameFix].rotation);

                GJReplayManager::frameFix++;
            }
        }*/

        /*if (GJReplayManager::recording && Mod::get()->getSavedValue<bool>("frame-fixes", true))
        {
            auto plr = m_player1;

            GJReplayManager::replay.frames.push_back(MyInput(m_gameState.m_unk1f8, 69, false, false, plr->m_position.x, plr->m_position.y, GJReplayManager::dt, plr->m_platformerXVelocity, plr->m_yVelocity, plr->getRotation()));
        }* /
    }

    void resetLevel() {
        GJReplayManager::dt = 0;
        GJReplayManager::frame = 0;
        GJReplayManager::frameFix = 0;

        log::info("resetLevel");
        PlayLayer::resetLevel();
    }

    void loadFromCheckpoint(CheckpointObject* p0)
    {
        log::info("cp");
        PlayLayer::loadFromCheckpoint(p0);

        GJReplayManager::dt = as<CheckpointObjectExt*>(p0)->dt;
        GJReplayManager::frame = as<CheckpointObjectExt*>(p0)->frame;
        GJReplayManager::frameFix = as<CheckpointObjectExt*>(p0)->frameFix;

        while (GJReplayManager::replay.inputs[GJReplayManager::replay.inputs.size() - 1].dt > GJReplayManager::dt)
        {
            GJReplayManager::replay.inputs.pop_back();
        }
    }
};

class $modify(GJBaseGameLayer) {

    void handleButton(bool push, int button, bool player1) {

        GJBaseGameLayer::handleButton(push, button, player1);

        PlayerObject* plr = m_player1;

        if (!player1)
            plr = m_player2;

        if (GJReplayManager::recording) GJReplayManager::replay.inputs.push_back(MyInput(m_gameState.m_unk1f8, button, !player1, push, plr->m_position.x, plr->m_position.y, GJReplayManager::dt, plr->m_platformerXVelocity, plr->m_yVelocity, plr->getRotation()));
    }
};

#endif*/