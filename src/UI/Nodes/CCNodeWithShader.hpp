#pragma once

#include <Geode/Geode.hpp>

class CCNodeWithShader : public cocos2d::CCNodeRGBA
{
    protected:
        geode::Ref<cocos2d::CCRenderTexture> rt = nullptr;

    public:
        CREATE_FUNC(CCNodeWithShader);

        virtual bool init();
        virtual void visit();
};