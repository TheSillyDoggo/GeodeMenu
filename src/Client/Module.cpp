#include "Module.hpp"

void Module::setUserEnabled(bool enabled)
{
    this->userEnabled = enabled;
}

bool Module::getUserEnabled()
{
    return userEnabled;
}

void Module::setForceDisabled(bool forced)
{
    forceDisabled = forced;
}

bool Module::getForceDisabled()
{
    return forceDisabled;
}

bool Module::getRealEnabled()
{
    if (forceDisabled)
        return false;

    return userEnabled;
}