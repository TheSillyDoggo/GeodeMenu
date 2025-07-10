#pragma once

#include <Geode/Geode.hpp>

class Module
{
    private:
        std::string name = "";
        std::string id = "";

        bool userEnabled = false;
        bool forceDisabled = false;

    public:
        void setUserEnabled(bool enabled);
        bool getUserEnabled();

        void setForceDisabled(bool forced);
        bool getForceDisabled();

        bool getRealEnabled();
};