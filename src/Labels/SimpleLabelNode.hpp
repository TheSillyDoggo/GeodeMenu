#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

enum class SimpleLabelAnchor
{
    TopLeft,
    TopMiddle,
    TopRight,
    BottomLeft,
    BottomMiddle,
    BottomRight,
    MiddleLeft,
    Middle,
    MiddleRight,
};

class SimpleLabelNode : public CCLayer
{
    protected:
        std::unordered_map<SimpleLabelAnchor, CCLabelBMFont*> labels = {};
        std::unordered_map<SimpleLabelAnchor, std::string> labelStrs = {};

    public:
        CREATE_FUNC(SimpleLabelNode);

        void setupLabel(SimpleLabelAnchor anchor);
        void addString(SimpleLabelAnchor anchor, std::string str);

        CCPoint getPointForAnchor(SimpleLabelAnchor anchor);

        virtual bool init();
        virtual void update(float dt);
        virtual void setContentSize(const CCSize& var);
};