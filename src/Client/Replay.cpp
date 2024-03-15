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

#endif*/

class $modify (PlayLayer)
{
    bool started = false;
    std::vector<CheckpointObject*> checkpoints = {};
    std::vector<float> checkpointsDT = {};
    std::vector<int> checkpointsFrame = {};
    std::vector<int> checkpointsFrameFix = {};

    TodoReturn startGameDelayed()
    {
        PlayLayer::startGameDelayed();

        m_fields->started = true;

        log::info("started");
    }

    TodoReturn storeCheckpoint(CheckpointObject* p0)
    {
        PlayLayer::storeCheckpoint(p0);

        if (!p0)
            return;
        
        m_fields->checkpoints.push_back(p0);
        m_fields->checkpointsDT.push_back(GJReplayManager::dt);
        m_fields->checkpointsFrame.push_back(GJReplayManager::frame);
        m_fields->checkpointsFrameFix.push_back(GJReplayManager::frameFix);

        log::info("cp {}", p0);
    }

    TodoReturn removeCheckpoint(bool p0)
    {
        PlayLayer::removeCheckpoint(p0);
        m_fields->checkpoints.pop_back();
        m_fields->checkpointsDT.pop_back();
        m_fields->checkpointsFrame.pop_back();
        m_fields->checkpointsFrameFix.pop_back();
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

        if (GJReplayManager::playing && Mod::get()->getSavedValue<bool>("frame-fixes", true) && GJReplayManager::frameFix + 1 < GJReplayManager::replay.frames.size())
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
        }

        if (GJReplayManager::recording && Mod::get()->getSavedValue<bool>("frame-fixes", true))
        {
            auto plr = m_player1;

            GJReplayManager::replay.frames.push_back(MyInput(m_gameState.m_unk1f8, 69, false, false, plr->m_position.x, plr->m_position.y, GJReplayManager::dt, plr->m_platformerXVelocity, plr->m_yVelocity, plr->getRotation()));
        }
    }

    void resetLevel() {
        GJReplayManager::dt = 0;
        GJReplayManager::frame = 0;
        GJReplayManager::frameFix = 0;

        PlayLayer::resetLevel();

        log::info("resetLevel");
    }

    TodoReturn loadFromCheckpoint(CheckpointObject* p0)
    {
        PlayLayer::loadFromCheckpoint(p0);

        auto it = find(m_fields->checkpoints.begin(), m_fields->checkpoints.end(), p0);
  
        if (it != m_fields->checkpoints.end())  
        { 
            int index = it - m_fields->checkpoints.begin();

            GJReplayManager::dt = m_fields->checkpointsDT[index];
            GJReplayManager::frame = m_fields->checkpointsFrame[index];
            GJReplayManager::frameFix = m_fields->checkpointsFrameFix[index];

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
            }
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