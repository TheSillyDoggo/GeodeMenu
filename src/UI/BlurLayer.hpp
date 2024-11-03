#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/GameManager.hpp>

using namespace geode::prelude;

class BlurLayer : public CCLayerColor
{
    private:
        float blurSize = 8;
        int blurPasses = 1;

        bool drawOnce = false;
        bool hasDrawn = false;

        GLint sizeLocation;
        GLint passesLocation;

        Ref<CCRenderTexture> rtex;
        CCGLProgram* program;
        Ref<CCNode> node;

        static inline std::vector<BlurLayer*> instances;
    
    public:
        virtual bool init();
        virtual void visit();
        virtual void draw();

        void setBlurSize(float size);
        float getBlurSize();

        void setBlurPasses(int passes);
        int getBlurPasses();

        void setDrawOnce(bool draw);
        bool getDrawOnce();

        CREATE_FUNC(BlurLayer);
        ~BlurLayer();
};