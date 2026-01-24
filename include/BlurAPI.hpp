#pragma once

#include <Geode/Geode.hpp>

#define BLUR_TAG "thesillydoggo.blur_api/blur-options"

namespace BlurAPI
{
    class BlurOptions : public cocos2d::CCObject
    {
        public:
            int apiVersion = 1; // dont change
            cocos2d::CCRenderTexture* rTex = nullptr;
            geode::Ref<cocos2d::CCClippingNode> clip = nullptr;
            bool forcePasses = false;
            int passes = 3;
            float alphaThreshold = 0.01f;

            virtual bool init() { return true; }
            CREATE_FUNC(BlurOptions);
    };

    inline BlurOptions* getOptions(cocos2d::CCNode* node)
    {
        return static_cast<BlurOptions*>(node->getUserObject(BLUR_TAG));
    }

    inline void addBlur(cocos2d::CCNode* node)
    {
        if (getOptions(node))
            return;

        node->setUserObject(BLUR_TAG, BlurOptions::create());
    }

    inline void removeBlur(cocos2d::CCNode* node)
    {
        node->setUserObject(BLUR_TAG, nullptr);
    }

    inline bool isBlurAPIEnabled()
    {
        if (auto blur = geode::Loader::get()->getLoadedMod("thesillydoggo.blur_api"))
        {
            if (blur->getSettingValue<bool>("enabled"))
                return true;
        }

        return false;
    }
};