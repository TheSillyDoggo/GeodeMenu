#include "TrajectoryNode.hpp"
#include "../CheckpointFix/PlayerState.hpp"

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

    player = PlayerObject::create(0, 0, gjbgl, gjbgl->m_objectLayer, false);
    player->setVisible(false);
    gjbgl->m_objectLayer->addChild(player);

    setDeltaIter(0.5f);
    setIterCount(240);

    instance = this;

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

    simulate(gjbgl->m_player1, false);
    simulate(gjbgl->m_player1, true);
    
    if (gjbgl->m_player2 && gjbgl->m_gameState.m_isDualMode)
    {
        simulate(gjbgl->m_player2, false);
        simulate(gjbgl->m_player2, true);
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

    CCPoint prevPoint = plr->getPosition();

    if (held && state.plMembers.m_dashRing)
    {
        // should keep same
    }
    else
    {
        player->releaseButton(PlayerButton::Jump);
        if (held)
            player->pushButton(PlayerButton::Jump);
    }

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

        drawSegment(prevPoint, player->getPosition(), 1, ccc4f(0, 1, 0, 1));
        prevPoint = player->getPosition();

        if (player->m_isDead)
        {
            drawPlayerHitbox(player);
            break;
        }
    }

    simulating = false;
}