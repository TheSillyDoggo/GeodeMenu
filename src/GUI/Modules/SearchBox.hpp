#pragma once

#include "../../Client/Module.hpp"

using namespace geode::prelude;

class SearchBox : public Module
{
    public:
        MODULE_SETUP(SearchBox)
        {
            setName("Search Box");
            setID("show-searchbox");
            setCategory("Config");
            setDescription("Shows a search box icon");
            setDefaultEnabled(true);
        }

        virtual void onToggle()
        {
            if (AndroidUI::get())
                AndroidUI::get()->updateTabs();
        }
};


SUBMIT_HACK(SearchBox);