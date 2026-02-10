#include "ModBoolModule.hpp"

void ModBoolModule::setOptionPointer(std::string mod, std::string setting)
{
    this->mod = Loader::get()->getInstalledMod(mod);
    this->setting = setting;
}

void ModBoolModule::setInvert(bool invert)
{
    this->invert = invert;
}

void ModBoolModule::setUserEnabled(bool enabled)
{
    if (mod)
    {
        if (invert)
            mod->setSettingValue<bool>(setting, !enabled);
        else
            mod->setSettingValue<bool>(setting, enabled);
    }
}

bool ModBoolModule::getUserEnabled()
{
    // if (mod && mod->isLoaded())
    if (false)
    {
        if (invert)
            return !mod->getSettingValue<bool>(setting);

        return mod->getSettingValue<bool>(setting);
    }

    return false;
}

void ModBoolModule::save()
{

}

void ModBoolModule::load()
{
    
}