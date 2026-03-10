#pragma once

#include <Geode/Geode.hpp>
#include <GrabNodeLayer.hpp>

namespace qolmod
{
    class ZoomControl : public cocos2d::CCNode
    {
        protected:
            GrabNodeLayer* grab = nullptr;
            CCLabelBMFont* label = nullptr;
            CCMenuItemSpriteExtra* zoomIn = nullptr;
            CCMenuItemSpriteExtra* zoomOut = nullptr;
            CCMenu* menu = nullptr;

            ~ZoomControl();

        public:
            CREATE_FUNC(ZoomControl)

            void onZoomIn(CCObject* sender);
            void onZoomOut(CCObject* sender);

            virtual bool init();
            virtual void update(float dt);
    };
};