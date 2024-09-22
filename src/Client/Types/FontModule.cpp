#include "FontModule.hpp"
#include "../../Layers/ChooseFontPopup.hpp"

FontModule::FontModule(std::string id)
{
    this->id = id;
    font = "bigFont.fnt";

    load();
}

void FontModule::save()
{
    Mod::get()->setSavedValue(fmt::format("{}_font", id), font);
}

void FontModule::load()
{
    font = Mod::get()->getSavedValue(fmt::format("{}_font", id), font);

    save();
}

void FontModule::makeAndroid(CCNode* menu, CCPoint pos)
{
    auto btn = ButtonSprite::create("Change Font", 100, 0, 1.0f, false, getSelectedFont().c_str(), "GJ_button_05.png", 23);

    auto btn2 = CCMenuItemSpriteExtra::create(btn, this, menu_selector(FontModule::onChooseFont));
    btn2->setPosition(pos + ccp(135 / 2, 0));

    menu->addChild(btn2);
}

void FontModule::onChooseFont(CCObject* sender)
{
    ChooseFontPopup::addToScene([this](std::string font)
    {
        log::info("selected font: {}", font);

        this->font = font;
        save();

        if (onToggle)
            onToggle(true);
        
    })->setSelected(getSelectedFont());
}

std::string FontModule::getSelectedFont()
{
    // if the font does not exist, return the default which probably already exists. if it doesn't exist gd would've crashed earlier
    if (!CCFileUtils::sharedFileUtils()->isFileExist(CCFileUtils::sharedFileUtils()->fullPathForFilename(font.c_str(), false)))
        return "bigFont.fnt";
    
    return font;
}