#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class KeycodeListenerLayer : public geode::Popup<>
{
    protected:
        int currentBind = 0;
        std::function<void(int)> onSucceed = nullptr;

    public:
        static KeycodeListenerLayer* create(int currentBind, std::function<void(int)> onSucceed);

        virtual void keyDown(cocos2d::enumKeyCodes key);
        virtual bool setup();
};