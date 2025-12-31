#pragma once

#include <Geode/Geode.hpp>
#include "BackgroundSprite.hpp"
#include "../Client/CategoryNode.hpp"
#include "BetterInputNode.hpp"

using namespace geode::prelude;

class SetupRotationUI : public geode::Popup<>, public TextInputDelegate, public GJRotationControlDelegate
{
    protected:
        BackgroundSprite* bg;
        float rot = 0;
        float startRot = 0;
        float defaultRot = 0;
        std::function<void(float)> onFinish = nullptr;
        BetterInputNode* input = nullptr;
        GJRotationControl* control = nullptr;

        virtual void textChanged(CCTextInputNode* node);
        virtual void angleChanged(float angle);

    public:
        static SetupRotationUI* create(std::function<void(float)> onFinish);

        void setDefaultRot(float value);
        void setStartRot(float value);

        void onClose(CCObject* sender);

        void updateUI();

        virtual bool setup();
};