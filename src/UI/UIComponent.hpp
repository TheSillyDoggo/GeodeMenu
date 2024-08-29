#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class UIComponent : public CCNode
{
    public:
        virtual void drawImGui();
        virtual void refreshNode();
        virtual void setParent(CCNode* parent);
};