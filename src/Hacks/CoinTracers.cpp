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
    struct Fields {
        std::vector<GameObject*> coins;

        CCDrawNode* dn;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
    {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        coinTracers = Module::get("coin-tracers");
        coinColour = as<ColourModule*>(coinTracers->options[0]);

        auto dn = CCDrawNode::create();
        m_objectLayer->addChild(dn);

        m_fields->dn = dn;

        return true;
    }

    void addObject(GameObject* p0)
    {
        PlayLayer::addObject(p0);

        if (p0->m_objectType == GameObjectType::UserCoin || p0->m_objectType == GameObjectType::SecretCoin) // 142 
        {
            m_fields->coins.push_back(p0);
        }
    }

    virtual void postUpdate(float p0)
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