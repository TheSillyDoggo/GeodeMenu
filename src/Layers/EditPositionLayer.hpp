#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/TextAlertPopup.hpp>
#include "../Client/Module.h"

#include "SillyBaseLayer.h"

using namespace geode::prelude;

enum EditPositionType
{
    StartposSwitcher
};

class EditPositionLayer : public SillyBaseLayer
{
    public:
        CCLayerColor* bg;
        EditPositionType type;
        CCMenu* node;
        CCSprite* left;
        CCSprite* right;
        CCSprite* previewBG;
        CCLabelBMFont* label;
        CCMenu* nodeOuter;
        Slider* scaleSlider;
        Slider* opacitySlider;
        CCPoint position;
        float scale;
        float opacity;
        CCPoint offset;
        bool isDragging;

        virtual void customSetup();
        virtual void update(float dt);

        virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
        virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
        virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
        virtual void ccTouchCancelled(CCTouch* touch, CCEvent* event);

        void onClose(CCObject*);
        void onReset(CCObject*);
        void onTogglePreview(CCObject*);
        void sliderValueChanged(CCObject* sender);

        CCMenu* getNodeForType();

        float scaleValue(float originalValue);
        float unscaleValue(float scaledValue);

        static EditPositionLayer* create(EditPositionType type)
        {
            EditPositionLayer* pRet = new EditPositionLayer();
            
            pRet->type = type;

            if (pRet && pRet->initWithSizeAndName(ccp(0, 0), "")) {
                pRet->autorelease();
                return pRet;
            } else {
                delete pRet;
                return nullptr;
            }
        }

        static EditPositionLayer* addToScene(EditPositionType type)
        {
            auto pRet = EditPositionLayer::create(type);

            CCScene::get()->addChild(pRet, 99999);

            return pRet;
        }
};