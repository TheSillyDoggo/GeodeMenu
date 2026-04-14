#include "TrajectoryNode.hpp"
#include "../CheckpointFix/PlayerState.hpp"
#include "ShowTrajectory.hpp"

using namespace geode::prelude;
using namespace qolmod;

TrajectoryNode* TrajectoryNode::get()
{
    return instance;
}

TrajectoryNode::~TrajectoryNode()
{
    instance = nullptr;
}

bool TrajectoryNode::init()
{
    if (!BaseDrawNode::init())
        return false;

    instance = this;

    player = PlayerObject::create(0, 0, gjbgl, gjbgl->m_objectLayer, false);
    player->setVisible(false);
    gjbgl->m_objectLayer->addChild(player);

    setDeltaIter(0.5f);
    setIterCount(240);

    return true;
}

bool TrajectoryNode::isSimulating()
{
    return simulating;
}

float TrajectoryNode::getDeltaIter()
{
    return deltaIter;
}

int TrajectoryNode::getIterCount()
{
    return iterCount;
}

void TrajectoryNode::setDeltaIter(float delta)
{
    this->deltaIter = delta;
}

void TrajectoryNode::setIterCount(int iters)
{
    this->iterCount = iters;
}

void TrajectoryNode::redraw()
{
    clear();

    if (!ShowTrajectory::get()->getRealEnabled())
        return;

    if (ShowTrajectoryP1::get()->getRealEnabled())
    {
        simulate(gjbgl->m_player1, false);
        simulate(gjbgl->m_player1, true);
    }
    
    if (ShowTrajectoryP2::get()->getRealEnabled())
    {
        if (gjbgl->m_player2 && gjbgl->m_gameState.m_isDualMode)
        {
            simulate(gjbgl->m_player2, false);
            simulate(gjbgl->m_player2, true);
        }
    }
}

void TrajectoryNode::simulate(PlayerObject* plr, bool held)
{
    if (plr->m_isDead)
        return;

    simulating = true;

    PlayerState state;
    state.saveState(plr);
    state.loadState(this->player);

    if (gjbgl->m_playbackMode != PlaybackMode::Playing && gjbgl->m_isEditor)
    {
        player->m_isShip = gjbgl->m_levelSettings->m_startMode == 1;
        player->m_isBall = gjbgl->m_levelSettings->m_startMode == 2;
        player->m_isBird = gjbgl->m_levelSettings->m_startMode == 3;
        player->m_isDart = gjbgl->m_levelSettings->m_startMode == 4;
        player->m_isRobot = gjbgl->m_levelSettings->m_startMode == 5;
        player->m_isSpider = gjbgl->m_levelSettings->m_startMode == 6;
        player->m_isSwing = gjbgl->m_levelSettings->m_startMode == 7;
    }

    CCPoint prevPoint = plr->getPosition();

    if (
        (held && state.plMembers.m_dashRing) ||
        (held && state.plMembers.m_isRobot && state.plMembers.m_holdingButtons[(int)PlayerButton::Jump])
    ) {}
    else
    {
        player->releaseButton(PlayerButton::Jump);
        if (held)
            player->pushButton(PlayerButton::Jump);
    }

    bool useHoldCol = held == plr->m_holdingButtons[(int)PlayerButton::Jump];
    ccColor4F col;
    bool useTrail = false;
    
    if (plr == gjbgl->m_player1)
    {
        col = ccc4FFromccc3B(useHoldCol ?
            ShowTrajectoryP1Hold::get()->getColour() :
            ShowTrajectoryP1Release::get()->getColour());
        useTrail = ShowTrajectoryP1Trail::get()->getRealEnabled();
    }
    else
    {
        col = ccc4FFromccc3B(useHoldCol ?
            ShowTrajectoryP2Hold::get()->getColour() :
            ShowTrajectoryP2Release::get()->getColour());
        useTrail = ShowTrajectoryP2Trail::get()->getRealEnabled();
    }

    if (player->m_regularTrail)
        player->m_regularTrail->setVisible(false);

    trailStates.clear();

    for (size_t i = 0; i < getIterCount(); i++)
    {
        player->m_collisionLogTop->removeAllObjects();
        player->m_collisionLogBottom->removeAllObjects();
        player->m_collisionLogLeft->removeAllObjects();
        player->m_collisionLogRight->removeAllObjects();

        player->update(deltaIter);
        gjbgl->checkCollisions(player, deltaIter, false);

        player->updateRotation(deltaIter);
        player->updatePlayerScale();

        drawSegment(prevPoint, player->getPosition(), 1, col);
        prevPoint = player->getPosition();

        if (player->m_isDead || player->m_maybeIsColliding)
        {
            drawPlayerTrails();
            drawPlayerHitbox(player);
            break;
        }
        if (useTrail)
        {
            trailStates.push_back(PlayerHitboxState{
                player->getObjectRect(player->m_vehicleSize, player->m_vehicleSize),
                player->getObjectRect(0.25f, 0.25f),
                0
            });
        }
    }

    simulating = false;
}

bool TrajectoryNode::shouldDrawTrail()
{
    return true;
}