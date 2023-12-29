/*#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/CCSpritePlus.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class HitboxNode : public CCDrawNode
{
    public:
        static inline std::vector<GameObject*> objects = {};

        static HitboxNode* create() {
            auto ret = new (std::nothrow) HitboxNode;
            if (ret && ret->init()) {
                ret->autorelease();
                return ret;
            }
            delete ret;
            return nullptr;
        }

        void update(float delta) override
        {
            return;
            this->clear();

            for (size_t i = 0; i < objects.size(); i++)
            {
                constexpr size_t N = 4;
                std::vector<CCPoint> points(N);

                points[0] = ccp(objects[i]->getPosition().x, 0);
                points[1] = ccp(100, 0);
                points[2] = ccp(100, 100);
                points[3] = ccp(0, 100);

                auto color1 = ccc4FFromccc4B(ccColor4B(255, 0, 0, 255));
                cocos2d::_ccColor4F colorA;
                colorA = ccc4FFromccc4B(ccc4(0, 0, 0, 0));

                this->drawPolygon(const_cast<CCPoint*>(points.data()), points.size(), colorA, 0.75f, color1);
            }
        }
};

class $modify (PlayLayer)
{
    bool init(GJGameLevel* p0, bool p1, bool p2)
    {
        if (!PlayLayer::init(p0, p1, p2))
            return false;

        auto obj = HitboxNode::create();
        obj->setPosition(ccp(0, 0));
        obj->setAnchorPoint(ccp(0, 0));
        obj->setID("HitboxNode");
        //schedule update
        obj->scheduleUpdate();

        HitboxNode::objects.clear();

        this->addChild(obj);

        return true;
    }
};*/