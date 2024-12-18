#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

// they call this my gender
class TransLabelBMFont : public CCNode
{
    private:
        CCLabelBMFont* label;
        CCLabelTTF* ttf;
        bool useTtf = false;
        std::string text;
        std::string originalText;
        std::string font;

    public:
        bool isLimited = false;
    private:
        float limitWidth = 0.0f;
        float limitDefaultScale = 0.0f;
        float limitMinScale = 0.0f;
    
        CCLabelTTF* createTTF(std::string font, float fontSize);

        static inline std::vector<TransLabelBMFont*> instances = {};

    public:
        bool init(std::string text, std::string font);
        ~TransLabelBMFont();

        void limitLabelWidth(float width, float defaultScale, float minScale);

        void setOpacity(int opacity);
        void setColor(ccColor3B colour);

        void setString(const char* str);
        std::string getString();

        void updateTTFVisible();

        static void updateAllLabels();

        static TransLabelBMFont* create(std::string text, std::string font);
};