#pragma once

#include <Geode/Geode.hpp>

#if GEODE_COMP_GD_VERSION < 22081
namespace cocos2d
{
    enum class BorderAlignment
    {
        Outside = 0,
        Center = 1,
        Inside = 2
    };
};
#endif

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
            bool cullingEnabled = false;
            bool outerCull = false;
            cocos2d::CCRect cullArea = cocos2d::CCRectZero;

            BaseDrawNode();
            ~BaseDrawNode();

            void ensureCapacity(unsigned int count);
            void updateDebug();
            
            virtual bool init();
            virtual void draw();
            virtual void transform();
            virtual void visit();

            void enableCull(cocos2d::CCRect area, bool outer = false);
            void disableCull();

            void drawSegment(const cocos2d::CCPoint& startPoint, const cocos2d::CCPoint& endPoint, float radius, const cocos2d::ccColor4F& color);
            bool drawPolygon(cocos2d::CCPoint *verts, unsigned int count, const cocos2d::ccColor4F &fillColor, float borderWidth, const cocos2d::ccColor4F &borderColor, cocos2d::BorderAlignment alignment = cocos2d::BorderAlignment::Outside);

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