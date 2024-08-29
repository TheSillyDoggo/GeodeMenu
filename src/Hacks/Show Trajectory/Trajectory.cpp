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

    RM(m_wasTeleported)
    RM(m_fixGravityBug)
    RM(m_reverseSync)
    RM(m_yVelocityBeforeSlope)
    RM(m_dashX)
    RM(m_dashY)
    RM(m_dashAngle)
    RM(m_dashStartTime)
    RM(m_dashRing)
    RM(m_slopeStartTime)
    RM(m_justPlacedStreak)
    RM(m_maybeLastGroundObject)
    RM(m_unk50C)
    RM(m_unk510)
    RM(m_currentSlope2)
    RM(m_preLastGroundObject)
    RM(m_slopeAngle)
    RM(m_slopeSlidingMaybeRotated)
    RM(m_quickCheckpointMode)
    RM(m_collidedObject)
    RM(m_lastGroundObject)
    RM(m_collidingWithLeft)
    RM(m_collidingWithRight)
    RM(m_maybeSavedPlayerFrame)
    RM(m_scaleXRelated2)
    RM(m_groundYVelocity)
    RM(m_yVelocityRelated)
    RM(m_scaleXRelated3)
    RM(m_scaleXRelated4)
    RM(m_scaleXRelated5)
    RM(m_isCollidingWithSlope)
    RM(m_isBallRotating)
    RM(m_unk669)
    RM(m_currentSlope3)
    RM(m_currentSlope)
    RM(unk_584)
    RM(m_collidingWithSlopeId)
    RM(m_slopeFlipGravityRelated)
    RM(m_slopeAngleRadians)
    // RM(m_rotateObjectsRelated)
    // RM(m_maybeRotatedObjectsMap)
    RM(m_rotationSpeed)
    RM(m_rotateSpeed)
    RM(m_isRotating)
    RM(m_isBallRotating2)
    RM(m_hasGlow)
    RM(m_isHidden)
    RM(m_speedMultiplier)
    RM(m_yStart)
    RM(m_gravity)
    RM(m_trailingParticleLife)
    RM(m_unk648)
    RM(m_gameModeChangedTime)
    RM(m_padRingRelated)
    RM(m_maybeReducedEffects)
    RM(m_maybeIsFalling)
    RM(m_shouldTryPlacingCheckpoint)
    RM(m_playEffects)
    RM(m_maybeCanRunIntoBlocks)
    RM(m_hasGroundParticles)
    RM(m_hasShipParticles)
    RM(m_isOnGround3)
    RM(m_checkpointTimeout)
    RM(m_lastCheckpointTime)
    RM(m_lastJumpTime)
    RM(m_lastFlipTime)
    RM(m_flashTime)
    RM(m_flashRelated)
    RM(m_flashRelated1)
    RM(m_colorRelated2)
    RM(m_flashRelated3)
    RM(m_lastSpiderFlipTime)
    RM(m_unkBool5)
    RM(m_maybeIsVehicleGlowing)
    RM(m_gv0096)
    RM(m_gv0100)
    RM(m_accelerationOrSpeed)
    RM(m_snapDistance)
    RM(m_ringJumpRelated)
    // RM(m_ringRelatedSet)
    RM(m_objectSnappedTo)
    RM(m_pendingCheckpoint)
    RM(m_maybeSpriteRelated)
    RM(m_slopeRotation)
    RM(m_currentSlopeYVelocity)
    RM(m_unk3d0)
    RM(m_blackOrbRelated)
    RM(m_unk3e0)
    RM(m_unk3e1)
    RM(m_isAccelerating)
    RM(m_isCurrentSlopeTop)
    RM(m_collidedTopMinY)
    RM(m_collidedBottomMaxY)
    RM(m_collidedLeftMaxX)
    RM(m_collidedRightMinX)
    RM(m_streakRelated4)
    RM(m_canPlaceCheckpoint)
    RM(m_colorRelated)
    RM(m_secondColorRelated)
    RM(m_hasCustomGlowColor)
    RM(m_glowColor)
    RM(m_maybeIsColliding)
    RM(m_jumpBuffered)
    RM(m_stateRingJump)
    RM(m_wasJumpBuffered)
    RM(m_wasRobotJump)
    RM(m_stateJumpBuffered)
    RM(m_stateRingJump2)
    RM(m_touchedRing)
    RM(m_touchedCustomRing)
    RM(m_touchedGravityPortal)
    RM(m_maybeTouchedBreakableBlock)
    RM(m_jumpRelatedAC2)
    RM(m_touchedPad)
    RM(m_yVelocity)
    RM(m_fallSpeed)
    RM(m_isOnSlope)
    RM(m_wasOnSlope)
    RM(m_slopeVelocity)
    RM(m_maybeUpsideDownSlope)
    RM(m_isShip)
    RM(m_isBird)
    RM(m_isBall)
    RM(m_isDart)
    RM(m_isRobot)
    RM(m_isSpider)
    RM(m_isUpsideDown)
    RM(m_isDead)
    RM(m_isOnGround)
    RM(m_isGoingLeft)
    RM(m_isSideways)
    RM(m_isSwing)
    RM(m_reverseRelated)
    RM(m_maybeReverseSpeed)
    RM(m_maybeReverseAcceleration)
    RM(m_xVelocityRelated2)
    RM(m_isDashing)
    RM(m_unk9e8)
    RM(m_groundObjectMaterial)
    RM(m_vehicleSize)
    RM(m_playerSpeed)
    RM(m_shipRotation)
    RM(m_lastPortalPos)
    RM(m_unkUnused3)
    RM(m_isOnGround2)
    RM(m_lastLandTime)
    RM(m_platformerVelocityRelated)
    RM(m_maybeIsBoosted)
    RM(m_scaleXRelatedTime)
    RM(m_decreaseBoostSlide)
    RM(m_unkA29)
    RM(m_isLocked)
    RM(m_controlsDisabled)
    RM(m_lastGroundedPos)
    // RM(m_touchedRings)
    RM(m_lastActivatedPortal)
    RM(m_hasEverJumped)
    RM(m_ringOrStreakRelated)
    RM(m_playerColor1)
    RM(m_playerColor2)
    RM(m_position)
    RM(m_isSecondPlayer)
    RM(m_unkA99)
    RM(m_totalTime)
    RM(m_isBeingSpawnedByDualPortal)
    RM(m_unkAAC)
    RM(m_unkAngle1)
    RM(m_yVelocityRelated3)
    RM(m_gamevar0060)
    RM(m_swapColors)
    RM(m_gamevar0062)
    RM(m_followRelated)
    RM(m_playerFollowFloats)
    RM(m_isOutOfBounds)
    RM(m_fallStartY)
    RM(m_disablePlayerSqueeze)
    RM(m_robotHasRun3)
    RM(m_robotHasRun2)
    RM(m_item20)
    RM(m_ignoreDamage)
    RM(m_enable22Changes)
    RM(m_unk838)
    RM(m_stateOnGround)
    RM(m_stateUnk)
    RM(m_stateNoStickX)
    RM(m_stateNoStickY)
    RM(m_stateUnk2)
    RM(m_stateBoostX)
    RM(m_stateBoostY)
    RM(m_maybeStateForce2)
    RM(m_stateScale)
    RM(m_platformerXVelocity)
    RM(m_holdingRight)
    RM(m_holdingLeft)
    RM(m_leftPressedFirst)
    RM(m_scaleXRelated)
    RM(m_maybeHasStopped)
    RM(m_xVelocityRelated)
    RM(m_maybeGoingCorrectSlopeDirection)
    RM(m_isSliding)
    RM(m_maybeSlopeForce)
    RM(m_isOnIce)
    RM(m_physDeltaRelated)
    RM(m_isOnGround4)
    RM(m_maybeSlidingTime)
    RM(m_maybeSlidingStartTime)
    RM(m_changedDirectionsTime)
    RM(m_slopeEndTime)
    RM(m_isMoving)
    RM(m_platformerMovingLeft)
    RM(m_platformerMovingRight)
    RM(m_isSlidingRight)
    RM(m_maybeChangedDirectionAngle)
    RM(m_unkUnused2)
    RM(m_isPlatformer)
    RM(m_stateNoAutoJump)
    RM(m_stateDartSlide)
    RM(m_stateHitHead)
    RM(m_stateFlipGravity)
    RM(m_gravityMod)
    RM(m_stateForce)
    RM(m_stateForceVector)
    RM(m_affectedByForces)
    RM(m_jumpPadRelated)
    RM(m_somethingPlayerSpeedTime)
    RM(m_playerSpeedAC)
    RM(m_fixRobotJump)
    RM(m_holdingButtons)
    RM(m_inputsLocked)
    RM(m_currentRobotAnimation)
    RM(m_gv0123)
    RM(m_iconRequestID)
    RM(m_unkUnused)
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