#pragma once

#include <Geode/Geode.hpp>

namespace qolmod
{
    class BaseDrawNode : public cocos2d::CCDrawNode
    {
        protected:
            cocos2d::CCLabelBMFont* debugLabel = nullptr;
            bool debugEnabled = false;
            bool worldSpace = true;
            cocos2d::CCNode* world1 = nullptr;
            cocos2d::CCNode* world2 = nullptr;
            GJBaseGameLayer* gjbgl = nullptr;

            BaseDrawNode();
            ~BaseDrawNode();

            void ensureCapacity(unsigned int count);
            void updateDebug();
            virtual bool init();
            virtual void draw();
            virtual void transform();
            virtual void visit();

            void drawSegment(const cocos2d::CCPoint& startPoint, const cocos2d::CCPoint& endPoint, float radius, const cocos2d::ccColor4F& color);

        public:
            /*
             * Called at the end of update, use only for stuff that needs to be updated
             * if there is a tick, but doesnt need tick precision
            */
            virtual void redraw();
            virtual void onTickStart();
            virtual void onTickEnd();

            void setDebugEnabled(bool enabled);
            bool isDebugEnabled();

            void setWorldSpace(bool world);
            bool useWorldSpace();
    };
};