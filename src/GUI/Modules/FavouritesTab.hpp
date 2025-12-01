#pragma once

#include "../../Client/Module.hpp"

using namespace geode::prelude;

class FavouritesTab : public Module
{
    public:
        MODULE_SETUP(FavouritesTab)
        {
            setName("Favourites Tab");
            setID("show-favourites");
            setCategory("Config/Menu");
            setDescription("Shows a favourites tab");
            setDefaultEnabled(true);
        }

        virtual void onToggle()
        {
            if (AndroidUI::get())
                AndroidUI::get()->updateTabs();
        }
};

SUBMIT_HACK(FavouritesTab);