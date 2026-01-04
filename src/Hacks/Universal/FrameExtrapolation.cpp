#include "../../Client/Module.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class FrameExtrapolation : public Module
{
    public:
        MODULE_SETUP(FrameExtrapolation)
        {
            setName("Frame Extrapolation");
            setID("frame-extrapolation");
            setCategory("Universal");
            setDescription("Smooths between frames by predicting where the player will be the next frame using its velocity.");
        }
};

SUBMIT_HACK(FrameExtrapolation)

class $modify (ExtrapolatedGameLayer, GJBaseGameLayer)
{
    struct Fields
    {
        float timeTilNextTick = 0;
        float progressTilNextTick = 0;
        
        CCPoint lastCamPos2;
        CCPoint lastCamPos;
        float modifiedDeltaReturn = 0;
    };

    float getModifiedDelta(float dt)
    {
        auto pRet = GJBaseGameLayer::getModifiedDelta(dt);

        m_fields->modifiedDeltaReturn = pRet;

        return pRet;
    }

    virtual void update(float dt)
    {
        GJBaseGameLayer::update(dt);

        if (!typeinfo_cast<PlayLayer*>(this))
            return;

        if (!FrameExtrapolation::get()->getRealEnabled())
        {
            return;
        }

        if (isFlipping())
            return;

        auto self = m_fields.self();

        if (isRunning() && dt != 0 && !PlayLayer::get()->m_levelEndAnimationStarted)
        {
            if (self->modifiedDeltaReturn != 0)
            {
                self->timeTilNextTick = self->modifiedDeltaReturn;
                self->progressTilNextTick = 0;
                self->lastCamPos2 = self->lastCamPos;
                self->lastCamPos = m_objectLayer->getPosition();
            }
            else
            {
                self->progressTilNextTick += dt;
            }

            if (self->timeTilNextTick == 0)
                return;

            // the percentage towards the next tick we are
            float percent = self->progressTilNextTick / self->timeTilNextTick;
            auto endCamPos = self->lastCamPos + (self->lastCamPos - self->lastCamPos2);
            //endCamPos = m_gameState.m_cameraPosition2 - m_gameState.m_cameraOffset;

            //log::info("xPos: {}, {}", .x, m_gameState.m_cameraOffset.x);
            
            m_objectLayer->setPosition(std::lerp<double>(self->lastCamPos.x, endCamPos.x, percent), std::lerp<double>(self->lastCamPos.y, endCamPos.y, percent));

            extrapolateGround(m_groundLayer, percent);
            extrapolateGround(m_groundLayer2, percent);

            extrapolatePlayer(m_player1, percent);

            if (m_player2)
                extrapolatePlayer(m_player2, percent);
        }
    }

    float playerGetRotatedHitbox(PlayerObject* player)
    {
        float rot = 0;

        if (player->m_isSideways)
        {
            rot = -90;
        }

        return rot;
    }

    void extrapolatePlayer(PlayerObject* player, float percent)
    {
        float endXPos = player->m_position.x + (player->m_position.x - player->m_lastPosition.x);
        float endYPos = player->m_position.y + (player->m_position.y - player->m_lastPosition.y);

        float rotateSpeed = (player->m_isBall && player->m_isBallRotating) ? 1.0 : player->m_rotateSpeed;
        float endRot = ((player->m_rotationSpeed * rotateSpeed) / 240.0f);

        player->CCNode::setPosition(ccp(std::lerp<double>(player->m_position.x, endXPos, percent), std::lerp<double>(player->m_position.y, endYPos, percent)));
        player->m_mainLayer->setRotation(std::lerp(0, endRot, percent) + playerGetRotatedHitbox(player));
    }

    void extrapolateGround(GJGroundLayer* ground, float percent)
    {
        auto self = m_fields.self();

        float moveBy = (self->lastCamPos.x - self->lastCamPos2.x); // should multiply by ground speed, but idk the member, oh wait, am i confusing it with middleground speed? idk.

        for (auto child : CCArrayExt<CCNode*>(ground->getChildren()))
        {
            if (typeinfo_cast<CCSpriteBatchNode*>(child))
            {
                child->setPositionX(std::lerp<double>(0, moveBy, percent));
            }
        }
    }
};