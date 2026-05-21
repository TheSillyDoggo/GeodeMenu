#pragma once

#include <Geode/Geode.hpp>

namespace qolmod
{
    class LayerColor : public cocos2d::CCLayerColor
    {
        public:
            static LayerColor* create();
            static LayerColor * create(const cocos2d::ccColor4B& color, GLfloat width, GLfloat height);
            static LayerColor * create(const cocos2d::ccColor4B& color);

            virtual void draw();
    };
};