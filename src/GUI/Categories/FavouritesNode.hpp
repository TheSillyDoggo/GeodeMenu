#pragma once

#include "../../Client/CategoryNode.hpp"

class FavouritesNode : public CategoryNode
{
    protected:
        CCLabelBMFont* text = nullptr;
        CCSprite* star = nullptr;
        CCSprite* info = nullptr;
        static inline FavouritesNode* instance = nullptr;

        ~FavouritesNode();

    public:
        CREATE_FUNC(FavouritesNode)
        static FavouritesNode* get();

        void refresh();
        virtual void updateUI();
        bool init();
};

SUBMIT_CATEGORY("Favourites", FavouritesNode)