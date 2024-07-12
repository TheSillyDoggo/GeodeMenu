#include "DropdownModule.hpp"

void DropdownModule::makeAndroid(CCNode* menu, CCPoint pos)
{
    auto dd = Dropdown::create(ccp(140, 30), content, menu_selector(DropdownModule::onDropdownSelectionChanged), index);
    dd->setUserData(this);
    dd->setPosition(pos + ccp(0, -15));
    menu->addChild(dd);
}

void DropdownModule::onDropdownSelectionChanged(CCObject* sender)
{
    auto mod = as<DropdownModule*>(as<CCNode*>(sender)->getParent()->getUserData());
    auto drop = as<Dropdown*>(as<CCNode*>(sender)->getParent());

    mod->index = drop->getSelectedIndex();
    mod->save();

    if (mod->delegate)
        mod->delegate->onModuleChanged(mod->enabled);
}

DropdownModule::DropdownModule(std::vector<std::string> stuff, std::string id, int def)
{
    this->name = name;
    this->id = id;
    index = def;
    this->content = stuff;

    this->load();
}

void DropdownModule::save()
{
    Mod::get()->setSavedValue<int>(id + "_index", index);
}

void DropdownModule::load()
{
    index = Mod::get()->getSavedValue<int>(id + "_index", index);
}