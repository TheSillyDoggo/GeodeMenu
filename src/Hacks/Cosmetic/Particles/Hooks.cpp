#include "NoParticlesClassic.hpp"
#include "NoParticlesAdv.hpp"
#include "../../Utils/PlayLayer.hpp"
#include <Geode/modify/CCParticleSystemQuad.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GameObject.hpp>

#define CHECK_IF_PARTICLE($func, $member) \
if ($func::get()->getRealEnabled() && fields->$member) \
    return;

class $modify (NoParticleSystem, CCParticleSystemQuad)
{
    struct Fields
    {
        bool particleObject = false;
        bool playerGroundParticles = false;
        bool playerLandParticles = false;
        bool playerTrailParticles = false;
        bool shipBoostParticles = false;
        bool ufoClickParticles = false;
        bool shipGroundParticles = false;
        bool robotBoostParticles = false;
        bool glitterParticle = false;

        bool gravityPortalParticles = false;
        bool gamemodePortalParticles = false;
        bool dualPortalParticles = false;
        bool sizePortalParticles = false;
        bool mirrorPortalParticles = false;
        bool speedPortalParticles = false;
        bool padParticle = false;
        bool ringParticle = false;
        bool endscreenParticle = false;
        bool swingFireParticles = false;
        bool collectableParticles = false;
    };

    virtual void draw()
    {
        if (typeinfo_cast<CCParticleSnow*>(this))
            return CCParticleSystemQuad::draw();

        if (NoParticlesClassic::get()->getRealEnabled() ? (ParticlesInEditor::get()->getRealEnabled() ? LevelEditorLayer::get() != nullptr : false) : true)
        {
            if (NoParticlesAdv::get()->getRealEnabled())
            {
                auto fields = m_fields.self();

                CHECK_IF_PARTICLE(ParticleObjects, particleObject);
                CHECK_IF_PARTICLE(PlayerGroundParticles, playerGroundParticles);
                CHECK_IF_PARTICLE(PlayerLandParticles, playerLandParticles);
                CHECK_IF_PARTICLE(PlayerTrailParticles, playerTrailParticles);
                CHECK_IF_PARTICLE(ShipHoldParticles, shipBoostParticles);
                CHECK_IF_PARTICLE(UfoClickParticles, ufoClickParticles);
                CHECK_IF_PARTICLE(ShipGroundParticles, shipGroundParticles);
                CHECK_IF_PARTICLE(RobotHoldParticles, robotBoostParticles);
                CHECK_IF_PARTICLE(GlitterParticles, glitterParticle);
                CHECK_IF_PARTICLE(PadParticles, padParticle);
                CHECK_IF_PARTICLE(RingParticles, ringParticle);
                CHECK_IF_PARTICLE(GravityPortalParticles, gravityPortalParticles);
                CHECK_IF_PARTICLE(GamemodePortalParticles, gamemodePortalParticles);
                CHECK_IF_PARTICLE(DualPortalParticles, dualPortalParticles);
                CHECK_IF_PARTICLE(SizePortalParticles, sizePortalParticles);
                CHECK_IF_PARTICLE(MirrorPortalParticles, mirrorPortalParticles);
                CHECK_IF_PARTICLE(SpeedPortalParticles, speedPortalParticles);
                CHECK_IF_PARTICLE(EndPortalParticles, endscreenParticle);
                CHECK_IF_PARTICLE(SwingFireParticles, swingFireParticles);
                CHECK_IF_PARTICLE(CoinParticles, collectableParticles);

                if (SpeedChangeParticles::get()->getRealEnabled())
                {
                    if (m_sPlistFile.ends_with("speedEffect_normal.plist"))
                        return;

                    if (m_sPlistFile.ends_with("speedEffect_fast.plist"))
                        return;

                    if (m_sPlistFile.ends_with("speedEffect_vfast.plist"))
                        return;

                    if (m_sPlistFile.ends_with("speedEffect_vvfast.plist"))
                        return;
                }
            }

            CCParticleSystemQuad::draw();
        }
    }

    static void onModify(auto& self)
    {
        (void)self.setHookPriorityPost("cocos2d::CCParticleSystemQuad::draw", Priority::Last);
    }
};


class $modify (GJBaseGameLayer)
{
    virtual cocos2d::CCParticleSystemQuad* createCustomParticle(gd::string const& key, cocos2d::ParticleStruct const& particleStruct, int minimum, bool dontAdd)
    {
        auto pRet = GJBaseGameLayer::createCustomParticle(key, particleStruct, minimum, dontAdd);

        if (pRet)
            static_cast<NoParticleSystem*>(pRet)->m_fields->particleObject = true;

        return pRet;
    }
};

class $modify (PlayerObject)
{
    void addAllParticles()
    {
        PlayerObject::addAllParticles();

        if (m_playerGroundParticles)
            static_cast<NoParticleSystem*>(m_playerGroundParticles)->m_fields->playerGroundParticles = true;

        if (m_landParticles0)
            static_cast<NoParticleSystem*>(m_landParticles0)->m_fields->playerLandParticles = true;

        if (m_landParticles1)
            static_cast<NoParticleSystem*>(m_landParticles1)->m_fields->playerLandParticles = true;

        if (m_trailingParticles)
            static_cast<NoParticleSystem*>(m_trailingParticles)->m_fields->playerTrailParticles = true;

        if (m_shipClickParticles)
            static_cast<NoParticleSystem*>(m_shipClickParticles)->m_fields->shipBoostParticles = true;

        if (m_vehicleGroundParticles)
            static_cast<NoParticleSystem*>(m_vehicleGroundParticles)->m_fields->shipGroundParticles = true;

        if (m_robotBurstParticles)
            static_cast<NoParticleSystem*>(m_robotBurstParticles)->m_fields->robotBoostParticles = true;

        if (m_ufoClickParticles)
            static_cast<NoParticleSystem*>(m_ufoClickParticles)->m_fields->ufoClickParticles = true;

        if (m_swingBurstParticles1)
            static_cast<NoParticleSystem*>(m_swingBurstParticles1)->m_fields->swingFireParticles = true;

        if (m_swingBurstParticles2)
            static_cast<NoParticleSystem*>(m_swingBurstParticles2)->m_fields->swingFireParticles = true;
    }
};

class $modify (PlayLayer)
{
    void createObjectsFromSetupFinished()
    {
        PlayLayer::createObjectsFromSetupFinished();

        if (m_glitterParticles)
            static_cast<NoParticleSystem*>(m_glitterParticles)->m_fields->glitterParticle = true;
    }
};

class $modify (GameObject)
{
    cocos2d::CCParticleSystemQuad* createAndAddParticle(int objectType, char const* plistName, int tag, cocos2d::tCCPositionType positionType)
    {
        auto pRet = GameObject::createAndAddParticle(objectType, plistName, tag, positionType);

        if (pRet)
        {
            if (typeinfo_cast<RingObject*>(this))
                static_cast<NoParticleSystem*>(pRet)->m_fields->ringParticle = true;

            auto obj = this;

            if (obj->m_objectType == GameObjectType::YellowJumpPad || obj->m_objectType == GameObjectType::PinkJumpPad || obj->m_objectType == GameObjectType::GravityPad || obj->m_objectType == GameObjectType::PinkJumpPad || obj->m_objectType == GameObjectType::RedJumpPad || obj->m_objectType == GameObjectType::SpiderPad)
                static_cast<NoParticleSystem*>(pRet)->m_fields->padParticle = true;

            if (typeinfo_cast<EndPortalObject*>(this))
                static_cast<NoParticleSystem*>(pRet)->m_fields->endscreenParticle = true;

            if (this->m_objectType == GameObjectType::Collectible || this->m_objectType == GameObjectType::UserCoin || m_objectType == GameObjectType::SecretCoin)
                static_cast<NoParticleSystem*>(pRet)->m_fields->collectableParticles = true;

            switch (obj->m_objectID)
            { //thanks prevter :3
                case 10: // Blue Gravity Portal
                case 11: // Yellow Gravity Portal
                case 2926: // Green Gravity Portal
                    static_cast<NoParticleSystem*>(pRet)->m_fields->gravityPortalParticles = true;
                    break;
                case 12: // Cube Portal
                case 13: // Ship Portal
                case 47: // Ball Portal
                case 111: // UFO Portal
                case 660: // Wave Portal
                case 745: // Robot Portal
                case 1331: // Spider Portal
                case 1933: // SwingCopter Portal
                    static_cast<NoParticleSystem*>(pRet)->m_fields->gamemodePortalParticles = true;
                    break;
                case 286: // Orange Dual Portal
                case 287: // Blue Dual Portal
                    static_cast<NoParticleSystem*>(pRet)->m_fields->dualPortalParticles = true;
                    break;
                case 99: // Normal Size Portal
                case 101: // Mini Portal
                    static_cast<NoParticleSystem*>(pRet)->m_fields->sizePortalParticles = true;
                    break;
                case 45: // Orange Mirror Portal
                case 46: // Blue Mirror Portal
                    static_cast<NoParticleSystem*>(pRet)->m_fields->mirrorPortalParticles = true;
                    break;
                case 200: // -1x Speed Change
                case 201: // 1x Speed Change
                case 202: // 2x Speed Change
                case 203: // 3x Speed Change
                case 1334: // 4x Speed Change
                    static_cast<NoParticleSystem*>(pRet)->m_fields->speedPortalParticles = true;
                    break;
                default:
                    break;
            }
        }

        return pRet;
    }
};