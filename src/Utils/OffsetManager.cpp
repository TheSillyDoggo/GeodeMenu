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
            // found in GJBaseGameLayer::getModifiedDelta

            #if GEODE_COMP_GD_VERSION == 22074

            GEODE_WINDOWS(return 0x607008);
            GEODE_ANDROID32(return 0x4740f0 - 0x10000);
            GEODE_ANDROID64(return 0x9473c0 - 0x100000);
            GEODE_INTEL_MAC(return 0x7ff4d8);
            GEODE_IOS(return 0x631788);

            #elif GEODE_COMP_GD_VERSION == 22073

            GEODE_WINDOWS(return 0x606f98);

            #elif GEODE_COMP_GD_VERSION == 22060

            GEODE_WINDOWS(return 0x5ec6d0);
            GEODE_ANDROID32(return 0x46ce38 - 0x10000);
            GEODE_ANDROID64(return 0x9384b8 - 0x100000);
            GEODE_INTEL_MAC(return 0x823b00);
            GEODE_ARM_MAC(return );
            GEODE_IOS(return 0x642b60);

            #else

            static_assert(false, "Patches not updated for this game version");

            #endif
            
            break;

        case PatchType::EditorExtension1:
            // found in CCPoint * EditorUI::getLimitedPosition(EditorUI *this,CCPoi nt *__return,CCPoint param_3)

            #if GEODE_COMP_GD_VERSION == 22074

            GEODE_WINDOWS(return 0x607ca0);
            GEODE_ANDROID32(return 0x382bd8 - 0x10000);
            GEODE_ANDROID64(return 0x7773c4 - 0x100000);
            GEODE_INTEL_MAC(return 0x7fe110);
            GEODE_IOS(return );
    
            #elif GEODE_COMP_GD_VERSION == 22073

            GEODE_WINDOWS(return 0x607c30);
    
            #elif GEODE_COMP_GD_VERSION == 22060

            GEODE_WINDOWS(return 0x5ed33c);
            GEODE_ANDROID32(return 0x37e678 - 0x10000);
            GEODE_ANDROID64(return 0x76cdb4 - 0x100000);
            GEODE_INTEL_MAC(return 0x822770);
            GEODE_IOS(return );
    
            #else

            static_assert(false, "Patches not updated for this game version");

            #endif

            break;

        case PatchType::EditorExtension2:
            // found in void EditorUI::constrainGameLayerPosition(EditorUI *this,fl oat param_2,float param_3)

            #if GEODE_COMP_GD_VERSION == 22074

            GEODE_WINDOWS(return 0x607ca4);
            GEODE_ANDROID32(return 0x382f7c - 0x10000);
            GEODE_ANDROID64(return 0x77790c - 0x100000);
            GEODE_INTEL_MAC(return 0x7fdf34);
            GEODE_IOS(return );

            #elif GEODE_COMP_GD_VERSION == 22073

            GEODE_WINDOWS(return 0x607c34);

            #elif GEODE_COMP_GD_VERSION == 22060

            GEODE_WINDOWS(return 0x5ed340);
            GEODE_ANDROID32(return 0x37ea1c - 0x10000);
            GEODE_ANDROID64(return 0x76d2fc - 0x100000);
            GEODE_INTEL_MAC(return 0x82258c);
            GEODE_IOS(return );
    
            #else

            static_assert(false, "Patches not updated for this game version");

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