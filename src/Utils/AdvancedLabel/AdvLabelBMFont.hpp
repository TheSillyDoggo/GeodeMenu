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

    struct AdvPart
    {
        AdvPartType type;
        std::string label;
    };
};

class AdvLabelBMFont : public CCNode
{
    protected:
        AdvLabelStruct str;
        std::string font;
    
    public:
        static AdvLabelBMFont* createWithStruct(AdvLabelStruct lblStruct, std::string font);
        static AdvLabelBMFont* createWithString(std::string label, std::string font);

        static AdvLabelStruct structFromString(std::string lbl);
        
        void setStruct(AdvLabelStruct str);
        void updateLabel();
        bool initWithStruct(AdvLabelStruct lblStruct, std::string font);
};