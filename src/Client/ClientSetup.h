#include "Client.h"

class ClientUtils
{
public:
	static void Setup(bool android = false)
	{
        SetupLevel();
        SetupBypass();        
        SetupCosmetic();
        SetupIconEffects();
        SetupSpeedhack();

        SetupVariables(android);

        Window* level = new Window();
        level->name = "Replay";
        level->id = "replay-window";
        Client::instance->windows.push_back(level);

        SetupOptions(android);

        if (android)
            Client::instance->windows.push_back(new Credits());
	}

#pragma region Setup Windows

    static void SetupSpeedhack()
    {
        Window* speedhack = new Speedhack();
        speedhack->name = "Speedhack";
        speedhack->id = "speedhack-window";
        speedhack->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 2, 50);

        speedhack->modules.push_back(new SpeedhackTop());
        speedhack->modules.push_back(new Module("Enabled", "speedhack-enabled", "Is the speedhack enabled?"));
        speedhack->modules.push_back(new Module("Speedhack Music", "speedhack-music", "Speedhack all sound by your speed modifier", true));
        speedhack->modules.push_back(new Module("Gameplay Only", "speedhack-gameplay", "Only enables the speedhack in gameplay and editor"));

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
        level->modules.push_back(new Module("Instant Complete", "instant", "Instantly completes a level.\nMay not work because of the <cg>Geometry Dash</c> anti-cheat.\nUsing this cheat on a <co>rated level</c> <cr>WILL GET YOU LEADERBOARD BANNED</c>"));
        #ifndef GEODE_IS_ANDROID
        level->modules.push_back(new Module("Force Platformer", "force-plat", "Force Platformer mode on all levels.\nOnly supported on <cl>Windows</c> for the time being"));
        #endif
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
        bypass->modules.push_back(new Module("Fire in the hole!", "fire-in-the-hole", "Replaces every single sfx with\n<cr>FIRE IN THE HOLE!</c>"));

        bypass->modules.push_back(new Module("Copy any level", "copy-hack", "Allows you to copy any level from the servers\nCode by <co>Firee</c>"));
        bypass->modules.push_back(new Module("No Copy Mark", "noc-hack", "Hides the (c) mark from your levels on publish.\nCode by <co>Firee</c>"));

        bypass->modules.push_back(new Module("Auto Safe mode", "auto-safe-mode", "Automatically enables safe mode if you have cheats enabled", true));
        bypass->modules.push_back(new Module("Safe mode", "safe-mode", "Disables all progress on levels"));

        bypass->modules.push_back(new Module("Character Limit", "char-limit", "Removes the <cl>character limit</c> on input fields"));
        bypass->modules.push_back(new Module("Character Filter", "char-filter", "Allows you to input <cg>any</c> character in input fields"));

        #ifdef GEODE_IS_ANDROID
        //bypass->modules.push_back(new Module("Slider Limit Bypass", "slider-limit", "Allows sliders to go beyond the limit of the slider. <cr>Doesn't work for scaling in the editor currently</c>"));
        #endif


        Client::instance->windows.push_back(bypass);
    }

    static void SetupCosmetic()
    {
        Window* cosmetic = new Window();
        cosmetic->name = "Cosmetic";
        cosmetic->id = "cosmetic-window";
        cosmetic->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 3, 50);

        //cosmetic->modules.push_back(new Module("Hide Endscreen BTN", "end-screen", "Adds an arrow to hide the end screen"));
        cosmetic->modules.push_back(new Module("No Transition", "no-trans", "Disables the fade scene transitions"));
        #ifndef GEODE_IS_ANDROID32
        cosmetic->modules.push_back(new Module("No Player Rotation", "no-rot", "Disables rotation on players"));
        #endif
        #ifdef GEODE_IS_WINDOWS
        cosmetic->modules.push_back(new Module("No Particles", "no-particles", "Disables particles, seems pretty obvious what this does imo"));
        #endif
        cosmetic->modules.push_back(new Module("No Shaders", "no-shaders", "Disables shaders, <cl>maybe read</c> the name"));
        cosmetic->modules.push_back(new Module("No Death Effect", "no-death", "Disables the death effect on the player"));

        cosmetic->modules.push_back(new Module("No Wave Trail", "no-wave", "Disables the wave trail"));
        cosmetic->modules.push_back(new Module("Solid Wave Trail", "solid-wave", "Makes the wave trail a solid colour"));

        cosmetic->modules.push_back(new Module("Transparent BG", "trans-bg", "Disables the colouring on the gradient backgrounds"));
        cosmetic->modules.push_back(new Module("Transparent Lists", "trans-lists", "Disables the colouring on all cells"));
        //cosmetic->modules.push_back(new Module("No Player Rotation", "no-plr-rot", "Disables Player Rotation :3\nIt looks ugly imo but you do you"));

        Client::instance->windows.push_back(cosmetic);
    }

    static void SetupIconEffects()
    {
        Client::instance->windows.push_back(new IconEffects());
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
        if (android)
        {
            auto cfg = new Config();
            
            cfg->modules.push_back(new Module("Draggable Ball", "allow-dragging", "Allow dragging the ball on screen", true));
            cfg->modules.push_back(new Module("Save Ball Position", "save-pos", "Saves the position of the ball on screen between sessions", true));

            Client::instance->windows.push_back(cfg);
            return;
        }

        Window* replay = new Window();
        replay->name = "Config";
        replay->id = "options-window";
        replay->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 4, 50);

        if (android)
        {
            replay->modules.push_back(new AltModuleLocation());
            replay->modules.push_back(new CompactMode());
        }
        else
        {
            replay->modules.push_back(new Module("Draggable Ball", "allow-dragging", "Allow dragging the ball on screen"));
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
    }*/

    static void SetupVariables(bool android)
    {
        return;
                
        if (android)
            Client::instance->windows.push_back(new Variables());

        //Client::instance->windows.push_back(new Variables());
    }

#pragma endregion
};