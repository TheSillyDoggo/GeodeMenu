#include "OffsetManager.hpp"

OffsetManager* OffsetManager::get()
{
    static OffsetManager* instance = nullptr;

    if (!instance)
        instance = new OffsetManager();

    return instance;
}

unsigned int OffsetManager::offsetForType(PatchType type)
{
    switch (type)
    {
        case PatchType::PhysicsBypass:
            #if GEODE_COMP_GD_VERSION == 22074

            GEODE_WINDOWS(return 0x607008);
            GEODE_ANDROID32(return  - 0x10000);
            GEODE_ANDROID64(return  - 0x100000);
            GEODE_INTEL_MAC(return );
            GEODE_IOS(return );
    
            #endif

            #if GEODE_COMP_GD_VERSION == 22073

            GEODE_WINDOWS(return 0x606f98);
    
            #endif

            #if GEODE_COMP_GD_VERSION == 22060

            GEODE_WINDOWS(return 0x5ec6d0);
            GEODE_ANDROID32(return 0x46ce38 - 0x10000);
            GEODE_ANDROID64(return 0x9384b8 - 0x100000);
            GEODE_INTEL_MAC(return 0x823b00);
            GEODE_IOS(return 0x642b60);
    
            #endif

            break;
    }

    return 0x80085;
}

unsigned int OffsetManager::offsetForRandomSeed()
{
    #ifndef GEODE_IS_WINDOWS
    return 0x80085;
    #endif

    #if GEODE_COMP_GD_VERSION == 22074
    return 0x6a4e20;
    #endif

    #if GEODE_COMP_GD_VERSION == 22073
    return 0x6a4e20;
    #endif

    #if GEODE_COMP_GD_VERSION == 22060
    return 0x687dd0;
    #endif
}