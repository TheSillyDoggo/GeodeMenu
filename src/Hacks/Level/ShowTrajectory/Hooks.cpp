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


class $modify(PlayLayer)
{
    void playEndAnimationToPos(cocos2d::CCPoint p0)
    {
        if (TrajectoryNode::get() && !TrajectoryNode::get()->isSimulating())
            PlayLayer::playEndAnimationToPos(p0);
    }

};

class $modify(GJBaseGameLayer)
{
    void customCollisionCheck(PlayerObject* player, GameObject* obj)
    {
        bool intersects = false;

        if (obj->m_orientedBox && obj->m_shouldUseOuterOb)
            intersects = obj->m_orientedBox->overlaps(player->m_orientedBox);
        else
            intersects = player->getObjectRect().intersectsRect(obj->getObjectRect());

        if (intersects)
        {
            if (obj->m_objectID == 1829) // s block
            {
                player->stopDashing();
            }

            switch (obj->m_objectType)
            {
                case GameObjectType::CubePortal:
                    player->toggleFlyMode(false, false);
                    player->toggleRollMode(false, false);
                    player->toggleBirdMode(false, false);
                    player->toggleDartMode(false, false);
                    player->toggleRobotMode(false, false);
                    player->toggleSpiderMode(false, false);
                    player->toggleSwingMode(false, false);
                    break;
                case GameObjectType::ShipPortal:
                    player->toggleFlyMode(true, true);
                    break;
                case GameObjectType::BallPortal:
                    player->toggleRollMode(true, true);
                    break;
                case GameObjectType::UfoPortal:
                    player->toggleBirdMode(true, true);
                    break;
                case GameObjectType::WavePortal:
                    player->toggleDartMode(true, true);
                    break;
                case GameObjectType::RobotPortal:
                    player->toggleRobotMode(true, true);
                    break;
                case GameObjectType::SpiderPortal:
                    player->toggleSpiderMode(true, true);
                    break;
                case GameObjectType::SwingPortal:
                    player->toggleSwingMode(true, true);
                    break;
                
                case GameObjectType::NormalGravityPortal:
                    this->flipGravity(player, false, true);
                    break;
                case GameObjectType::InverseGravityPortal:
                    this->flipGravity(player, true, true);
                    break;
                case GameObjectType::GravityTogglePortal:
                    this->flipGravity(player, !player->m_isUpsideDown, true);
                    break;

                // TODO: this causes orb circles to break
                case GameObjectType::YellowJumpPad:
                case GameObjectType::PinkJumpPad:
                case GameObjectType::RedJumpPad:
                case GameObjectType::SpiderPad:
                    if (static_cast<EffectGameObject*>(obj)->m_isReverse)
                        player->reversePlayer(static_cast<EffectGameObject*>(obj));

                    player->bumpPlayer(getBumpMod(player, (int)obj->m_objectType), (int)obj->m_objectType, true, nullptr);
                    break;
                
                default:
                    break;
            }
        }
    }


    void collisionCheckObjects(PlayerObject* p0, gd::vector<GameObject*>* p1, int p2, float p3)
    {
        if (TrajectoryNode::get() && TrajectoryNode::get()->isSimulating())
        {
            gd::vector<GameObject*> p1old = *p1;

            auto new_end = std::remove_if(p1->begin(), p1->end(), [this, p0](GameObject* obj)
            {
                bool del = true;
                
                if (typeinfo_cast<GameObject*>(obj))
                {
                    if (obj->m_objectType == GameObjectType::Solid)
                        del = false;

                    if (obj->m_objectType == GameObjectType::Hazard)
                        del = false;

                    if (obj->m_objectType == GameObjectType::AnimatedHazard)
                        del = false;

                    if (obj->m_objectType == GameObjectType::Slope)
                        del = false;
                }

                if (del)
                    customCollisionCheck(p0, obj);
                
                return del;
            });

            p1->erase(new_end, p1->end());
            p2 = p1->size();

            GJBaseGameLayer::collisionCheckObjects(p0, p1, p2, p3);

            *p1 = p1old;

            return;
        }

        GJBaseGameLayer::collisionCheckObjects(p0, p1, p2, p3);
    }

    bool canBeActivatedByPlayer(PlayerObject * p0, EffectGameObject * p1)
    {
        if (TrajectoryNode::get()->isSimulating())
        {
            return false;
        }

        return GJBaseGameLayer::canBeActivatedByPlayer(p0, p1);
    }

    void playerTouchedRing(PlayerObject * p0, RingObject * p1)
    {
        if (TrajectoryNode::get() && !TrajectoryNode::get()->isSimulating())
            GJBaseGameLayer::playerTouchedRing(p0, p1);
    }

    void playerTouchedTrigger(PlayerObject * p0, EffectGameObject * p1)
    {
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

class $modify(PlayerObject)
{

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