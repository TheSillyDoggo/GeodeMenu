#pragma once

#include "Module.hpp"

class FloatSliderModule : public Module
{
    friend class FloatSliderModuleNode;

    protected:
        float value = 1.0f;
        float defaultValue = 1.0f;
        float minRange = 0.0f;
        float maxRange = 1.0f;
        std::vector<float> snaps = {};

        virtual void save();
        virtual void load();

        void setRange(float min, float max);
        void setDefaultValue(float def);
        void setSnapValues(std::vector<float> snaps);

    public:
        void setValue(float value);
        float getValue();

        float getDefaultValue();

        virtual ModuleNode* getNode();
};