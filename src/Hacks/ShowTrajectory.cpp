/*#ifndef GEODE_IS_MACOS

#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/EffectGameObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

CCPoint startPos = CCPointZero;
bool ignoreStuff = false;
Module* trajectoryMod = nullptr;

class $modify (PlayerObjectExt, PlayerObject)
{
    struct Fields {
        bool isHeld;
    };

    void pushButton(PlayerButton p0)
    {
        PlayerObject::pushButton(p0);

        if (p0 == PlayerButton::Jump)
            m_fields->isHeld = true;
    }

    void releaseButton(PlayerButton p0)
    {
        PlayerObject::releaseButton(p0);

        if (p0 == PlayerButton::Jump)
            m_fields->isHeld = false;
    }
};

/*class $modify (EffectGameObject)
{
    virtual TodoReturn triggerObject(GJBaseGameLayer* p0, int p1, gd::vector<int> const* p2)
    {
        if (!ignoreStuff)
            EffectGameObject::triggerObject(p0, p1, p2);
    }
};* /


class $modify (GJBaseGameLayer)
{
    void gameEventTriggered(GJGameEvent p0, int p1, int p2)
    {
        //if (!ignoreStuff)
            GJBaseGameLayer::gameEventTriggered(p0, p1, p2);
    }

    void playerWillSwitchMode(PlayerObject* p0, GameObject* p1)
    {
        if (p0->getID() == "trajectory-player"_spr || ignoreStuff)
            return;

        GJBaseGameLayer::playerWillSwitchMode(p0, p1);
    }

    bool canBeActivatedByPlayer(PlayerObject* p0, EffectGameObject* p1)
    {
        if (p0->getID() == "trajectory-player"_spr)
        {
            //if (p1->m_objectType == GameObjectType::Fa)
                //return false;
        }

        return GJBaseGameLayer::canBeActivatedByPlayer(p0, p1);
    }

    void destroyObject(GameObject* p0)
    {
        if (!ignoreStuff)
            GJBaseGameLayer::destroyObject(p0);
    }

    static void onModify(auto& self) {
        std::vector<geode::Hook*> hooks;

        hooks.push_back(self.getHook("GJBaseGameLayer::destroyObject").unwrap());
        hooks.push_back(self.getHook("GJBaseGameLayer::canBeActivatedByPlayer").unwrap());
        hooks.push_back(self.getHook("GJBaseGameLayer::playerWillSwitchMode").unwrap());
        hooks.push_back(self.getHook("GJBaseGameLayer::gameEventTriggered").unwrap());

        Loader::get()->queueInMainThread([hooks] 
        {
            auto modu = Client::GetModule("show-trajectory");

            for (auto hook : hooks)
            {
                modu->addHook(hook);
            }
        });
    }
};

class $modify (PlayLayer)
{
    CCDrawNode* dn;
    PlayerObject* plr;
    PlayerObject* ship;
    CCLayer* mainLayer = nullptr;

    virtual void destroyPlayer(PlayerObject* p0, GameObject* p1)
    {
        if ((p0 && p0->getID() == "trajectory-player"_spr) || ignoreStuff)
        {
            p0->m_isDead = true;

            return;
        }

        PlayLayer::destroyPlayer(p0, p1);
    }

    static void onModify(auto& self) {
        std::vector<geode::Hook*> hooks;

        hooks.push_back(self.getHook("PlayLayer::destroyPlayer").unwrap());
        hooks.push_back(self.getHook("PlayLayer::incrementJumps").unwrap());
        hooks.push_back(self.getHook("PlayLayer::playEndAnimationToPos").unwrap());
        //hooks.push_back(self.getHook("PlayLayer::init").unwrap());
        //hooks.push_back(self.getHook("PlayLayer::postUpdate").unwrap());

        Loader::get()->queueInMainThread([hooks] 
        {
            auto modu = Client::GetModule("show-trajectory");

            for (auto hook : hooks)
            {
                modu->addHook(hook);
            }
        });
    }

    void incrementJumps()
    {
        if (!ignoreStuff)
            PlayLayer::incrementJumps();
    }

    void playEndAnimationToPos(cocos2d::CCPoint p0)
    {
        if (!ignoreStuff)
            PlayLayer::playEndAnimationToPos(p0);
    }

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
    {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        if (!m_fields->mainLayer)
        {
            if (auto mainNode = getChildOfType<CCNode>(this, 1))
            {
                if (auto l = getChildOfType<CCLayer>(mainNode, 0))
                {
                    m_fields->mainLayer = l;
                }
            }
        }

        trajectoryMod = Client::GetModule("show-trajectory");

        auto plr = PlayerObject::create(1, 1, this, m_fields->mainLayer, false);
	    plr->setPosition({0, 105});
	    plr->setVisible(false);
        plr->setID("trajectory-player"_spr);
	    m_fields->mainLayer->addChild(plr);
        plr->pushButton(PlayerButton::Jump);

        auto ship = PlayerObject::create(1, 1, this, m_fields->mainLayer, false);
	    ship->setPosition({0, 105});
	    ship->setVisible(false);
        ship->setID("trajectory-player"_spr);
	    m_fields->mainLayer->addChild(ship);
        ship->pushButton(PlayerButton::Jump);
        ship->toggleFlyMode(true, true);

        CCPoint point = plr->getPosition();

        auto dn = CCDrawNode::create();
        dn->setID("trajectory-node"_spr);
        m_fields->mainLayer->addChild(dn);

        m_fields->dn = dn;
        m_fields->plr = plr;
        m_fields->ship = ship;

        return true;
    }

    int getGamemode(PlayerObject* plr)
    {
        if (plr->m_isShip)
            return 1;

        if (plr->m_isBall)
            return 2;

        if (plr->m_isBird)
            return 3;

        if (plr->m_isDart)
            return 4;

        if (plr->m_isRobot)
            return 5;

        if (plr->m_isSpider)
            return 6;

        if (plr->m_isSwing)
            return 7;

        return 0;
    }

    void perform(PlayerObject* plr, bool first = false)
    {
        auto dn = m_fields->dn;

        int updateRate = 5;
        int steps = 150;

        bool held = as<PlayerObjectExt*>(m_player1)->m_fields->isHeld == first;

        CCPoint point = /*(first && (getGamemode(m_player1) == 0 || getGamemode(m_player1) == 5)) ? m_player1->m_lastGroundedPos : * /m_player1->getPosition();
        plr->setPosition(point);
        plr->m_isPlatformer = m_player1->m_isPlatformer;
        plr->m_isUpsideDown = m_player1->m_isUpsideDown;
        plr->m_isDead = false;
        plr->m_collidedObject = nullptr;
        plr->m_isOnGround = m_player1->m_isOnGround;
        plr->m_isOnSlope = m_player1->m_isOnSlope;
        plr->m_wasOnSlope = m_player1->m_wasOnSlope;
        plr->m_isDashing = m_player1->m_isDashing;
        plr->m_vehicleSize = m_player1->m_vehicleSize;

        plr->m_yVelocity = m_player1->m_yVelocity;
        plr->m_collidedObject = m_player1->m_collidedObject;
        plr->m_vehicleSize = m_player1->m_vehicleSize;
        plr->m_playerSpeed = m_player1->m_playerSpeed;

        //plr->m_isShip = m_player1->m_isShip;
        plr->m_isBall = m_player1->m_isBall;
        plr->m_isBird = m_player1->m_isBird;
        plr->m_isDart = m_player1->m_isDart;
        plr->m_isRobot = m_player1->m_isRobot;
        plr->m_isSpider = m_player1->m_isSpider;
        plr->m_isSwing = m_player1->m_isSwing;

        for (size_t i = 0; i < updateRate * steps; i++) {
            plr->m_isDead = false;

            if (first && !plr->m_isSpider)
                plr->pushButton(PlayerButton::Jump);

            plr->update(0.2f);
            plr->updateSpecial(0.2f);
            this->checkCollisions(plr, 0.2f, false);
            
            dn->drawSegment(point, plr->getPosition(), 1, plr->m_isDead ? ccc4f(1, 0, 0, 1) : ccc4f(0, held ? 1 : 0.45f, 0, 1));
            point = plr->getPosition();

            if (plr->m_isDead)
            {
                plr->m_isDead = false;

                CCPoint squareSize = plr->getObjectRect().size;
                CCPoint squarePosition = plr->getPosition();

                CCPoint squareVertices[] = {
                    ccp(squarePosition.x - squareSize.x / 2, squarePosition.y - squareSize.y / 2), // Bottom-left
                    ccp(squarePosition.x + squareSize.x / 2, squarePosition.y - squareSize.y / 2), // Bottom-right
                    ccp(squarePosition.x + squareSize.x / 2, squarePosition.y + squareSize.y / 2), // Top-right
                    ccp(squarePosition.x - squareSize.x / 2, squarePosition.y + squareSize.y / 2)  // Top-left
                };

                dn->drawPolygon(squareVertices, 4, ccc4f(0, 0, 0, 0), 0.35f, ccc4f(1, 0, 0, 1));

                return;
            }
        }
    }

    virtual void postUpdate(float p0)
    {
        ignoreStuff = false;

        PlayLayer::postUpdate(p0);

        auto dn = m_fields->dn;
        auto plr = m_player1->m_isShip ? m_fields->ship : m_fields->plr;

        dn->clear();

        if (!trajectoryMod->enabled)
            return;

        ignoreStuff = true;

        plr->pushButton(PlayerButton::Jump);
        perform(plr, true);

        plr->releaseButton(PlayerButton::Jump);
        perform(plr);

        ignoreStuff = false;
    }
};

#endif*/