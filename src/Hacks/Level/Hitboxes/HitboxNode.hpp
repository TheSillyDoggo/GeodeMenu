#pragma once

#include <Geode/Geode.hpp>

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

class HitboxNode : public cocos2d::CCDrawNode
{
    protected:

        bool shouldFillHitboxes();
        float getHitboxThickness();
        bool shouldObjectDraw(GameObject* obj);
        HitboxColourType getObjectColour(GameObject* obj);
        cocos2d::ccColor3B colourForType(HitboxColourType colour);
        bool objTypeMatchesAny(GameObject* obj, std::vector<GameObjectType> types);
        // thanks prevter
        void forEachObject(GJBaseGameLayer* game, const std::function<void(GameObject*)>& callback);

    public:
        CREATE_FUNC(HitboxNode);

        void drawObjectHitbox(GameObject* obj);

        virtual bool init();
        void updateNode();
};