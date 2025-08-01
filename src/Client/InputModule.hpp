#pragma once

#include "Module.hpp"

class InputModule : public Module
{
    protected:
        std::string text = "";
        std::string defaultText = "";
        std::string placeholder = "";
        std::string filter = "";
        std::string displayFilter = "%s";
        int charLimit = 0;
        float textFloat = 1.0f;

        void save();
        void load();

        void setDefaultString(std::string str);
        void setPlaceholderString(std::string str);
        void setStringFilter(std::string str);
        void setMaxCharCount(int count);
        void setDisplayFilter(std::string str);

    public:
        void setString(std::string str);
        std::string getString();
        float getStringFloat();
        int getStringInt();

        std::string getDefaultString();
        std::string getPlaceholderString();

        int getMaxCharCount();
        std::string getStringFilter();
        std::string getDisplayFilter();

        bool appendColon();

        virtual ModuleNode* getNode();
};