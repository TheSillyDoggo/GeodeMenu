#include <Geode/Geode.hpp>
#include <Geode/modify/CCSpriteFrameCache.hpp>
#include <Geode/modify/EnhancedGameObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (CCSpriteFrameCache)
{
    CCSpriteFrame* spriteFrameByName(const char *pszName)
    {
        if (std::string(pszName).starts_with("secretCoin_2"))
            pszName = utils::string::replace(std::string(pszName), "_2", "").c_str();

        return CCSpriteFrameCache::spriteFrameByName(pszName);
    }

    QOLMOD_MOD_ALL_HOOKS("gold-user-coins")
};

bool is;
int objID;

class $modify (GJBaseGameLayer)
{
    bool hasUniqueCoin(EffectGameObject* p0)
    {
        auto v = GJBaseGameLayer::hasUniqueCoin(p0);

        if (is)
            p0->m_objectID = 142;

        return v;
    }

    QOLMOD_MOD_ALL_HOOKS("gold-user-coins")
};

class $modify (EnhancedGameObject)
{
    void updateUserCoin()
    {
        objID = m_objectID;
        is = true;

        EnhancedGameObject::updateUserCoin();

        this->setColor(ccc3(255, 255, 255));

        is = false;
        m_objectID = objID;
    }

    QOLMOD_MOD_ALL_HOOKS("gold-user-coins")
};