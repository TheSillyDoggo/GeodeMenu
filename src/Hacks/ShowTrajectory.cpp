/*#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

class $modify (PlayLayer)
{
    CCDrawNode* dn;
    PlayerObject* plr;
    CCLayer* mainLayer = nullptr;

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

        auto plr = PlayerObject::create(1, 1, this, m_fields->mainLayer, false);
	    plr->setPosition({0, 105});
	    plr->setVisible(false);
	    m_fields->mainLayer->addChild(plr);
        plr->pushButton(PlayerButton::Jump);

        CCPoint point = plr->getPosition();

        auto dn = CCDrawNode::create();
        m_fields->mainLayer->addChild(dn);

        m_fields->dn = dn;
        m_fields->plr = plr;

        return true;
    }

    virtual TodoReturn postUpdate(float p0)
    {
        PlayLayer::postUpdate(p0);

        auto dn = m_fields->dn;
        auto plr = m_fields->plr;

        dn->clear();

        int updateRate = 5;
        int steps = 150;

        CCPoint point = m_player1->getPosition();
        plr->setPosition(point);
        plr->m_isPlatformer = m_player1->m_isPlatformer;
        plr->m_isUpsideDown = plr->m_isUpsideDown;
        plr->m_isDead = false;
        plr->m_vehicleSize = m_player1->m_vehicleSize;

        plr->m_isShip = m_player1->m_isShip;
        plr->m_isBall = m_player1->m_isBall;
        plr->m_isBird = m_player1->m_isBird;
        plr->m_isDart = m_player1->m_isDart;
        plr->m_isRobot = m_player1->m_isRobot;
        plr->m_isSpider = m_player1->m_isSpider;
        plr->m_isSwing = m_player1->m_isSwing;

        for (size_t i = 0; i < updateRate * steps; i++) {
            plr->update(0.2f);
            plr->updateSpecial(0.2f);
            this->checkCollisions(plr, 1, false);
            
            dn->drawSegment(point, plr->getPosition(), 1, ccc4f(0, 1, 0, 1));
            point = plr->getPosition();

            if (plr->m_isDead)
                break;
        }
    }
};*/