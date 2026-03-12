#pragma once

#include <Geode/Geode.hpp>

namespace qolmod
{
    class PaintNode : public cocos2d::CCNode
    {
        protected:
            cocos2d::CCRenderTexture* rTex = nullptr;
            std::unordered_map<int, geode::Ref<cocos2d::CCDrawNode>> cursorPreviews = {};

            void drawPixel(int x, int y, cocos2d::ccColor4F col);

            virtual bool init();

        public:
            static PaintNode* get();

            void paintBrush(cocos2d::CCPoint position, cocos2d::ccColor3B colour, int size, bool erase = false);

            cocos2d::CCDrawNode* getPreview(int size);

            void paintPos(cocos2d::CCPoint position, cocos2d::ccColor3B colour);
            void erasePos(cocos2d::CCPoint position);

            virtual void visit();
    };
};