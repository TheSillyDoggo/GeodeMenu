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
            cocos2d::CCLayerColor* selectDarken = nullptr;
            cocos2d::CCDrawNode* previewOutline = nullptr;
            cocos2d::CCDrawNode* anchorPointsNode = nullptr;
            cocos2d::CCNode* previewNode = nullptr;
            cocos2d::CCNode* previewNodeParent = nullptr;
            GrabNodeLayer* grab = nullptr;
            bool lastFrameSelected = false;
            std::vector<std::pair<float, bool>> anchorPoints = {};
            float anchorSnapDis = 10;
            float anchorShowDis = 60;
            bool snapEnabled = true;

            CCNode* createBackground(bool acu = true);

            void updateOutline();
            void updateAnchorPoints();
            void fixToAnchorPoints();
            void fixToScreenBounds();

        public:
            static EditLayoutUI* create(LayoutModule* module);

            void addAnchorPoint(float position, bool horizontal);

            virtual bool setup();
            virtual void update(float dt);
    };
};