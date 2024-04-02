#pragma once

#include "Client.h"
#include "HitboxesModule.h"

class ClientUtils
{
public:
	static void Setup(bool android = false)
	{
        SetupLevel();
        SetupBypass();
        SetupCreator();
        SetupCosmetic();
        SetupIconEffects();
        SetupSpeedhack();
        SetupStatus();

        Window* level = new _Replay();
        level->name = "Replay";
        level->id = "replay-window";
        Client::instance->windows.push_back(level);

        SetupOptions(android);

        if (!android)
            setupTheming();

        if (!android)
            setupDevtools();

        Client::instance->onPostSetup();
	}

#pragma region Setup Windows

    static void SetupSpeedhack()
    {
        Window* speedhack = new Speedhack();
        speedhack->name = "Speedhack";
        speedhack->id = "speedhack-window";
        speedhack->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 2, 50);

        speedhack->modules.push_back(new SpeedhackTop());
        speedhack->modules.push_back(new SpeedhackEnabled());
        speedhack->modules.push_back(new SpeedhackMus());
        speedhack->modules.push_back(new SpeedhackGameplay());
        speedhack->modules.push_back(new SpeedhackTransFix());

        //(reinterpret_cast<SpeedhackTop*>(speedhack->modules.front()))->format = "%x";

        Client::instance->windows.push_back(speedhack);
    }

    static void SetupLevel()
    {
        Window* level = new Level();
        level->name = "Level";
        level->id = "level-window";
        level->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 1, 50);

        level->modules.push_back(new Module("Noclip", "noclip", "Prevents the player from dying"));
        #ifndef GEODE_IS_MACOS
        level->modules.push_back(new Module("Instant Complete", "instant", "Instantly completes a level.\nMay not work because of the <cg>Geometry Dash</c> anti-cheat.\nUsing this cheat on a <co>rated level</c> <cr>WILL GET YOU LEADERBOARD BANNED</c>", false, "<cr>Warning!</c>\nUsing this cheat on a <co>rated level</c> <cr>WILL GET YOU LEADERBOARD BANNED</c>", true));
        #endif

        level->modules.push_back(new Module("Confirm Practice", "conf-prac", "confirm practice mode help"));
        level->modules.push_back(new Module("Confirm Restart", "conf-res", "confirm restart help"));

        level->modules.push_back(new Module("No Mirror Portal", "no-reverse", "Disables mirror portals, only bad players use this"));
        level->modules.push_back(new Module("Instant Restart", "instant-restart", "Restarts the level instantly upon death"));

        level->modules.push_back(new Hitboxes());
        level->modules.push_back(new Module("Show Hitboxes On Death", "show-hitboxes-on-death", "Shows object hitboxes on death"));

        level->modules.push_back(new Module("Show Triggers", "show-triggers", "Shows the triggers in normal mode like you're playtesting"));
        level->modules.push_back(new Module("Kill at %", "kill-after", "you're not supposed to be here"));

        level->modules.push_back(new Module("Force Platformer", "force-plat", "Force Platformer mode on all levels."));
        level->modules.push_back(new Module("No Static Camera", "no-static", "Disables static camera"));

        level->modules.push_back(new Module("Quests in Pause", "pause-quests", "Adds a button to open the quests menu in the pause menu"));
        level->modules.push_back(new Module("Coin Finder", "coin-tracers", "Draws a line to secret / user coins in the level"));

        level->modules.push_back(new Module("Accurate Percentage", "accurate-percentage", "Allows you to have any amount of decimal places in your percentage text"));
        level->modules.push_back(new Module("Classic Percentage", "classic-percentage", "Percentage is calculated like in 2.1, The percentage for new best is still calculated like 2.2"));

        //level->modules.push_back(new Module("Show Layout", "show-layout", "Shows the layout of the level, its as <cr>shrimple</c> as that"));
        level->modules.push_back(new Module("Jump Hack", "jump-hack", "Allows you to jump infinitely"));
        level->modules.push_back(new Module("Show Trajectory", "show-trajectory", "Shows the players trajectory, This is broken for Ship and Ufo, in those gamemodes the trajectory may disappear unexpectedly"));


        Client::instance->windows.push_back(level);


        Client::GetModule("noclip")->options.push_back(new Module("Tint on death", "noclip-death-tint", "Tints the screen red when you die in noclip"));
        Client::GetModule("noclip")->options.push_back(new SliderModule("Tint Opacity:", "tint-opacity", 0.25f));


        Client::GetModule("kill-after")->options.push_back(new InputModule("Percent:", "death-percent", "100"));
        Client::GetModule("kill-after")->options.push_back(new InputModule("Time:", "death-time", "6.9"));

        Client::GetModule("coin-tracers")->options.push_back(new ColourModule("Line Colour:", "coin-tracers-colour", ccc3(255, 0, 0)));

        //Client::GetModule("show-layout")->options.push_back(new Module("Keep Camera Triggers", "layout-retain-camera", "Keeps the 2.2 camera triggers in the level", true));


        auto decimals = new InputModule("Decimal Places:", "accurate-percentage-places", "2");
        decimals->allowedChars = "1234567890";
        Client::GetModule("accurate-percentage")->options.push_back(decimals);
    }

    static void SetupBypass()
    {
        Window* bypass = new Universal();
        bypass->name = "Universal";
        bypass->id = "bypass-window";
        bypass->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 0, 50);


        bypass->modules.push_back(new Module("Practice Music Sync", "practice-bypass", "Unlocks the Practice Music Sync button in the pause menu options"));
        bypass->modules.push_back(new Module("Unlock All Icons", "icon-bypass", "It... Unlocks all the icons, You should know what this does"));

        bypass->modules.push_back(new Module("Music Unlocker", "music-bypass", "Unlocks the menu and practice music buttons"));
        bypass->modules.push_back(new Module("Fire in the hole!", "fire-in-the-hole", "Replaces every single sfx with\n<cr>FIRE IN THE HOLE!</c>"));

        bypass->modules.push_back(new Module("Auto Safe mode", "auto-safe-mode", "Automatically enables safe mode if you have cheats enabled", true));
        bypass->modules.push_back(new Module("Safe mode", "safe-mode", "Disables all progress on levels"));

        bypass->modules.push_back(new Module("Character Limit", "char-limit", "Removes the <cl>character limit</c> on input fields", false, "<cr>Warning!</c>\nThis module has been <cl>known</c> to cause issues on <cg>some</c> devices such as:\n<cl>- Crashing when you type</c>\n- Crashing when the input field goes away.\nIf the game crashes pressing an ok button, <cr>Disable</c> this module", true));
        bypass->modules.push_back(new Module("Character Filter", "char-filter", "Allows you to input <cg>any</c> character in input fields", false, "<cr>Warning!</c>\nThis module has been <cl>known</c> to cause issues on <cg>some</c> devices such as:\n<cl>- Crashing when you type</c>\n- Crashing when the input field goes away.\nIf the game crashes pressing an ok button, <cr>Disable</c> this module", true));

        bypass->modules.push_back(new Module("Auto Song Download", "auto-song", "Automatically downloads songs when you open a level"));
        bypass->modules.push_back(new Module("Full Options Menu", "full-options", "Replaces the mini options menu in the pause menu with the full options menu"));

        //bypass->modules.push_back(new Module("Slider Limit Bypass", "slider-limit", "Allows sliders to go beyond the limit of the slider. <cr>Doesn't work for scaling in the editor currently</c>"));
        
        Client::instance->windows.push_back(bypass);
    }

    static void SetupCreator()
    {
        Window* creator = new Window();
        creator->name = "Creator";
        creator->id = "creator-window";
        creator->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 0, 50);

        creator->modules.push_back(new Module("Copy any level", "copy-hack", "Allows you to copy any level from the servers\nCode by <co>Firee</c>"));
        creator->modules.push_back(new Module("No Copy Mark", "noc-hack", "Hides the (c) mark from your levels on publish.\nCode by <co>Firee</c>"));

        creator->modules.push_back(new Module("Level Edit", "level-edit", "Allows you to edit any level"));
        creator->modules.push_back(new Module("Verify Hack", "verify-hack", "Lets you upload levels without verifying them"));

        #ifdef GEODE_IS_WINDOWS
        creator->modules.push_back(new Module("Free Scroll", "free-scroll", "Allows you to scroll past the limits of the editor"));
        creator->modules.push_back(new Module("No Custom Object Limit", "custom-obj-limit", "Removes the custom object limit"));
        #endif

        //creator->modules.push_back(new Module("Free Scroll", "free-scroll", "Allows you to scroll past the limits of the editor"));

        #ifdef GEODE_IS_ANDROID
        //bypass->modules.push_back(new Module("Slider Limit Bypass", "slider-limit", "Allows sliders to go beyond the limit of the slider. <cr>Doesn't work for scaling in the editor currently</c>"));
        #endif


        Client::instance->windows.push_back(creator);
    }

    static void SetupCosmetic()
    {
        Window* cosmetic = new Window();
        cosmetic->name = "Cosmetic";
        cosmetic->id = "cosmetic-window";
        cosmetic->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 3, 50);

        //cosmetic->modules.push_back(new Module("Hide Endscreen BTN", "end-screen", "Adds an arrow to hide the end screen"));
        cosmetic->modules.push_back(new Module("No Transition", "no-trans", "Disables the fade scene transitions"));
        cosmetic->modules.push_back(new Module("No Camera Shake", "no-shake", "Disables camera shake globally"));

        cosmetic->modules.push_back(new Module("No Shaders", "no-shaders", "Disables shaders, <cl>maybe read</c> the name"));
        cosmetic->modules.push_back(new Module("No Death Effect", "no-death", "Disables the death effect on the player"));

        cosmetic->modules.push_back(new Module("No Wave Trail", "no-wave", "Disables the wave trail"));
        cosmetic->modules.push_back(new Module("Solid Wave Trail", "solid-wave", "Makes the wave trail a solid colour"));

        cosmetic->modules.push_back(new Module("Transparent BG", "trans-bg", "Disables the colouring on the gradient backgrounds"));
        cosmetic->modules.push_back(new Module("Transparent Lists", "trans-lists", "Disables the colouring on all cells"));

        cosmetic->modules.push_back(new Module("Force Trail On", "trail-on", "Forces the trail on"));
        cosmetic->modules.push_back(new Module("Force Trail Off", "trail-off", "Forces the trail off"));

        cosmetic->modules.push_back(new Module("No Glow", "no-glow", "Disables Object Glow"));
        cosmetic->modules.push_back(new Module("No Respawn Blink", "no-blink", "Disables the blinking when the player respawns"));

        cosmetic->modules.push_back(new Module("No Circles", "no-circles", "Disables circle effects such as the one when you touch an orb"));
        cosmetic->modules.push_back(new Module("No Wave Pulse", "no-wave-pulse", "Disables wave pulse"));

        cosmetic->modules.push_back(new Module("Pulse Menu", "menu-pulse", "Pulses the <cg>Geometry Dash</c> logo on the main menu to the music"));
        #ifndef GEODE_IS_ANDROID
        cosmetic->modules.push_back(new Module("Pulse Scene", "all-pulse", "Pulses the CCScene to the music, <cl>why would you want this?</c>"));
        #endif

        //cosmetic->modules.push_back(new Module("Force Don't Fade", "dont-fade", "Forces all object to be Don't Fade"));
        //cosmetic->modules.push_back(new Module("Force Don't Enter", "dont-enter", "Forces all object to be Don't Enter"));

        #ifndef GEODE_IS_ANDROID32
        cosmetic->modules.push_back(new Module("No Player Rotation", "no-rot", "Disables rotation on players"));
        #endif
        cosmetic->modules.push_back(new Module("No Particles", "no-particles", "Disables particles, seems pretty obvious what this does imo"));

        //cosmetic->modules.push_back(new Module("No Camera Movement", "no-camera", "Disables camera movements that are made with <cl>triggers</c>"));
        //cosmetic->modules.push_back(new Module("No Player Rotation", "no-plr-rot", "Disables Player Rotation :3\nIt looks ugly imo but you do you"));

        Client::instance->windows.push_back(cosmetic);


        Client::GetModule("no-wave-pulse")->options.push_back(new SliderModule("Pulse Size:", "wave-pulse-size", 0.5f));
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

    static void SetupStatus()
    {
        Window* replay = new Status();
        replay->name = "Labels";
        replay->id = "status-window";
        replay->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 6, 50);

        replay->modules.push_back(new StatusOpacity());
        replay->modules.push_back(new StatusScale());

        //replay->modules.push_back(new Module("Testmode", "status-testmode", "Show the test mode text if there's a startpos"));
        replay->modules.push_back(new Module("Cheat Indicator", "cheat-indicator", "Shows if you are cheating"));
        replay->modules.push_back(new Module("FPS Counter", "status-fps", "Shows your current game fps"));
        replay->modules.push_back(new Module("Noclip Accuracy", "status-accuracy", "Shows your accuracy in noclip (hidden when noclip is disabled)"));
        replay->modules.push_back(new Module("Noclip Deaths", "status-deaths", "Shows your deaths in noclip (hidden when noclip is disabled)"));
        replay->modules.push_back(new Module("Attempts", "status-attempt", "Shows Attempt Count"));
        //replay->modules.push_back(new Module("Clicks", "status-cps", "Shows Attempt Count"));
        
        replay->modules.push_back(new Module("Replay Status", "status-replay", "Replay debug info"));
        //replay->modules.push_back(new Module("Noclip Deaths (not fully accurate)", "status-death", "Shows your death count (hidden when noclip is disabled)"));
        //replay->modules.push_back(new Module("Noclip Accuracy", "status-accuracy", "Shows your death accuracy (hidden when noclip is disabled)"));
        //replay->modules.push_back(new Module("Attempts", "status-attempts", "Shows your attempt count"));
        //replay->modules.push_back(new Module("Clicks", "status-clicks", "Shows your click count"));
        replay->modules.push_back(new Module("Message", "status-message", "Write a message of your choice to be shown"));
        //replay->modules.push_back(new StatusMessage());

        Client::instance->windows.push_back(replay);

        auto messageOption = new InputModule("Message Text: ", "status-message-text", "Default Message");
        messageOption->allowedChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_=+[]{};:\'\",.<>/?|`~ ";
        messageOption->description = "What text is displayed";
        messageOption->maxSize = 48; // its just a bit before it overflows on 16:9, perfect
        Client::GetModule("status-message")->options.push_back(messageOption);
    }

    static void SetupOptions(bool android)
    {
        //if (android)
        if (true)
        {
            auto cfg = new Config();
            
            cfg->modules.push_back(new Module("Draggable Ball", "allow-dragging", "Allow dragging the ball on screen", true));
            cfg->modules.push_back(new Module("Save Ball Position", "save-pos", "Saves the position of the ball on screen between sessions", true));
            cfg->modules.push_back(new Module("Additional Width", "npesta-width", "Add's borders to the sides of the menu. Looks weird with <cl>vanilla gd pack</c> but looks nice with <cg>npesta texture pack</c>"));
            cfg->modules.push_back(new Module("Instant Button Fade", "instant-fade", "Makes the mod menu button fade instantly"));
            #ifdef GEODE_IS_DESKTOP
            cfg->modules.push_back(new Module("Hide Button", "hide-btn", "Hides the button to open the menu, use the <cl>Tab</c> or <cl>Insert</c> key to open the mod menu", true));
            cfg->modules.push_back(new CompactMode());
            cfg->modules.push_back(new AltModuleLocation());
            #endif        

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
        if (android)
            Client::instance->windows.push_back(new Variables());

        //Client::instance->windows.push_back(new Variables());
    }

    static void setupTheming()
    {
        Window* theming = new Window();
        theming->name = "Theming";
        theming->id = "themes-window";
        theming->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 5, 50);

        theming->modules.push_back(new ColourModule("Accent", "accent-colour", ccc3(10, 0, 0)));

        Client::instance->windows.push_back(theming);
    }

    static void setupDevtools()
    {
        Window* devtools = new Window();
        devtools->name = "Developer";
        devtools->id = "devtools-window";
        devtools->windowPos = ImVec2(50 + (50 + (Client::instance->tileSize.x)) * 5, 50);

        devtools->modules.push_back(new Module("Recompile Shader", "recomp-shaders", "Recompiles blur shader"));

        Client::instance->windows.push_back(devtools);
    }

#pragma endregion
};