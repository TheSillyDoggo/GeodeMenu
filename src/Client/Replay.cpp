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
};*/

/*class $modify (CheckpointObjectExt, CheckpointObject)
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
};*/

class $modify (PlayLayer)
{
    void postUpdate(float dt) { //until GJBaseGameLayer::update
        PlayLayer::postUpdate(dt);

        if (!m_started)
            return;
        
        if (!m_player1->m_isDead)
            GJReplayManager::dt += dt / CCScheduler::get()->getTimeScale();

        if (GJReplayManager::playing)
        {
            //log::info("cur: {}, inp: {}", GJReplayManager::dt, GJReplayManager::replay.inputs[GJReplayManager::frame].dt);

            if (GJReplayManager::replay.inputs[GJReplayManager::frame].dt <= GJReplayManager::dt)
            {
                PlayerObject* plr = m_player1;

                if (GJReplayManager::replay.inputs[GJReplayManager::frame].player2)
                    plr = m_player2;

                plr->m_position = ccp(GJReplayManager::replay.inputs[GJReplayManager::frame].xpos, GJReplayManager::replay.inputs[GJReplayManager::frame].ypos);
                plr->m_platformerXVelocity = GJReplayManager::replay.inputs[GJReplayManager::frame].xvelocity;
                plr->m_yVelocity = GJReplayManager::replay.inputs[GJReplayManager::frame].yvelocity;
                plr->setRotation(GJReplayManager::replay.inputs[GJReplayManager::frame].rotation);

                handleButton(GJReplayManager::replay.inputs[GJReplayManager::frame].down, GJReplayManager::replay.inputs[GJReplayManager::frame].button, GJReplayManager::replay.inputs[GJReplayManager::frame].player2);
                GJReplayManager::frame++;
            }
        }
    }

    void resetLevel() {
        GJReplayManager::dt = 0;
        GJReplayManager::frame = 0;
        PlayLayer::resetLevel();

        log::info("resetLevel");
    }

    TodoReturn loadFromCheckpoint(CheckpointObject* p0)
    {
        PlayLayer::loadFromCheckpoint(p0);

        /*

        GJReplayManager::dt = as<CheckpointObjectExt*>(p0)->m_fields->dt;
        GJReplayManager::frame = as<CheckpointObjectExt*>(p0)->m_fields->frame;

        if (GJReplayManager::recording)
        {
            std::vector<MyInput> myvec = {};

            for (size_t i = 0; i < GJReplayManager::replay.inputs.size(); i++)
            {
                if (GJReplayManager::replay.inputs[i].dt <= GJReplayManager::dt)
                {
                    myvec.push_back(GJReplayManager::replay.inputs[i]);
                }
            }

            GJReplayManager::replay.inputs = myvec;
        }*/
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