#pragma once

#include <numbers>
#include <Geode/Geode.hpp>
#include <Geode/modify/CCEGLViewProtocol.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include <Geode/modify/CCLayerColor.hpp>
#include <Geode/modify/CCNode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class CCBlurLayer : public CCLayerColor
{
    public:
        bool visiting = false;
        float blurStrength = 1;

        ~CCBlurLayer();
        bool init();
        void visit();
        void draw();

        GLuint compileShader(GLenum type, const char* source);
        GLuint loadShaderProgram(ghc::filesystem::path vertShaderPath, ghc::filesystem::path fragShaderPath);

        static CCBlurLayer* create();
};