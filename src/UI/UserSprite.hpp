#pragma once

#include <Geode/Geode.hpp>

namespace qolmod
{
    class UserSprite : public cocos2d::CCNode
    {
        protected:
            std::string str;
            
            virtual bool init();
            ~UserSprite();

        public:
            CREATE_FUNC(UserSprite);
            static UserSprite* create(std::string str);
            static void refreshAll();

            void updateSprite(std::string str);
            void refresh();
    };
};