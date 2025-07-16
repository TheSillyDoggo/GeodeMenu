#include "FavouritesNode.hpp"

FavouritesNode::~FavouritesNode()
{
    instance = nullptr;
}

FavouritesNode* FavouritesNode::get()
{
    return instance;
}

bool FavouritesNode::init()
{
    if (!CategoryNode::init())
        return false;

    instance = this;

    text = CCLabelBMFont::create("No favourites added yet </3\nTap the info (  ) button next to a mod\nthen press the star (  ) button to favourite it!", "bigFont.fnt");
    text->setAlignment(kCCTextAlignmentCenter);
    text->setScale(0.375f);

    star = CCSprite::create("favourites.png"_spr);
    star->setScale(0.625f);

    info = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    info->setScale(0.425f);

    for (auto mod : Module::getAllFavourited())
    {
        this->addModule(mod);
    }

    this->addChildAtPosition(text, Anchor::Center);
    this->addChildAtPosition(star, Anchor::Center, ccp(-10, -12.5f));
    this->addChildAtPosition(info, Anchor::Center, ccp(-30.2f, -0.5f));
    return true;
}

void FavouritesNode::refresh()
{
    auto mods = Module::getAllFavourited();

    for (auto mod : mods)
    {
        if (!modules.contains(mod))
        {
            addModule(mod);
        }
    }

    for (auto mod : modules)
    {
        if (std::find(mods.begin(), mods.end(), mod.first) == mods.end())
        {
            removeModule(mod.first);
        }
    }
}

void FavouritesNode::updateUI()
{
    CategoryNode::updateUI();

    text->setVisible(modules.size() == 0);
    star->setVisible(modules.size() == 0);
    info->setVisible(modules.size() == 0);
}