#pragma once

#include <Geode/Geode.hpp>
#include <deque>
#include <BaseDrawNode.hpp>

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

class HitboxNode : public qolmod::BaseDrawNode
{
    protected:
        std::vector<PlayerHitboxState> trailStates = {};
        GameObject* onlyObject = nullptr;
        std::unordered_map<PlayerObject*, bool> playerClicks = {};

        cocos2d::CCRect getObjectRect(GameObject* obj);

        virtual bool shouldDrawTrail();
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

        bool drawCircle(cocos2d::CCPoint const& center, float radius, cocos2d::_ccColor4F const& fillColor, float borderWidth, cocos2d::_ccColor4F const& borderColor, unsigned int segments);
        bool isPointOnScreen(cocos2d::CCPoint point);

        virtual void redraw();
        virtual void onTickEnd();

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