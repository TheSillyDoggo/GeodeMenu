#pragma once

#include "Module.hpp"
#include "EnumModuleNode.hpp"
#include <enchantum/enchantum.hpp>

class EnumModule : public Module
{
    protected:
        int value;
        int defaultValue;
        std::map<int, std::string> listedValues = {};

        virtual void save();
        virtual void load();

        void setDefaultValue(int value)
        {
            this->defaultValue = value;
        }

    public:
        void setValue(int value);
        int getValue();

        int getDefaultValue();

        template <class T>
        void initValues()
        {
            listedValues.clear();

            for(const auto [value,name] : enchantum::entries_generator<T>)
            {
                listedValues.emplace((int)value, name);
            }
        }

        std::map<int, std::string> getDisplayNames();

        void setNext();
        void setPrev();

        virtual ModuleNode* getNode();
};