#pragma once

#include "Module.hpp"

class RotationModule : public Module
{
    protected:
        float rotation = 0;
        float defaultValue = 0;
    
        virtual void save();
        virtual void load();

        void setDefaultValue(float v);

    public:
        void setRotation(float value);
        float getRotationDegrees();
        cocos2d::CCPoint getRotationVector();

        float getDefaultValue();

        virtual ModuleNode* getNode();
};