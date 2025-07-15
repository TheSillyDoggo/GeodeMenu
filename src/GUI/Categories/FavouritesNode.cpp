#include "FavouritesNode.hpp"

bool FavouritesNode::init()
{
    if (!CategoryNode::init())
        return false;

    text = CCLabelBMFont::create("No favourites help message", "bigFont.fnt");
    text->setScale(0.45f);

    for (auto mod : Module::getAllFavourited())
    {
        this->addModule(mod);
    }

    this->addChildAtPosition(text, Anchor::Center);
    return true;
}

void FavouritesNode::addModule(Module* module)
{
    CategoryNode::addModule(module);

    text->setVisible(modules.size() == 0);
}