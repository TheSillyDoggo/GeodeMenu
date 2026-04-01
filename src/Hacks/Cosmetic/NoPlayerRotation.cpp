#include "../../Client/Module.hpp"
#include <Geode/modify/PlayerObject.hpp>
#include <VisitHook.hpp>

using namespace geode::prelude;

class NoPlayerRotation : public Module
{
    public:
        MODULE_SETUP(NoPlayerRotation)
        {
            setName("No Rotation");
            setID("no-rot");
            setCategory("Cosmetic");
            setDescription("Disables rotation on players");
        }
};

class NoRotCube : public Module
{
    public:
        MODULE_SETUP(NoRotCube)
        {
            setID("no-rot/cube");
            setDefaultEnabled(true);
            setPriority(1);
        }
};

class NoRotShip : public Module
{
    public:
        MODULE_SETUP(NoRotShip)
        {
            setID("no-rot/ship");
            setDefaultEnabled(true);
            setPriority(2);
        }
};

class NoRotJetpack : public Module
{
    public:
        MODULE_SETUP(NoRotJetpack)
        {
            setID("no-rot/jetpack");
            setDefaultEnabled(true);
            setPriority(3);
        }
};

class NoRotBall : public Module
{
    public:
        MODULE_SETUP(NoRotBall)
        {
            setID("no-rot/ball");
            setDefaultEnabled(true);
            setPriority(4);
        }
};

class NoRotUfo : public Module
{
    public:
        MODULE_SETUP(NoRotUfo)
        {
            setID("no-rot/ufo");
            setDefaultEnabled(true);
            setPriority(5);
        }
};

class NoRotWave : public Module
{
    public:
        MODULE_SETUP(NoRotWave)
        {
            setID("no-rot/wave");
            setDefaultEnabled(true);
            setPriority(6);
        }
};

class NoRotRobot : public Module
{
    public:
        MODULE_SETUP(NoRotRobot)
        {
            setID("no-rot/robot");
            setDefaultEnabled(true);
            setPriority(7);
        }
};

class NoRotSpider : public Module
{
    public:
        MODULE_SETUP(NoRotSpider)
        {
            setID("no-rot/spider");
            setDefaultEnabled(true);
            setPriority(8);
        }
};

class NoRotSwing : public Module
{
    public:
        MODULE_SETUP(NoRotSwing)
        {
            setID("no-rot/swing");
            setDefaultEnabled(true);
            setPriority(9);
        }
};

SUBMIT_HACK(NoPlayerRotation);
SUBMIT_OPTION(NoPlayerRotation, NoRotCube);
SUBMIT_OPTION(NoPlayerRotation, NoRotShip);
SUBMIT_OPTION(NoPlayerRotation, NoRotJetpack);
SUBMIT_OPTION(NoPlayerRotation, NoRotBall);
SUBMIT_OPTION(NoPlayerRotation, NoRotUfo);
SUBMIT_OPTION(NoPlayerRotation, NoRotWave);
SUBMIT_OPTION(NoPlayerRotation, NoRotRobot);
SUBMIT_OPTION(NoPlayerRotation, NoRotSpider);
SUBMIT_OPTION(NoPlayerRotation, NoRotSwing);

namespace qolmod
{
    class PlayerObjectVisitHook : public qolmod::VisitHook
    {
        public:
            CREATE_FUNC(PlayerObjectVisitHook)
            PlayerObject* player = nullptr;
            float rot = 0;
            float rot2 = 0;

            bool noRotateEnabled(PlayerObject* po)
            {
                if (!NoPlayerRotation::get()->getRealEnabled())
                    return false;

                if (po->m_isShip)
                {
                    if (po->m_isPlatformer)
                        return NoRotJetpack::get()->getRealEnabled();
                    else
                        return NoRotShip::get()->getRealEnabled();
                }
                else if (po->m_isBall)
                {
                    return NoRotBall::get()->getRealEnabled();
                }
                else if (po->m_isBird)
                {
                    return NoRotUfo::get()->getRealEnabled();
                }
                else if (po->m_isDart)
                {
                    return NoRotWave::get()->getRealEnabled();
                }
                else if (po->m_isRobot)
                {
                    return NoRotRobot::get()->getRealEnabled();
                }
                else if (po->m_isSpider)
                {
                    return NoRotSpider::get()->getRealEnabled();
                }
                else if (po->m_isSwing)
                {
                    return NoRotSwing::get()->getRealEnabled();
                }

                return NoRotCube::get()->getRealEnabled();
            }

            virtual void preVisit()
            {
                rot = player->getRotation();
                rot2 = player->m_iconSprite->getRotation();

                if (noRotateEnabled(player))
                {
                    player->setRotation(0);
                    player->m_iconSprite->setRotation(0);
                }
            }

            virtual void postVisit(cocos2d::CCNode* node)
            {
                player->setRotation(rot);
                player->m_iconSprite->setRotation(rot2);
            }
    };
};

class $modify (PlayerObject)
{
    bool init(int player, int ship, GJBaseGameLayer* gameLayer, cocos2d::CCLayer* layer, bool playLayer)
    {
        if (!PlayerObject::init(player, ship, gameLayer, layer, playLayer))
            return false;

        auto vh = qolmod::PlayerObjectVisitHook::create();
        vh->player = this;

        this->setGrid(vh);
        return true;
    }
};