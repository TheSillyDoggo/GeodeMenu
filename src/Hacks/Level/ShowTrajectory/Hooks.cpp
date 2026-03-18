#include "Hooks.hpp"
#include "../Hitboxes/Hooks.hpp"
#include "../CheckpointFix/PlayerState.hpp"

using namespace geode::prelude;

void drawSegmentAlternative(CCDrawNode* drawNode, const CCPoint& startPoint, const CCPoint& endPoint, float radius, const ccColor4F& color) // taken from the depths of google
{
    CCPoint direction = ccpNormalize(ccpSub(endPoint, startPoint));
    CCPoint perpendicular = ccp(-direction.y, direction.x);

    CCPoint vertices[4];
    vertices[0] = ccpAdd(startPoint, ccpMult(perpendicular, radius));
    vertices[1] = ccpSub(startPoint, ccpMult(perpendicular, radius));
    vertices[2] = ccpSub(endPoint, ccpMult(perpendicular, radius));
    vertices[3] = ccpAdd(endPoint, ccpMult(perpendicular, radius));

    drawNode->drawPolygon(vertices, 4, color, 0, color);
}

bool TrajectoryPlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
{
    m_fields->trajectoryDrawNode = CCDrawNode::create();

    if (!PlayLayer::init(level, useReplay, dontCreateObjects))
        return false;

    m_fields->simulationPlayer = PlayerObject::create(0, 0, this, m_objectLayer, false);
    m_fields->simulationPlayer->setVisible(false);
    m_objectLayer->addChild(m_fields->simulationPlayer);

    base_cast<HitboxBaseGameLayer*>(this)->m_fields->nodeContainer->addChild(m_fields->trajectoryDrawNode);
    updateSimulation();
    return true;
}

void TrajectoryPlayLayer::createObjectsFromSetupFinished()
{
    PlayLayer::createObjectsFromSetupFinished();
}

void TrajectoryPlayLayer::postUpdate(float dt)
{
    PlayLayer::postUpdate(dt);

    if (m_started)
        updateSimulation();
}

void TrajectoryPlayLayer::destroyPlayer(PlayerObject* player, GameObject* object)
{
    if (player == m_fields->simulationPlayer)
    {
        player->m_isDead = true;
        return;
    }

    PlayLayer::destroyPlayer(player, object);
}

void TrajectoryPlayLayer::updateSimulation()
{
    return;

    auto fields = m_fields.self();

    if (!fields->trajectoryDrawNode || !m_player1)
        return;

    fields->trajectoryDrawNode->clear();

    simulatePlayer(m_player1);
}

void TrajectoryPlayLayer::simulatePlayer(PlayerObject* player)
{
    auto fields = m_fields.self();
    qolmod::PlayerState state;

    state.saveState(player);
    state.loadState(fields->simulationPlayer);

    fields->simulationPlayer->releaseButton(PlayerButton::Jump);
    fields->simulationPlayer->pushButton(PlayerButton::Jump);

    int iters = 100;
    float delta = 0.5f;

    CCPoint point = state.plMembers.m_position;

    for (size_t i = 0; i < iters; i++)
    {
        m_fields->simulationPlayer->m_isDead = false;

        m_fields->simulationPlayer->update(delta);
        this->checkCollisions(m_fields->simulationPlayer, delta, false);

        drawSegmentAlternative(m_fields->trajectoryDrawNode, fields->simulationPlayer->getPosition(), point, 0.5f, m_fields->simulationPlayer->m_isDead ? ccc4f(1, 0, 0, 1) : ccc4f(0, true ? 1 : 0.45f, 0, 1));
        point = fields->simulationPlayer->getPosition();

        if (fields->simulationPlayer->m_isDead)
        {
            CCPoint squareSize = m_fields->simulationPlayer->getObjectRect().size;
            CCPoint squarePosition = m_fields->simulationPlayer->getPosition();

            CCPoint squareVertices[] = {
                ccp(squarePosition.x - squareSize.x / 2, squarePosition.y - squareSize.y / 2), // Bottom-left
                ccp(squarePosition.x + squareSize.x / 2, squarePosition.y - squareSize.y / 2), // Bottom-right
                ccp(squarePosition.x + squareSize.x / 2, squarePosition.y + squareSize.y / 2), // Top-right
                ccp(squarePosition.x - squareSize.x / 2, squarePosition.y + squareSize.y / 2)  // Top-left
            };

            m_fields->trajectoryDrawNode->drawPolygon(squareVertices, 4, ccc4f(0, 0, 0, 0), 0.35f, ccc4f(1, 0, 0, 1));
            return;
        }
    }
}