#pragma once

#include <Geode/Geode.hpp>

namespace qolmod
{
    class CCNodeWithShader : public cocos2d::CCNodeRGBA
    {
        protected:
            geode::Ref<cocos2d::CCRenderTexture> rt = nullptr;
            cocos2d::CCSize offset = cocos2d::CCSizeMake(250, 250);
            bool listened = false;

            ~CCNodeWithShader();

        public:
            CREATE_FUNC(CCNodeWithShader);

            void listenBackToForeground(CCObject* sender);

            virtual bool init();
            virtual void visit();
    };
};