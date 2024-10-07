#include "LaunchArgs.hpp"

LaunchArgs* LaunchArgs::get()
{
    if (!instance)
        instance = new LaunchArgs();

    return instance;
}

bool LaunchArgs::hasLaunchArg(std::string arg)
{
    #ifdef QOLMOD_SUPPORTS_LAUNCH_ARGS

    std::string launchStr = GetCommandLineA();

    auto args = string::split(launchStr, " ");

    for (auto larg : args)
    {
        if (larg == arg)
            return true;
    }

    #endif

    return false;
}