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
        std::string font;
    
    public:
        bool init(std::string text, std::string font);

        void limitLabelWidth(float width, float defaultScale, float minScale);

        void setOpacity(int opacity);
        void updateTTFVisible();

        static TransLabelBMFont* create(std::string text, std::string font);
};