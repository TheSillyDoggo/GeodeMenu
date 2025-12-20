#pragma once

#include <numbers>
#include <Geode/Geode.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include <Geode/modify/CCLayerColor.hpp>
#include <Geode/modify/CCNode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class CCBlurLayer : public CCLayerColor
{
    public:
        static inline std::vector<CCBlurLayer*> instances = {};
        bool visiting = false;
        CCRenderTexture* rtex;

        ~CCBlurLayer();
        bool init();
        void draw();

        void doSetup();

        GLuint compileShader(GLenum type, const char* source);
        GLuint loadShaderProgram(std::filesystem::path vertShaderPath, std::filesystem::path fragShaderPath);

        static CCBlurLayer* create();
};