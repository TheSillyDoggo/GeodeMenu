#include "../../Client/Module.hpp"
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class GravityArrow : public Module
{
    public:
        MODULE_SETUP(GravityArrow)
        {
            setID("gravity-arrow");
            setCategory("Level");
            setSafeModeTrigger(SafeModeTrigger::Attempt);
        }
};

namespace qolmod
{
    class UntransformedNode : public CCNode
    {
        public:
            CREATE_FUNC(UntransformedNode);

            virtual void visit()
            {
                if (!GravityArrow::get()->getRealEnabled())
                    return;

                setRotation(-(getParent()->getRotation() + (static_cast<PlayerObject*>(getParent())->m_isSideways ? -90 : 0)));
                setScale(1.0f / getParent()->getScale());

                CCNode::visit();
            }
    };
};

SUBMIT_HACK(GravityArrow);

class $modify (PlayerObject)
{
    struct Fields
    {
        CCNode* center = nullptr;
        CCSprite* gravSprite = nullptr;
    };

    bool init(int player, int ship, GJBaseGameLayer* gameLayer, cocos2d::CCLayer* layer, bool playLayer)
    {
        if (!PlayerObject::init(player, ship, gameLayer, layer, playLayer))
            return false;

        auto fields = m_fields.self();

        fields->center = qolmod::UntransformedNode::create();
        fields->center->setID("gravity-arrow-center"_spr);
        fields->center->setAdditionalTransform(CCAffineTransformMakeIdentity());

        fields->gravSprite = CCSprite::createWithSpriteFrameName("edit_upBtn_001.png");
        fields->gravSprite->setID("gravity-sprite"_spr);
        fields->center->addChild(fields->gravSprite);
        
        this->addChild(fields->center, 69);
        return true;
    }

    virtual void update(float dt)
    {
        PlayerObject::update(dt);

        auto fields = m_fields.self();

        fields->gravSprite->setPositionY((!m_isUpsideDown ? -1 : 1) * m_vehicleSize * 35);
        fields->gravSprite->setScale((!m_isUpsideDown ? -1 : 1) * m_vehicleSize * 0.8f);
    }
};