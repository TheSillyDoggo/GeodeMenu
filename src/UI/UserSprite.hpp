#pragma once

#include <Geode/Geode.hpp>

namespace qolmod
{
    class UserSprite : public cocos2d::CCNodeRGBA
    {
        protected:
            std::string str;
            
            virtual bool init();
            ~UserSprite();

        public:
            CREATE_FUNC(UserSprite);
            static UserSprite* create(std::string str);
            static void refreshAll();

            virtual void setOpacity(GLubyte opacity);
            virtual void setColor(const cocos2d::ccColor3B& color);

            void updateSprite(std::string str);
            void refresh();
    };
};