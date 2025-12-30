#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct AdvLabelStruct
{
    enum class AdvPartType
    {
        Label,
        NewLine,
    };

    bool forceTTF = false;
    bool forceBM = false;
    bool splitSpaces = false;

    struct AdvPart
    {
        AdvPartType type = AdvPartType::NewLine;
        std::string label;
        ccColor3B colour = ccWHITE;
        float opacity = 1.0f;
    };

    std::string getTotalString();

    std::vector<AdvPart> parts = {};
};

/*class CCLabelTTFCache
{
    protected:
        std::unordered_map<std::string, CCTexture2D*> stringsCached = {};
        CCLabelTTF* label;
        bool assetsPreloaded = false;

        void addTexture(std::string text);

    public:
        static CCLabelTTFCache* get();

        void flushTextures();
        void preloadTextures();

        CCTexture2D* getTexture(std::string text);
};*/

enum class AdvLabelTTFUsage
{
    None,
    Auto,
    Forced,
};

class AdvLabelBMFont : public CCNode, public CCRGBAProtocol, public CCLabelProtocol
{
    protected:
        static inline std::unordered_map<std::string, CCLabelBMFont*> bmConfigs = {};

        AdvLabelStruct str;
        std::string font;
        CCSprite* ttfDisplay;
        AdvLabelTTFUsage ttfUsage = AdvLabelTTFUsage::Auto;
        ccColor3B colour = ccWHITE;
        GLubyte opacity = 255;
        std::unordered_map<std::string, std::vector<CCLabelBMFont*>> labelsCached = {};
        std::vector<CCLabelBMFont*> visibleLabels = {};
        CCTextAlignment alignment = kCCTextAlignmentLeft;
        float maxWidth = 0;
        bool usingTTFCurrently = false;
        int lineCount = 0;
        bool splitEverySpace = false;
        float lineSpacing = 0;

        static AdvLabelStruct structFromString(std::string lbl, bool splitSpaces = false);

        bool useTTFFont();
    
    public:
        static AdvLabelBMFont* createWithStruct(AdvLabelStruct lblStruct, std::string font);
        static AdvLabelBMFont* createWithString(std::string label, std::string font);
        static AdvLabelBMFont* createWithLocalisation(std::string localisationKey, std::string font);

        void limitLabelWidth(float width, float defaultScale, float minScale);
        void setStruct(AdvLabelStruct str);
        void updateLabel();
        bool initWithStruct(AdvLabelStruct lblStruct, std::string font);

        void setTTFUsage(AdvLabelTTFUsage usage);
        AdvLabelTTFUsage getTTFUsage();

        bool isUsingTTFCurrently();

        virtual void setString(const char *label);
        virtual const char* getString(void);

        void setFntFile(const char* fntFile);
        const char* getFntFile();

        virtual void setAlignment(CCTextAlignment alignment);

        std::vector<CCLabelBMFont*> getVisibleLabels();

        void setMaxWidth(float width);
        void setSplitEverySpace(bool split);
        void setLineSpacing(float spacing);
        int getLineCount();

        CCBMFontConfiguration* getConfiguration();
        CCBMFontConfiguration* getConfiguration(std::string font);

        virtual void setColor(const ccColor3B& color) { this->colour = color; updateLabel(); }
        virtual const ccColor3B& getColor(void) { return colour; }
        virtual const ccColor3B& getDisplayedColor(void) { return colour; }
        virtual GLubyte getDisplayedOpacity(void) { return opacity; }
        virtual GLubyte getOpacity(void) { return opacity; }
        virtual void setOpacity(GLubyte opacity) { this->opacity = opacity; updateLabel(); }
        virtual void setOpacityModifyRGB(bool bValue) { }
        virtual bool isOpacityModifyRGB(void) { return false; }
        virtual bool isCascadeColorEnabled(void) { return false; }
        virtual void setCascadeColorEnabled(bool cascadeColorEnabled) { }
        virtual void updateDisplayedColor(const ccColor3B& color) { }
        virtual bool isCascadeOpacityEnabled(void) { return false; }
        virtual void setCascadeOpacityEnabled(bool cascadeOpacityEnabled) { }
        virtual void updateDisplayedOpacity(GLubyte opacity) { }
};