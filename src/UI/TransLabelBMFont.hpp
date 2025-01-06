#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#define ENGLISH_REGEX "^[a-zA-Z0-9\\s%\"!:,./#\\-<>'()]*$"

// they call this my gender
class TransLabelBMFont : public CCNode
{
    private:
        CCLabelBMFont* label;
        CCLabelTTF* ttf;
        bool useTtf = false;
        bool forceTtf = false;
        std::string text;
        std::string originalText;
        std::string font;

        std::function<void()> onLabelUpdated;

        bool isLimited = false;
        float limitWidth = 0.0f;
        float limitDefaultScale = 0.0f;
        float limitMinScale = 0.0f;

        static inline std::vector<TransLabelBMFont*> instances = {};
    
        CCLabelTTF* createTTF(std::string font, float fontSize);
        void updateTTFVisible();

        ~TransLabelBMFont();

    public:
        bool init(std::string text, std::string font);

        void limitLabelWidth(float width, float defaultScale, float minScale);

        void setOpacity(int opacity);
        void setColor(ccColor3B colour);

        void setString(std::string str);
        std::string getString();

        void setForceTTF(bool force);
        bool getForceTTF();

        void setOnLabelUpdated(std::function<void()> callback);

        static void updateAllLabels();
        static TransLabelBMFont* create(std::string text, std::string font);
};