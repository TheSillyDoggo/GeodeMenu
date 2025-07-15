#pragma once

#include "../../Client/CategoryNode.hpp"

class FavouritesNode : public CategoryNode
{
    protected:
        CCLabelBMFont* text = nullptr;

    public:
        CREATE_FUNC(FavouritesNode)

        void addModule(Module* module);
        bool init();
};

SUBMIT_CATEGORY("Favourites", FavouritesNode)