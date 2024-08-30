#include "Trajectory.hpp"

#define ResetMember(_mem) fields->trajectoryPlayer->_mem = player->_mem;
#define RM ResetMember

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
    if (!PlayLayer::init(level, useReplay, dontCreateObjects))
        return false;

    auto tbgl = base_cast<TrajectoryBGL*>(this);

    tbgl->m_fields->trajectoryPlayer = PlayerObject::create(0, 0, this, m_objectLayer, false);
    tbgl->m_fields->trajectoryDraw = CCDrawNode::create();

    tbgl->m_fields->trajectoryPlayer->setVisible(false);
    tbgl->m_fields->trajectoryPlayer->setID("trajectory-player"_spr);

    tbgl->m_fields->trajectoryDraw->setID("trajectory-draw-node"_spr);

    m_objectLayer->addChild(tbgl->m_fields->trajectoryPlayer);
    m_debugDrawNode->getParent()->addChild(tbgl->m_fields->trajectoryDraw);
    return true;
}

void TrajectoryPlayLayer::destroyPlayer(PlayerObject* p0, GameObject* p1)
{
    auto tbgl = base_cast<TrajectoryBGL*>(this);

    if (p0 == tbgl->m_fields->trajectoryPlayer)
    {
        tbgl->m_fields->trajectoryPlayer->m_isDead = true;

        return;
    }

    PlayLayer::destroyPlayer(p0, p1);
}

bool TrajectoryEditorLayer::init(GJGameLevel* p0, bool p1)
{
    if (!LevelEditorLayer::init(p0, p1))
        return false;

    auto tbgl = base_cast<TrajectoryBGL*>(this);

    tbgl->m_fields->trajectoryPlayer = PlayerObject::create(0, 0, this, m_objectLayer, false);
    tbgl->m_fields->trajectoryDraw = CCDrawNode::create();

    tbgl->m_fields->trajectoryPlayer->setVisible(false);
    tbgl->m_fields->trajectoryPlayer->setID("trajectory-player"_spr);

    tbgl->m_fields->trajectoryDraw->setID("trajectory-draw-node"_spr);

    m_objectLayer->addChild(tbgl->m_fields->trajectoryPlayer);
    m_debugDrawNode->getParent()->addChild(tbgl->m_fields->trajectoryDraw);
    return true;
}

void TrajectoryEditorLayer::playerTookDamage(PlayerObject* p0)
{
    auto tbgl = base_cast<TrajectoryBGL*>(this);

    if (p0 == tbgl->m_fields->trajectoryPlayer)
    {
        tbgl->m_fields->trajectoryPlayer->m_isDead = true;

        return;
    }

    LevelEditorLayer::playerTookDamage(p0);
}

void TrajectoryPlayerObject::playSpiderDashEffect(cocos2d::CCPoint from, cocos2d::CCPoint to)
{
    if (m_gameLayer && as<TrajectoryBGL*>(m_gameLayer)->m_fields->trajectoryPlayer == this)
        return;

    PlayerObject::playSpiderDashEffect(from, to);
}

void TrajectoryBGL::collisionCheckObjects(PlayerObject* p0, gd::vector<GameObject*>* p1, int p2, float p3)
{
    if (p0 == m_fields->trajectoryPlayer)
    {
        gd::vector<GameObject*> objsOld = *p1;

        p1->clear();

        for (auto obj : objsOld)
        {
            if (obj->m_objectType == GameObjectType::Solid || obj->m_objectType == GameObjectType::Hazard || obj->m_objectType == GameObjectType::AnimatedHazard || obj->m_objectType == GameObjectType::Slope)
            {
                p1->push_back(obj);
            }
        }

        p2 = p1->size();

        GJBaseGameLayer::collisionCheckObjects(p0, p1, p2, p3);

        *p1 = objsOld;

        return;
    }

    GJBaseGameLayer::collisionCheckObjects(p0, p1, p2, p3);
}

void TrajectoryBGL::checkRepellPlayer()
{
    GJBaseGameLayer::checkRepellPlayer();

    if (m_fields->trajectoryPlayer)
    {
        m_fields->trajectoryDraw->clear();

        resetSimulation(m_player1);
        simulateTrajectory(true, m_player1);
        resetSimulation(m_player1);
        simulateTrajectory(false, m_player1);
    }
}

void TrajectoryBGL::resetSimulation(PlayerObject* player)
{
    auto fields = m_fields.self();

    fields->point = player->getPosition();
    fields->trajectoryPlayer->setPosition(player->getPosition());

    fields->trajectoryPlayer->m_collisionLogTop->removeAllObjects();
    fields->trajectoryPlayer->m_collisionLogBottom->removeAllObjects();
    fields->trajectoryPlayer->m_collisionLogLeft->removeAllObjects();
    fields->trajectoryPlayer->m_collisionLogRight->removeAllObjects();

    fields->trajectoryPlayer->m_lastCollisionBottom = 0;
    fields->trajectoryPlayer->m_lastCollisionTop = 0;
    fields->trajectoryPlayer->m_lastCollisionLeft = 0;
    fields->trajectoryPlayer->m_lastCollisionRight = 0;

    int i = 0;

    RM(m_wasTeleported)  log::info("a: {}", i++);
    RM(m_fixGravityBug)  log::info("a: {}", i++);
    RM(m_reverseSync)  log::info("a: {}", i++);
    RM(m_yVelocityBeforeSlope)  log::info("a: {}", i++);
    RM(m_dashX)  log::info("a: {}", i++);
    RM(m_dashY)  log::info("a: {}", i++);
    RM(m_dashAngle)  log::info("a: {}", i++);
    RM(m_dashStartTime)  log::info("a: {}", i++);
    RM(m_dashRing)  log::info("a: {}", i++);
    RM(m_slopeStartTime)  log::info("a: {}", i++);
    RM(m_justPlacedStreak)  log::info("a: {}", i++);
    RM(m_maybeLastGroundObject)  log::info("a: {}", i++);
    RM(m_unk50C)  log::info("a: {}", i++);
    RM(m_unk510)  log::info("a: {}", i++);
    RM(m_currentSlope2)  log::info("a: {}", i++);
    RM(m_preLastGroundObject)  log::info("a: {}", i++);
    RM(m_slopeAngle)  log::info("a: {}", i++);
    RM(m_slopeSlidingMaybeRotated)  log::info("a: {}", i++);
    RM(m_quickCheckpointMode)  log::info("a: {}", i++);
    RM(m_collidedObject)  log::info("a: {}", i++);
    RM(m_lastGroundObject)  log::info("a: {}", i++);
    RM(m_collidingWithLeft)  log::info("a: {}", i++);
    RM(m_collidingWithRight)  log::info("a: {}", i++);
    RM(m_maybeSavedPlayerFrame)  log::info("a: {}", i++);
    RM(m_scaleXRelated2)  log::info("a: {}", i++);
    RM(m_groundYVelocity)  log::info("a: {}", i++);
    RM(m_yVelocityRelated)  log::info("a: {}", i++);
    RM(m_scaleXRelated3)  log::info("a: {}", i++);
    RM(m_scaleXRelated4)  log::info("a: {}", i++);
    RM(m_scaleXRelated5)  log::info("a: {}", i++);
    RM(m_isCollidingWithSlope)  log::info("a: {}", i++);
    RM(m_isBallRotating)  log::info("a: {}", i++);
    RM(m_unk669)  log::info("a: {}", i++);
    RM(m_currentSlope3)  log::info("a: {}", i++);
    RM(m_currentSlope)  log::info("a: {}", i++);
    RM(unk_584)  log::info("a: {}", i++);
    RM(m_collidingWithSlopeId)  log::info("a: {}", i++);
    RM(m_slopeFlipGravityRelated)  log::info("a: {}", i++);
    RM(m_slopeAngleRadians)  log::info("a: {}", i++);
    // RM(m_rotateObjectsRelated)  log::info("a: {}", i++);
    // RM(m_maybeRotatedObjectsMap)  log::info("a: {}", i++);
    RM(m_rotationSpeed)  log::info("a: {}", i++);
    RM(m_rotateSpeed)  log::info("a: {}", i++);
    RM(m_isRotating)  log::info("a: {}", i++);
    RM(m_isBallRotating2)  log::info("a: {}", i++);
    RM(m_hasGlow)  log::info("a: {}", i++);
    RM(m_isHidden)  log::info("a: {}", i++);
    RM(m_speedMultiplier)  log::info("a: {}", i++);
    RM(m_yStart)  log::info("a: {}", i++);
    RM(m_gravity)  log::info("a: {}", i++);
    RM(m_trailingParticleLife)  log::info("a: {}", i++);
    RM(m_unk648)  log::info("a: {}", i++);
    RM(m_gameModeChangedTime)  log::info("a: {}", i++);
    RM(m_padRingRelated)  log::info("a: {}", i++);
    RM(m_maybeReducedEffects)  log::info("a: {}", i++);
    RM(m_maybeIsFalling)  log::info("a: {}", i++);
    RM(m_shouldTryPlacingCheckpoint)  log::info("a: {}", i++);
    RM(m_playEffects)  log::info("a: {}", i++);
    RM(m_maybeCanRunIntoBlocks)  log::info("a: {}", i++);
    RM(m_hasGroundParticles)  log::info("a: {}", i++);
    RM(m_hasShipParticles)  log::info("a: {}", i++);
    RM(m_isOnGround3)  log::info("a: {}", i++);
    RM(m_checkpointTimeout)  log::info("a: {}", i++);
    RM(m_lastCheckpointTime)  log::info("a: {}", i++);
    RM(m_lastJumpTime)  log::info("a: {}", i++);
    RM(m_lastFlipTime)  log::info("a: {}", i++);
    RM(m_flashTime)  log::info("a: {}", i++);
    RM(m_flashRelated)  log::info("a: {}", i++);
    RM(m_flashRelated1)  log::info("a: {}", i++);
    RM(m_colorRelated2)  log::info("a: {}", i++);
    RM(m_flashRelated3)  log::info("a: {}", i++);
    RM(m_lastSpiderFlipTime)  log::info("a: {}", i++);
    RM(m_unkBool5)  log::info("a: {}", i++);
    RM(m_maybeIsVehicleGlowing)  log::info("a: {}", i++);
    RM(m_gv0096)  log::info("a: {}", i++);
    RM(m_gv0100)  log::info("a: {}", i++);
    RM(m_accelerationOrSpeed)  log::info("a: {}", i++);
    RM(m_snapDistance)  log::info("a: {}", i++);
    RM(m_ringJumpRelated)  log::info("a: {}", i++);
    // RM(m_ringRelatedSet)  log::info("a: {}", i++);
    RM(m_objectSnappedTo)  log::info("a: {}", i++);
    RM(m_pendingCheckpoint)  log::info("a: {}", i++);
    RM(m_maybeSpriteRelated)  log::info("a: {}", i++);
    RM(m_slopeRotation)  log::info("a: {}", i++);
    RM(m_currentSlopeYVelocity)  log::info("a: {}", i++);
    RM(m_unk3d0)  log::info("a: {}", i++);
    RM(m_blackOrbRelated)  log::info("a: {}", i++);
    RM(m_unk3e0)  log::info("a: {}", i++);
    RM(m_unk3e1)  log::info("a: {}", i++);
    RM(m_isAccelerating)  log::info("a: {}", i++);
    RM(m_isCurrentSlopeTop)  log::info("a: {}", i++);
    RM(m_collidedTopMinY)  log::info("a: {}", i++);
    RM(m_collidedBottomMaxY)  log::info("a: {}", i++);
    RM(m_collidedLeftMaxX)  log::info("a: {}", i++);
    RM(m_collidedRightMinX)  log::info("a: {}", i++);
    RM(m_streakRelated4)  log::info("a: {}", i++);
    RM(m_canPlaceCheckpoint)  log::info("a: {}", i++);
    RM(m_colorRelated)  log::info("a: {}", i++);
    RM(m_secondColorRelated)  log::info("a: {}", i++);
    RM(m_hasCustomGlowColor)  log::info("a: {}", i++);
    RM(m_glowColor)  log::info("a: {}", i++);
    RM(m_maybeIsColliding)  log::info("a: {}", i++);
    RM(m_jumpBuffered)  log::info("a: {}", i++);
    RM(m_stateRingJump)  log::info("a: {}", i++);
    RM(m_wasJumpBuffered)  log::info("a: {}", i++);
    RM(m_wasRobotJump)  log::info("a: {}", i++);
    RM(m_stateJumpBuffered)  log::info("a: {}", i++);
    RM(m_stateRingJump2)  log::info("a: {}", i++);
    RM(m_touchedRing)  log::info("a: {}", i++);
    RM(m_touchedCustomRing)  log::info("a: {}", i++);
    RM(m_touchedGravityPortal)  log::info("a: {}", i++);
    RM(m_maybeTouchedBreakableBlock)  log::info("a: {}", i++);
    RM(m_jumpRelatedAC2)  log::info("a: {}", i++);
    RM(m_touchedPad)  log::info("a: {}", i++);
    RM(m_yVelocity)  log::info("a: {}", i++);
    RM(m_fallSpeed)  log::info("a: {}", i++);
    RM(m_isOnSlope)  log::info("a: {}", i++);
    RM(m_wasOnSlope)  log::info("a: {}", i++);
    RM(m_slopeVelocity)  log::info("a: {}", i++);
    RM(m_maybeUpsideDownSlope)  log::info("a: {}", i++);
    RM(m_isShip)  log::info("a: {}", i++);
    RM(m_isBird)  log::info("a: {}", i++);
    RM(m_isBall)  log::info("a: {}", i++);
    RM(m_isDart)  log::info("a: {}", i++);
    RM(m_isRobot)  log::info("a: {}", i++);
    RM(m_isSpider)  log::info("a: {}", i++);
    RM(m_isUpsideDown)  log::info("a: {}", i++);
    RM(m_isDead)  log::info("a: {}", i++);
    RM(m_isOnGround)  log::info("a: {}", i++);
    RM(m_isGoingLeft)  log::info("a: {}", i++);
    RM(m_isSideways)  log::info("a: {}", i++);
    RM(m_isSwing)  log::info("a: {}", i++);
    RM(m_reverseRelated)  log::info("a: {}", i++);
    RM(m_maybeReverseSpeed)  log::info("a: {}", i++);
    RM(m_maybeReverseAcceleration)  log::info("a: {}", i++);
    RM(m_xVelocityRelated2)  log::info("a: {}", i++);
    RM(m_isDashing)  log::info("a: {}", i++);
    RM(m_unk9e8)  log::info("a: {}", i++);
    RM(m_groundObjectMaterial)  log::info("a: {}", i++);
    RM(m_vehicleSize)  log::info("a: {}", i++);
    RM(m_playerSpeed)  log::info("a: {}", i++);
    RM(m_shipRotation)  log::info("a: {}", i++);
    RM(m_lastPortalPos)  log::info("a: {}", i++);
    RM(m_unkUnused3)  log::info("a: {}", i++);
    RM(m_isOnGround2)  log::info("a: {}", i++);
    RM(m_lastLandTime)  log::info("a: {}", i++);
    RM(m_platformerVelocityRelated)  log::info("a: {}", i++);
    RM(m_maybeIsBoosted)  log::info("a: {}", i++);
    RM(m_scaleXRelatedTime)  log::info("a: {}", i++);
    RM(m_decreaseBoostSlide)  log::info("a: {}", i++);
    RM(m_unkA29)  log::info("a: {}", i++);
    RM(m_isLocked)  log::info("a: {}", i++);
    RM(m_controlsDisabled)  log::info("a: {}", i++);
    RM(m_lastGroundedPos)  log::info("a: {}", i++);
    // RM(m_touchedRings)  log::info("a: {}", i++);
    RM(m_lastActivatedPortal)  log::info("a: {}", i++);
    RM(m_hasEverJumped)  log::info("a: {}", i++);
    RM(m_ringOrStreakRelated)  log::info("a: {}", i++);
    RM(m_playerColor1)  log::info("a: {}", i++);
    RM(m_playerColor2)  log::info("a: {}", i++);
    RM(m_position)  log::info("a: {}", i++);
    RM(m_isSecondPlayer)  log::info("a: {}", i++);
    RM(m_unkA99)  log::info("a: {}", i++);
    RM(m_totalTime)  log::info("a: {}", i++);
    RM(m_isBeingSpawnedByDualPortal)  log::info("a: {}", i++);
    RM(m_unkAAC)  log::info("a: {}", i++);
    RM(m_unkAngle1)  log::info("a: {}", i++);
    RM(m_yVelocityRelated3)  log::info("a: {}", i++);
    RM(m_gamevar0060)  log::info("a: {}", i++);
    RM(m_swapColors)  log::info("a: {}", i++);
    RM(m_gamevar0062)  log::info("a: {}", i++);
    RM(m_followRelated)  log::info("a: {}", i++);
    RM(m_playerFollowFloats)  log::info("a: {}", i++);
    RM(m_isOutOfBounds)  log::info("a: {}", i++);
    RM(m_fallStartY)  log::info("a: {}", i++);
    RM(m_disablePlayerSqueeze)  log::info("a: {}", i++);
    RM(m_robotHasRun3)  log::info("a: {}", i++);
    RM(m_robotHasRun2)  log::info("a: {}", i++);
    RM(m_item20)  log::info("a: {}", i++);
    RM(m_ignoreDamage)  log::info("a: {}", i++);
    RM(m_enable22Changes)  log::info("a: {}", i++);
    RM(m_unk838)  log::info("a: {}", i++);
    RM(m_stateOnGround)  log::info("a: {}", i++);
    RM(m_stateUnk)  log::info("a: {}", i++);
    RM(m_stateNoStickX)  log::info("a: {}", i++);
    RM(m_stateNoStickY)  log::info("a: {}", i++);
    RM(m_stateUnk2)  log::info("a: {}", i++);
    RM(m_stateBoostX)  log::info("a: {}", i++);
    RM(m_stateBoostY)  log::info("a: {}", i++);
    RM(m_maybeStateForce2)  log::info("a: {}", i++);
    RM(m_stateScale)  log::info("a: {}", i++);
    RM(m_platformerXVelocity)  log::info("a: {}", i++);
    RM(m_holdingRight)  log::info("a: {}", i++);
    RM(m_holdingLeft)  log::info("a: {}", i++);
    RM(m_leftPressedFirst)  log::info("a: {}", i++);
    RM(m_scaleXRelated)  log::info("a: {}", i++);
    RM(m_maybeHasStopped)  log::info("a: {}", i++);
    RM(m_xVelocityRelated)  log::info("a: {}", i++);
    RM(m_maybeGoingCorrectSlopeDirection)  log::info("a: {}", i++);
    RM(m_isSliding)  log::info("a: {}", i++);
    RM(m_maybeSlopeForce)  log::info("a: {}", i++);
    RM(m_isOnIce)  log::info("a: {}", i++);
    RM(m_physDeltaRelated)  log::info("a: {}", i++);
    RM(m_isOnGround4)  log::info("a: {}", i++);
    RM(m_maybeSlidingTime)  log::info("a: {}", i++);
    RM(m_maybeSlidingStartTime)  log::info("a: {}", i++);
    RM(m_changedDirectionsTime)  log::info("a: {}", i++);
    RM(m_slopeEndTime)  log::info("a: {}", i++);
    RM(m_isMoving)  log::info("a: {}", i++);
    RM(m_platformerMovingLeft)  log::info("a: {}", i++);
    RM(m_platformerMovingRight)  log::info("a: {}", i++);
    RM(m_isSlidingRight)  log::info("a: {}", i++);
    RM(m_maybeChangedDirectionAngle)  log::info("a: {}", i++);
    RM(m_unkUnused2)  log::info("a: {}", i++);
    RM(m_isPlatformer)  log::info("a: {}", i++);
    RM(m_stateNoAutoJump)  log::info("a: {}", i++);
    RM(m_stateDartSlide)  log::info("a: {}", i++);
    RM(m_stateHitHead)  log::info("a: {}", i++);
    RM(m_stateFlipGravity)  log::info("a: {}", i++);
    RM(m_gravityMod)  log::info("a: {}", i++);
    RM(m_stateForce)  log::info("a: {}", i++);
    RM(m_stateForceVector)  log::info("a: {}", i++);
    RM(m_affectedByForces)  log::info("a: {}", i++);
    RM(m_jumpPadRelated)  log::info("a: {}", i++);
    RM(m_somethingPlayerSpeedTime)  log::info("a: {}", i++);
    RM(m_playerSpeedAC)  log::info("a: {}", i++);
    RM(m_fixRobotJump)  log::info("a: {}", i++);
    RM(m_holdingButtons)  log::info("a: {}", i++);
    RM(m_inputsLocked)  log::info("a: {}", i++);
    RM(m_currentRobotAnimation)  log::info("a: {}", i++);
    RM(m_gv0123)  log::info("a: {}", i++);
    RM(m_iconRequestID)  log::info("a: {}", i++);
    RM(m_unkUnused)  log::info("a: {}", i++);
}

void TrajectoryBGL::simulateTrajectory(bool press, PlayerObject* player)
{
    float delta = 0.5f;
    int iterations = 240;

    m_fields->trajectoryPlayer->releaseButton(PlayerButton::Jump);

    if (press)
        m_fields->trajectoryPlayer->pushButton(PlayerButton::Jump);

    if (m_fields->trajectoryPlayer->m_dashFireSprite)
        m_fields->trajectoryPlayer->m_dashFireSprite->setVisible(false);

    if (m_fields->trajectoryPlayer->m_dashParticles)
        m_fields->trajectoryPlayer->m_dashParticles->setVisible(false);

    if (m_fields->trajectoryPlayer->m_regularTrail)
        m_fields->trajectoryPlayer->m_regularTrail->setVisible(false);

    if (m_fields->trajectoryPlayer->m_ghostTrail)
        m_fields->trajectoryPlayer->m_ghostTrail->setVisible(false);

    for (size_t i = 0; i < iterations; i++)
    {
        m_fields->trajectoryPlayer->m_isDead = false;

        m_fields->trajectoryPlayer->update(delta);
        this->checkCollisions(m_fields->trajectoryPlayer, delta, false);
        
        drawSegmentAlternative(m_fields->trajectoryDraw, m_fields->trajectoryPlayer->getPosition(), m_fields->point, 0.5f, m_fields->trajectoryPlayer->m_isDead ? ccc4f(1, 0, 0, 1) : ccc4f(0, press ? 1 : 0.45f, 0, 1));
        m_fields->point = m_fields->trajectoryPlayer->getPosition();

        if (m_fields->trajectoryPlayer->m_isDead)
        {
            CCPoint squareSize = m_fields->trajectoryPlayer->getObjectRect().size;
            CCPoint squarePosition = m_fields->trajectoryPlayer->getPosition();

            CCPoint squareVertices[] = {
                ccp(squarePosition.x - squareSize.x / 2, squarePosition.y - squareSize.y / 2), // Bottom-left
                ccp(squarePosition.x + squareSize.x / 2, squarePosition.y - squareSize.y / 2), // Bottom-right
                ccp(squarePosition.x + squareSize.x / 2, squarePosition.y + squareSize.y / 2), // Top-right
                ccp(squarePosition.x - squareSize.x / 2, squarePosition.y + squareSize.y / 2)  // Top-left
            };

            m_fields->trajectoryDraw->drawPolygon(squareVertices, 4, ccc4f(0, 0, 0, 0), 0.35f, ccc4f(1, 0, 0, 1));
            return;
        }
    }
}

$execute
{
    Loader::get()->queueInMainThread([] {
        Client::GetModule("show-trajectory")->onToggle = [](bool enabled){
            if (auto bgl = as<TrajectoryBGL*>(GJBaseGameLayer::get()))
            {
                if (!enabled)
                    bgl->m_fields->trajectoryDraw->clear();
            }
        };
    });
}