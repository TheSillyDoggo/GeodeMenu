#pragma once

#include <Geode/Geode.hpp>

namespace qolmod
{
    class VisitHook : public cocos2d::CCGridBase
    {
        using VisitCallbackFunc = std::function<void(cocos2d::CCNode* node)>;

        protected:
            VisitCallbackFunc callbackPre = nullptr;
            VisitCallbackFunc callbackPost = nullptr;

            virtual bool init();

        public:
            CREATE_FUNC(VisitHook);

            void setCallbackPre(const VisitCallbackFunc& func);
            void setCallbackPost(const VisitCallbackFunc& func);
        
            virtual void preVisit();
            virtual void postVisit(cocos2d::CCNode* node);
    };
};