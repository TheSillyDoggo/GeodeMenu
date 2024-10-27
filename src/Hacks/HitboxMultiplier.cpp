/*#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GameObject)
{
    virtual cocos2d::CCRect getObjectRect(float p0, float p1)
    {
        auto r = GameObject::getObjectRect(p0, p1);

        if (!typeinfo_cast<PlayerObject*>(this))
        {
            r.size *= 0.5f;
            r.origin += r.size / 2;
        }

        return r;
    }
};*/