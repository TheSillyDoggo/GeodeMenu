#pragma once

#include <Geode/Geode.hpp>
#include <GrabNodeLayer.hpp>
#include <Button.hpp>

namespace qolmod
{
    class ZoomControl : public cocos2d::CCNode
    {
        protected:
            GrabNodeLayer* grab = nullptr;
            CCLabelBMFont* label = nullptr;
            qolmod::Button* zoomIn = nullptr;
            qolmod::Button* zoomOut = nullptr;
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