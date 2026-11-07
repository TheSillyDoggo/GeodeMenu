#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

enum class AdvLabelTTFUsage
{
    None,
    Auto,
    Forced,
};

class AdvLabelBMFont : public CCNode, public CCRGBAProtocol, public CCLabelProtocol
{
    protected:
        geode::Label* regularLabel = nullptr;
        geode::Label* fallbackLabel = nullptr;

        bool forceFallback = false;
        bool forceRegular = false;

        bool wrapByLetters = false;

        float maxWidth = 0;
        float lineSpacing = 0;

        std::string str;
        std::string font;
        AdvLabelTTFUsage ttfUsage = AdvLabelTTFUsage::Auto;
        ccColor3B colour = ccWHITE;
        GLubyte opacity = 255;
        geode::Label::Alignment alignment = geode::Label::Alignment::Left;

        bool useTTFFont();
        bool initWithString(std::string str, std::string font);

        cocos2d::CCGLProgram* createGoldShader();
    
    public:
        static AdvLabelBMFont* createWithString(std::string label, std::string font);
        static AdvLabelBMFont* createWithLocalisation(std::string localisationKey, std::string font);

        void limitLabelWidth(float width, float defaultScale, float minScale);
        void updateLabel();

        void setTTFUsage(AdvLabelTTFUsage usage);
        AdvLabelTTFUsage getTTFUsage();

        virtual void setString(const char *label);
        virtual const char* getString(void);

        void setFntFile(const char* fntFile);
        const char* getFntFile();

        void setMaxWidth(float width);
        float getMaxWidth() const;

        void setLineSpacing(float spacing);
        float getLineSpacing() const;

        void setWrapByLetters(bool wrap);
        bool getWrapByLetters() const;

        virtual void setAlignment(CCTextAlignment alignment);

        virtual void setColor(const ccColor3B& color) { if (this->colour != color) { this->colour = color; updateLabel(); } }
        virtual const ccColor3B& getColor(void) { return colour; }
        virtual const ccColor3B& getDisplayedColor(void) { return colour; }
        virtual GLubyte getDisplayedOpacity(void) { return opacity; }
        virtual GLubyte getOpacity(void) { return opacity; }
        virtual void setOpacity(GLubyte opacity)
        {
            if (this->opacity != opacity)
            {
                this->opacity = opacity;
                updateLabel();
            }
        }
        virtual void setOpacityModifyRGB(bool bValue) { }
        virtual bool isOpacityModifyRGB(void) { return false; }
        virtual bool isCascadeColorEnabled(void) { return false; }
        virtual void setCascadeColorEnabled(bool cascadeColorEnabled) { }
        virtual void updateDisplayedColor(const ccColor3B& color) { }
        virtual bool isCascadeOpacityEnabled(void) { return false; }
        virtual void setCascadeOpacityEnabled(bool cascadeOpacityEnabled) { }
        virtual void updateDisplayedOpacity(GLubyte opacity) { }
};