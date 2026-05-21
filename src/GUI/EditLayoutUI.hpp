#pragma once

#include <Geode/Geode.hpp>
#include <PopupBase.hpp>
#include "GrabNodeLayer.hpp"

class LayoutModule;

namespace qolmod
{
    class EditLayoutUI : public PopupBase
    {
        protected:
            LayoutModule* module = nullptr;
            geode::Ref<cocos2d::CCLayer> playerLayer = nullptr;
            cocos2d::CCDrawNode* previewOutline = nullptr;
            cocos2d::CCNode* previewNode = nullptr;
            GrabNodeLayer* grab = nullptr;

        public:
            static EditLayoutUI* create(LayoutModule* module);

            void updateOutline();

            virtual bool setup();
            virtual void update(float dt);
    };
};