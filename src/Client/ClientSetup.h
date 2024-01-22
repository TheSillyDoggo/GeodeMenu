#include "Client.h"

class ClientUtils
{
public:
	static void Setup(bool android = false)
	{
        SetupLevel();
        SetupBypass();
        SetupSpeedhack();
        SetupCosmetic();
        //SetupMisc();
        //SetupReplay();
        //SetupStatus();
        SetupOptions(android);
        //SetupOptions();
        //SetupCreator();
        //SetupVariables();

        if (android)
            Client::instance->windows.push_back(new Credits());
	}

#pragma region Setup Windows

    static void SetupSpeedhack()
    {
        Window* speedhack = new Window();
        speedhack->name = "Speedhack";
        speedhack->id = "speedhack-window";
        speedhack->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 2, 50);

        speedhack->modules.push_back(new SpeedhackTop());
        speedhack->modules.push_back(new Module("Enabled", "speedhack-enabled", "Is the speedhack enabled?"));
        //speedhack->modules.push_back(new Module("Speedhack Music", "speedhack-music"));

        (reinterpret_cast<SpeedhackTop*>(speedhack->modules.front()))->format = "%x";

        Client::instance->windows.push_back(speedhack);
    }

    static void SetupLevel()
    {
        Window* level = new Window();
        level->name = "Level";
        level->id = "level-window";
        level->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 1, 50);

        level->modules.push_back(new Module("Noclip", "noclip", "Prevents the player from dying"));
        level->modules.push_back(new Module("Instant Complete", "instant", "Instantly completes a level"));
        //level->modules.push_back(new Module("Startpos Switcher", "startpos-switcher", "Switch between start-positions in the level"));
        //level->modules.push_back(new Module("Show Hitboxes", "hitbox"));

        Client::instance->windows.push_back(level);
    }

    static void SetupBypass()
    {
        Window* bypass = new Window();
        bypass->name = "Universal";
        bypass->id = "bypass-window";
        bypass->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 0, 50);


        bypass->modules.push_back(new Module("Practice Music Sync", "practice-bypass", "Unlocks the Practice Music Sync button in the pause menu options"));
        bypass->modules.push_back(new Module("Unlock All Icons", "icon-bypass", "It... Unlocks all the icons, You should know what this does"));
        bypass->modules.push_back(new Module("Music Unlocker", "music-bypass", "Unlocks the menu and practice music buttons"));


        Client::instance->windows.push_back(bypass);
    }

    static void SetupCosmetic()
    {
        Window* cosmetic = new Window();
        cosmetic->name = "Cosmetic";
        cosmetic->id = "cosmetic-window";
        cosmetic->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 3, 50);

        //cosmetic->modules.push_back(new Module("Hide Endscreen BTN", "end-screen", "Adds an arrow to hide the end screen"));
        //cosmetic->modules.push_back(new Module("No Transition", "no-trans", "Disables the fade scene transitions"));
        cosmetic->modules.push_back(new Module("No Rotation", "no-rot", "Disables ALL rotation on ALL objects. This can make some levels impossible"));
        //cosmetic->modules.push_back(new Module("No Player Rotation", "no-plr-rot", "Disables Player Rotation :3\nIt looks ugly but you do you"));

        Client::instance->windows.push_back(cosmetic);
    }

    static void SetupMisc()
    {
        Window* misc = new Window();
        misc->name = "Misc";
        misc->id = "misc-window";
        misc->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 4, 50);

        //misc->modules.push_back(new Module("Use Full Options", "full-options", "Opens the full options menu instead of the mini options in the pause menu.\nAlso adds a button to the normal pause menu to open the mini options."));

        Client::instance->windows.push_back(misc);
    }

    static void SetupReplay()
    {
        Window* replay = new Window();
        replay->name = "Robtop Replay";
        replay->id = "replay-window";
        replay->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 5, 50);

        replay->modules.push_back(new InfoModule("Coming Soon", "This replay bot is called Robtop Replay because it uses Robtop's replay code"));

        Client::instance->windows.push_back(replay);
    }

    static void SetupStatus()
    {
        Window* replay = new Window();
        replay->name = "Status";
        replay->id = "status-window";
        replay->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 6, 50);

        replay->modules.push_back(new StatusOpacity());
        replay->modules.push_back(new StatusScale());

        replay->modules.push_back(new Module("Testmode", "status-testmode", "Show the test mode text if there's a startpos"));
        //replay->modules.push_back(new Module("Cheat Indicator", "", ""));
        replay->modules.push_back(new Module("FPS Counter", "status-fps", "Shows your current game fps"));
        //replay->modules.push_back(new Module("Noclip Deaths", "status-death", "Shows your death count (hidden when noclip is disabled)"));
        //replay->modules.push_back(new Module("Noclip Accuracy", "status-accuracy", "Shows your death accuracy (hidden when noclip is disabled)"));
        //replay->modules.push_back(new Module("Attempts", "status-attempts", "Shows your attempt count"));
        //replay->modules.push_back(new Module("Clicks", "status-clicks", "Shows your click count"));
        //replay->modules.push_back(new Module("Message", "status-message", "Write a message of your choice to be shown"));
        //replay->modules.push_back(new StatusMessage());

        Client::instance->windows.push_back(replay);
    }

    static void SetupOptions(bool android)
    {
        Window* replay = new Window();
        replay->name = "Config";
        replay->id = "options-window";
        replay->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 4, 50);

        if (android)
        {
            replay->modules.push_back(new AltModuleLocation());
            replay->modules.push_back(new CompactMode());
        }

        Client::instance->windows.push_back(replay);
    }

    /*static void SetupCreator()
    {
        Window* creator = new Window();
        creator->name = "Creator";
        creator->id = "creator-window";
        creator->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 4, 50);

        creator->modules.push_back(new Module("Copy Hack", "copy-hack"));

        Client::instance->windows.push_back(creator);
    }

    static void SetupVariables()
    {
        Window* variables = new Window();
        variables->name = "Variables";
        variables->id = "variables-window";
        variables->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 4, 50);

        auto mName = new InputModule();
        mName->id = "var-id";
        mName->allowedChars = "0987654321";
        variables->modules.push_back(mName);

        Client::instance->windows.push_back(variables);
    }*/

#pragma endregion
};