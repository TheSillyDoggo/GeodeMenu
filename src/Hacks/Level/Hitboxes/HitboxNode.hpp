#pragma once

#include <Geode/Geode.hpp>
#include <deque>

enum class HitboxColourType
{
    Unk,
    Solid,
    Hazard,
    Passable,
    Interactable,
    PlayerReg,
    PlayerRot,
};

struct PlayerHitboxState
{
    cocos2d::CCRect rectReg;
    cocos2d::CCRect rectBlue;
    char clickState = 0;
};

#if GEODE_COMP_GD_VERSION < 22081
namespace cocos2d
{
    enum class BorderAlignment
    {
        Outside = 0,
        Center = 1,
        Inside = 2
    };
};
#endif

class HitboxNode : public cocos2d::CCDrawNode
{
    protected:
        std::vector<PlayerHitboxState> trailStates = {};
        GameObject* onlyObject = nullptr;
        std::unordered_map<PlayerObject*, bool> playerClicks = {};

        cocos2d::CCRect getObjectRect(GameObject* obj);

        bool shouldFillHitboxes();
        float getHitboxThickness();
        bool shouldObjectDraw(GameObject* obj);
        HitboxColourType getObjectColour(GameObject* obj);
        cocos2d::ccColor3B colourForType(HitboxColourType colour);
        bool objTypeMatchesAny(GameObject* obj, std::vector<GameObjectType> types);
        // thanks prevter
        void forEachObject(GJBaseGameLayer* game, const std::function<void(GameObject*)>& callback);
        void drawLine(cocos2d::CCPoint point1, cocos2d::CCPoint point2, cocos2d::ccColor4F colour, float thickness, cocos2d::CCPoint towards);
        bool shouldRenderState(PlayerHitboxState* state);
        bool drawPolygon(cocos2d::CCPoint *verts, unsigned int count, const cocos2d::ccColor4F &fillColor, float borderWidth, const cocos2d::ccColor4F &borderColor, cocos2d::BorderAlignment alignment = cocos2d::BorderAlignment::Outside);

    public:
        CREATE_FUNC(HitboxNode);

        void drawObjectHitbox(GameObject* obj);
        void drawPlayerHitbox(PlayerObject* plr);

        void drawPlayerTrails();
        void storePlayerTrail(PlayerObject* plr);
        void resetTrails();

        void setOnlyObject(GameObject* go);

        virtual bool init();
        void updateNode();
};