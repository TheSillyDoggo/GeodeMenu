#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/DashRingObject.hpp>
#include <Geode/modify/UILayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* coinTracers = nullptr;
ColourModule* coinColour = nullptr;

class $modify (PlayLayer)
{
    std::vector<GameObject*> coins;

    CCDrawNode* dn;
    CCLayer* mainLayer = nullptr;

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
    {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        coinTracers = Client::GetModule("coin-tracers");
        coinColour = as<ColourModule*>(coinTracers->options[0]);

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

        auto dn = CCDrawNode::create();
        m_debugDrawNode->getParent()->addChild(dn);

        m_fields->dn = dn;

        return true;
    }

    TodoReturn addObject(GameObject* p0)
    {
        PlayLayer::addObject(p0);

        if (p0->m_objectType == GameObjectType::UserCoin || p0->m_objectType == GameObjectType::SecretCoin) // 142 
        {
            m_fields->coins.push_back(p0);
        }
    }

    virtual TodoReturn postUpdate(float p0)
    {
        PlayLayer::postUpdate(p0);

        m_fields->dn->clear();

        if (!coinTracers->enabled)
            return;

        for (auto coin : m_fields->coins)
        {
            if (coin->getOpacity() != 0)
                m_fields->dn->drawSegment(m_player1->getPosition(), coin->getPosition(), 1, ccc4FFromccc3B(coinColour->colour));
        }
    }
};