#include "Hooks.hpp"
#include "../Hitboxes/Hooks.hpp"
#include "../CheckpointFix/PlayerState.hpp"
#include "TrajectoryNode.hpp"

using namespace geode::prelude;
using namespace qolmod;

bool TrajectoryBGL::init()
{
    if (!GJBaseGameLayer::init())
        return false;

    return true;
}

bool TrajectoryEditor::init(GJGameLevel* level, bool noUI)
{
    if (!LevelEditorLayer::init(level, noUI))
        return false;

    this->addChild(TrajectoryNode::create(), 10);

    return true;
}

void TrajectoryPlayLayer::createObjectsFromSetupFinished()
{
    PlayLayer::createObjectsFromSetupFinished();

    this->addChild(TrajectoryNode::create(), 10);
}

void TrajectoryPlayLayer::destroyPlayer(PlayerObject* player, GameObject* object)
{
    if (TrajectoryNode::get()->isSimulating())
    {
        player->m_isDead = true;
        return;
    }

    PlayLayer::destroyPlayer(player, object);
}


class $modify(PlayLayer) {

    /*void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);

        if (!t.trajectoryNode() || TrajectoryNode::get()->isSimulating()) return;

        if (Global::get().showTrajectory) {
            ShowTrajectory::updateTrajectory(this);
        }

    }

    void setupHasCompleted() {
        PlayLayer::setupHasCompleted();

        t.fakePlayer1 = nullptr;
        t.fakePlayer2 = nullptr;
        t.cancelTrajectory = false;
        TrajectoryNode::get()->isSimulating() = false;

        t.fakePlayer1 = PlayerObject::create(1, 1, this, this, true);
        t.fakePlayer1->retain();
        t.fakePlayer1->setPosition({ 0, 105 });
        t.fakePlayer1->setVisible(false);
        m_objectLayer->addChild(t.fakePlayer1);

        t.fakePlayer2 = PlayerObject::create(1, 1, this, this, true);
        t.fakePlayer2->retain();
        t.fakePlayer2->setPosition({ 0, 105 });
        t.fakePlayer2->setVisible(false);
        m_objectLayer->addChild(t.fakePlayer2);

        m_objectLayer->addChild(t.trajectoryNode(), 500);
    }*/

    /*void destroyPlayer(PlayerObject * player, GameObject * gameObject) {
        if (TrajectoryNode::get()->isSimulating() || (player == t.fakePlayer1 || player == t.fakePlayer2)) {
            t.deathRotation = player->getRotation();
            t.cancelTrajectory = true;
            return;
        }

        PlayLayer::destroyPlayer(player, gameObject);
    }*/

    /*void onQuit() {
        if (t.trajectoryNode())
            t.trajectoryNode()->clear();

        t.fakePlayer1 = nullptr;
        t.fakePlayer2 = nullptr;
        t.cancelTrajectory = false;
        TrajectoryNode::get()->isSimulating() = false;

        PlayLayer::onQuit();
    }*/

    void playEndAnimationToPos(cocos2d::CCPoint p0) {
        if (TrajectoryNode::get() && !TrajectoryNode::get()->isSimulating())
            PlayLayer::playEndAnimationToPos(p0);
    }

};

/*class $modify(PauseLayer) {
    void goEdit() {
        if (t.trajectoryNode())
            t.trajectoryNode()->clear();

        t.fakePlayer1 = nullptr;
        t.fakePlayer2 = nullptr;
        t.cancelTrajectory = false;
        TrajectoryNode::get()->isSimulating() = false;

        PauseLayer::goEdit();
    }
};*/

class $modify(GJBaseGameLayer) {

    /*void collisionCheckObjects(PlayerObject * p0, gd::vector<GameObject*>*objects, int p2, float p3) {
        if (TrajectoryNode::get()->isSimulating()) {
            std::vector<GameObject*> disabledObjects;

            for (const auto& obj : *objects) {
                if (!obj) continue;

                if ((!objectTypes.contains(static_cast<int>(obj->m_objectType)) && !portalIDs.contains(obj->m_objectID)) || collectibleIDs.contains(obj->m_objectID)) {
                    if (obj->m_isDisabled || obj->m_isDisabled2) continue;  

                    disabledObjects.push_back(obj);
                    obj->m_isDisabled = true;
                    obj->m_isDisabled2 = true;
                }
            }

            GJBaseGameLayer::collisionCheckObjects(p0, objects, p2, p3);

            for (const auto& obj : disabledObjects) {
                if (!obj) continue;

                obj->m_isDisabled = false;
                obj->m_isDisabled2 = false;
            }

            disabledObjects.clear();

            return;
        }

        GJBaseGameLayer::collisionCheckObjects(p0, objects, p2, p3);
    }*/

    bool canBeActivatedByPlayer(PlayerObject * p0, EffectGameObject * p1) {
        if (TrajectoryNode::get()->isSimulating()) {

            // ShowTrajectory::handlePortal(p0, p1->m_objectID);

            return false;
        }

        return GJBaseGameLayer::canBeActivatedByPlayer(p0, p1);
    }

    void playerTouchedRing(PlayerObject * p0, RingObject * p1) {
        if (TrajectoryNode::get() && !TrajectoryNode::get()->isSimulating())
            GJBaseGameLayer::playerTouchedRing(p0, p1);
    }

    void playerTouchedTrigger(PlayerObject * p0, EffectGameObject * p1) {
        if (TrajectoryNode::get() && !TrajectoryNode::get()->isSimulating())
            GJBaseGameLayer::playerTouchedTrigger(p0, p1);
        // else
            // ShowTrajectory::handlePortal(p0, p1->m_objectID);
    }

    void activateSFXTrigger(SFXTriggerGameObject * p0) {
        if (TrajectoryNode::get() && !TrajectoryNode::get()->isSimulating())
            GJBaseGameLayer::activateSFXTrigger(p0);

    }
    void activateSongEditTrigger(SongTriggerGameObject * p0) {
        if (TrajectoryNode::get() && !TrajectoryNode::get()->isSimulating())
            GJBaseGameLayer::activateSongEditTrigger(p0);

    }
    // void activateSongTrigger(SongTriggerGameObject * p0) {
    //     if (TrajectoryNode::get() && !TrajectoryNode::get()->isSimulating())
    //         GJBaseGameLayer::activateSongTrigger(p0);
    // }

    void gameEventTriggered(GJGameEvent p0, int p1, int p2) {
        if (TrajectoryNode::get() && !TrajectoryNode::get()->isSimulating())
            GJBaseGameLayer::gameEventTriggered(p0, p1, p2);
    }

};

class $modify(PlayerObject) {

    void update(float dt) {
        PlayerObject::update(dt);
        // t.delta = dt;
    }

    void playSpiderDashEffect(cocos2d::CCPoint p0, cocos2d::CCPoint p1) {
        if (TrajectoryNode::get() && !TrajectoryNode::get()->isSimulating())
            PlayerObject::playSpiderDashEffect(p0, p1);
    }

    void incrementJumps() {
        if (TrajectoryNode::get() && !TrajectoryNode::get()->isSimulating())
            PlayerObject::incrementJumps();
    }

    void ringJump(RingObject * p0, bool p1) {
        if (TrajectoryNode::get() && !TrajectoryNode::get()->isSimulating())
            PlayerObject::ringJump(p0, p1);
    }

    void stopDashing()
    {
        PlayerObject::stopDashing();

        if (TrajectoryNode::get() && TrajectoryNode::get()->isSimulating())
        {
            if (m_dashFireSprite)
            {
                m_dashFireSprite->setScale(0);
            }
        }
    }

};

class $modify(HardStreak) {

    void addPoint(cocos2d::CCPoint p0) {
        if (TrajectoryNode::get() && !TrajectoryNode::get()->isSimulating())
            HardStreak::addPoint(p0);
    }
};

class $modify(GameObject) {

    void playShineEffect() {
        if (TrajectoryNode::get() && !TrajectoryNode::get()->isSimulating())
            GameObject::playShineEffect();
    }
};

class $modify(EffectGameObject) {

    void triggerObject(GJBaseGameLayer * p0, int p1, const gd::vector<int>*p2) {
        if (TrajectoryNode::get() && !TrajectoryNode::get()->isSimulating())
            EffectGameObject::triggerObject(p0, p1, p2);
    }
};