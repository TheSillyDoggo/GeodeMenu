#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/OBB2D.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Module.hpp>

using namespace geode::prelude;

class AccurateSpikeHitboxes : public Module
{
    public:
        MODULE_SETUP(AccurateSpikeHitboxes)
        {
            setID("accurate-spike-hitboxes");
            setCategory("Level");
            setSafeModeTrigger(SafeModeTrigger::LevelLoad);
        }
};

SUBMIT_HACK(AccurateSpikeHitboxes);

class $modify (BetterOBB2D, OBB2D)
{
    struct Fields
    {
        bool isBetter = false;
    };

    // stolen from stackoverflow
    // adapted to cocos by meee :3c
    static void projectOntoAxis(const std::array<cocos2d::CCPoint,4>& corners, const cocos2d::CCPoint& axis, float& min, float& max)
    {
        min = max = corners[0].x * axis.x + corners[0].y * axis.y;

        for (int i = 1; i < 4; i++)
        {
            float proj = corners[i].x * axis.x + corners[i].y * axis.y;
            min = std::min(min, proj);
            max = std::max(max, proj);
        }
    }

    static bool overlapOnAxis(const std::array<cocos2d::CCPoint,4>& a, const std::array<cocos2d::CCPoint,4>& b, const cocos2d::CCPoint& axis)
    {
        float minA, maxA;
        float minB, maxB;

        projectOntoAxis(a, axis, minA, maxA);
        projectOntoAxis(b, axis, minB, maxB);

        return !(maxA < minB || maxB < minA);
    }

    bool betteroverlaps1Way(OBB2D* other)
    {
        const auto& a = m_corners;
        const auto& b = other->m_corners;

        for (int i = 0; i < 4; i++)
        {
            CCPoint p1 = a[i];
            CCPoint p2 = a[(i + 1) % 4];

            CCPoint edge = ccpSub(p2, p1);
            CCPoint axis = ccp(-edge.y, edge.x);

            if (!overlapOnAxis(a, b, axis))
                return false;
        }

        return true;
    }

    bool overlaps1Way(OBB2D* other)
    {
        if (m_fields->isBetter)
            return betteroverlaps1Way(other);

        return OBB2D::overlaps1Way(other);
    }

    cocos2d::CCRect getBoundingRect()
    {
        auto ret = OBB2D::getBoundingRect();

        if (m_fields->isBetter) // pyrocynical
            ret.inflateRect(100);

        return ret;
    }
};

#define SIZE_CHK(width, height) \
if (m_width * m_spriteWidthScale == width && m_height * m_spriteHeightScale == height) \
    return true; \
if (m_width * m_spriteWidthScale == height && m_height * m_spriteHeightScale == width) \
    return true;

bool __accurateHitboxesEnabledOnLoad = false;

class $modify (AccurateGameObject, GameObject)
{
    bool shouldApplyAccurate()
    {
        if (!__accurateHitboxesEnabledOnLoad)
            return false;

        SIZE_CHK(6, 12);
        SIZE_CHK(6, 5.6f);
        SIZE_CHK(4, 7.6f);
        
        if (m_objectID == 392 || m_objectID == 458 || m_objectID == 459)
            return true;

        return false;
    }

    virtual void updateOrientedBox()
    {
        bool dirty = m_isOrientedBoxDirty || !m_orientedBox;

        GameObject::updateOrientedBox();

        if (dirty)
        {
            if (typeinfo_cast<PlayerObject*>(this))
                static_cast<BetterOBB2D*>(m_orientedBox)->m_fields->isBetter = true;

            if (shouldApplyAccurate())
            {                
                static_cast<BetterOBB2D*>(m_orientedBox)->m_fields->isBetter = true;
                auto rot = getRotation();

                if (m_isFlipY)
                    rot += 180;

                auto center = m_orientedBox->m_center;
                float angle = -CC_DEGREES_TO_RADIANS(rot);
                float width = m_width * m_scaleX;
                float height = m_height * m_scaleY;

                float cosAngle = cos(angle);
                float sinAngle = sin(angle);

                // should be 0.50, but that causes weird collision bugs that makes some lvls impossible
                CCPoint x = CCPoint(cosAngle, sinAngle) * width * 0.49f;
                CCPoint y = CCPoint(-sinAngle, cosAngle) * height * 0.49f;

                m_orientedBox->m_corners[0] = center - x - y;
                m_orientedBox->m_corners[1] = center - y;
                m_orientedBox->m_corners[2] = center + x - y;
                m_orientedBox->m_corners[3] = center + y;

                m_orientedBox->computeAxes();
                m_orientedBox->orderCorners();
            }
        }
    }
};

class $modify (PlayLayer)
{
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
    {
        __accurateHitboxesEnabledOnLoad = AccurateSpikeHitboxes::get()->getRealEnabled();
        return PlayLayer::init(level, useReplay, dontCreateObjects);
    }

    void addObject(GameObject* go)
    {
        if (static_cast<AccurateGameObject*>(go)->shouldApplyAccurate())
        {
            auto rot = go->getRotation();

            if ((int)rot % 90 == 0)
                go->setRotation(rot + 1);
        }

        PlayLayer::addObject(go);
    }
};