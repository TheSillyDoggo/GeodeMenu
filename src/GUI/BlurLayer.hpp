#pragma once

#include <numbers>
#include <Geode/Geode.hpp>
#include <Geode/modify/CCEGLView.hpp>
#include <Geode/modify/CCDirector.hpp>

using namespace geode::prelude;

class CCBlurLayer : public CCLayerColor
{
    protected:
        CCGLProgram* program;
        CCGLProgram* program2;
        CCRenderTexture* render;
        CCRenderTexture* render2;
        CCSprite* sprite;
        float blurStrength = 1;

        GLint uniformFirst;
        GLint uniformFast;
        GLint uniformRadius;

        void setFirst(bool first);
        void setRadius(float radius);
        CCGLProgram* createProgram(bool horizontal);

    public:
        ~CCBlurLayer();
        bool init();
        void visit();

        static CCBlurLayer* create();
};