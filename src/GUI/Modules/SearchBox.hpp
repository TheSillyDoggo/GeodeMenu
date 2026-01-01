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
            setCategory("Config/Menu");
            setDescription("Shows a search box icon");
            setDefaultEnabled(true);
        }

        virtual void onToggle()
        {
            if (AndroidUI::get())
                AndroidUI::get()->updateTabs();
        }
};

class SearchOnKeyPress : public Module
{
    public:
        MODULE_SETUP(SearchOnKeyPress)
        {
            setID("search-on-keypress");
            setCategory("Config/Menu");
            setDefaultEnabled(true);
        }
};


SUBMIT_HACK(SearchBox);
SUBMIT_HACK(SearchOnKeyPress);