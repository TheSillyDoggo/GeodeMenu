#pragma once

#include <Geode/Geode.hpp>

namespace qolmod
{
    class LabelBMFont : public cocos2d::CCSpriteBatchNode, public cocos2d::CCRGBAProtocol, public cocos2d::CCLabelProtocol
    {
        protected:
            std::vector<cocos2d::CCSprite*> fontSprites = {};
            std::wstring string;
            std::string utf8String = "";
            std::string fntFile = "";
            cocos2d::ccColor3B whiteColour = cocos2d::ccc3(255, 255, 255);
            GLubyte opacity = 255;
            cocos2d::CCBMFontConfiguration* config = nullptr;
            bool useGoldLetters = true;
            struct LabelSettings
            {
                bool useColourTags = true;
                bool removeColourTags = true;
            };
            LabelSettings settings = {};

            ~LabelBMFont();

            bool init(const char* str, const char* fnt);

            cocos2d::ccColor3B handleTag(std::wstring tag);
            cocos2d::CCSprite* getCharSprite(unsigned int index);
            void ensureCount(unsigned int count);

        public:
            static LabelBMFont* create(const char* str, const char* fnt);

            void updateLabel();

            virtual void setString(const char *label);
            virtual const char* getString(void);

            void setFntFile(const char* fntFile);
            const char* getFntFile();

            virtual void setColor(const cocos2d::ccColor3B& color) { this->whiteColour = color; updateLabel(); }
            virtual const cocos2d::ccColor3B& getColor(void) { return whiteColour; }
            virtual const cocos2d::ccColor3B& getDisplayedColor(void) { return whiteColour; }
            virtual GLubyte getDisplayedOpacity(void) { return opacity; }
            virtual GLubyte getOpacity(void) { return opacity; }
            virtual void setOpacity(GLubyte opacity) { this->opacity = opacity; updateLabel(); }
            virtual void setOpacityModifyRGB(bool bValue) { }
            virtual bool isOpacityModifyRGB(void) { return false; }
            virtual bool isCascadeColorEnabled(void) { return false; }
            virtual void setCascadeColorEnabled(bool cascadeColorEnabled) { }
            virtual void updateDisplayedColor(const cocos2d::ccColor3B& color) { }
            virtual bool isCascadeOpacityEnabled(void) { return false; }
            virtual void setCascadeOpacityEnabled(bool cascadeOpacityEnabled) { }
            virtual void updateDisplayedOpacity(GLubyte opacity) { }
    };
};