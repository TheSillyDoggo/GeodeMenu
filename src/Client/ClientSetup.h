#pragma once

#include "Windows/Credits.hpp"
#include "Windows/Level.hpp"
#include "Windows/Speedhack.hpp"
#include "Windows/Uncomplete.hpp"
#include "Windows/Universal.hpp"
#include "Windows/Variables.hpp"
#include "Windows/Labels.hpp"
#include "Windows/Config.hpp"
#include "Windows/IconEffects.hpp"

#include "Client.h"
#include "HitboxesModule.h"
#include "../Labels/Labels.h"
#include "../Hacks/SafeMode/SafeMode.hpp"

#include "idkwhattocallthis.hpp"

class ClientUtils
{
public:
	static void Setup(bool android = false)
	{
        SetupLevel();
        //SetupBypass();
        SetupUniversal();
        SetupCreator();
        SetupCosmetic();
        SetupIconEffects();
        SetupSpeedhack();
        SetupStatus();

        //Window* level = new _Replay();
        //level->name = "Replay";
        //level->id = "replay-window";
        //Client::instance->windows.push_back(level);

        SetupOptions(android);

        if (!android)
            setupTheming();

        if (!android)
            setupDevtools();

        registerIncompatibilitys();
        SafeMode::get()->addDelegateToModules();
	}

#pragma region Setup Windows

    static void SetupSpeedhack()
    {
        Window* speedhack = new Speedhack();
        speedhack->name = "Speedhack";
        speedhack->id = "speedhack-window";
        //speedhack->windowPos = ccp(50 + (50 + (Client::instance->tileSize.x)) * 2, 50);

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
        //level->windowPos = ccp(50 + (50 + (Client::instance->tileSize.x)) * 1, 50);

        level->modules.push_back(new Module("Noclip", "noclip", "Prevents the player from dying"));
        level->modules.push_back(new Module("Instant Complete", "instant", "Instantly completes a level.\nMay not work because of the <cg>Geometry Dash</c> anti-cheat.\nUsing this cheat on a <co>rated level</c> <cr>WILL GET YOU LEADERBOARD BANNED</c>", false, "<cr>Warning!</c>\nUsing this cheat on a <co>rated level</c> <cr>WILL GET YOU LEADERBOARD BANNED</c>", true));

        level->modules.push_back(new Module("Confirm Practice", "conf-prac", "Adds a popup to confirm if you want to enter practice mode"));
        level->modules.push_back(new Module("Confirm Restart", "conf-res", "Adds a popup to confirm if you want to restart the level"));

        level->modules.push_back(new Module("StartPos Switcher", "startpos-switcher", "Switch between start positions in levels", true));
        level->modules.push_back(new Module("Smart StartPos", "smart-startpos", "Automatically adjusts the settings of a startpos to correctly set the gamemode, speed, and more", true));

        level->modules.push_back(new Module("Custom Respawn Time", "custom-respawn-time", "Customize the time taken to respawn"));
        level->modules.push_back(new Module("Instant Restart", "instant-restart", "Restarts the level instantly upon death"));

        level->modules.push_back(new Hitboxes());
        level->modules.push_back(new Module("Show Hitboxes On Death", "show-hitboxes-on-death", "Shows object hitboxes on death"));

        level->modules.push_back(new Module("Force Platformer", "force-plat", "Force Platformer mode on all levels.\nReopen level to apply."));
        level->modules.push_back(new Module("No Static Camera", "no-static", "Disables static camera"));

        level->modules.push_back(new Module("No Mirror Portal", "no-reverse", "Disables mirror portals, only bad players use this"));
        level->modules.push_back(new Module("Coin Finder", "coin-tracers", "Draws a line to secret / user coins in the level"));

        level->modules.push_back(new Module("Accurate Percentage", "accurate-percentage", "Allows you to have any amount of decimal places in your percentage text"));
        level->modules.push_back(new Module("Classic Percentage", "classic-percentage", "Percentage is calculated like in 2.1, The percentage for new best is still calculated like 2.2"));

        level->modules.push_back(new Module("Show Layout", "show-layout", "Shows the layout of the level, its as <cr>shrimple</c> as that"));
        level->modules.push_back(new Module("Show Triggers", "show-triggers", "Shows the triggers in normal mode like you're playtesting"));

        level->modules.push_back(new Module("Quests in Pause", "pause-quests", "Adds a button to open the quests menu in the pause menu", true));
        level->modules.push_back(new Module("Show Trajectory", "show-trajectory", "Shows the players trajectory, This is broken for Ship and Ufo, in those gamemodes the trajectory may disappear unexpectedly"));

        level->modules.push_back(new Module("All Modes Platformer", "all-plat", "Allows all gamemodes in platformer mode"));
        level->modules.push_back(new Module("Random Seed", "rand-seed", "Sets the seed for random triggers to a value of your choice"));

        level->modules.push_back(new Module("Conditional Auto-Retry", "conditional-retry", "Shows the retry level popup ONLY when you get a new best"));
        level->modules.push_back(new Module("Auto Practice Mode", "auto-practice", "Automatically puts you into practice mode when you open a level"));

        level->modules.push_back(new Module("Kill at %", "kill-after", "Kills the player after a set percentage"));
        level->modules.push_back(new Module("Jump Hack", "jump-hack", "Allows you to jump infinitely"));

        level->modules.push_back(new Module("Pause Countdown", "pause-countdown", "Shows a countdown for 3 seconds when you unpause a level"));
        level->modules.push_back(new Module("1 Attempt Practice", "practice-complete", "Allows you to complete a level in practice mode if you beat it in <cr>one</c> attempt", true));

        level->modules.push_back(new Module("Best In Percentage", "best-in-percentage", "Shows the best percentage in the progress bar"));
        level->modules.push_back(new Module("Auto Collect Coins", "auto-coins", "Automatically collect coins in level"));

        level->modules.push_back(new Module("Suicide", "you-should-kill-yourself-now", "Kills the player instantly"));
        level->modules.push_back(new Module("No Checkpoint Limit", "no-checkpoint-limit", "Removes the 50 checkpoint limit from practice mode"));
        
        level->modules.push_back(new Module("Auto Clicker", "auto-clicker", "Automatically clicks (and holds for) every X ticks"));
        level->modules.push_back(new Module("Stop Triggers on Death", "stop-triggers-on-death", "Stops triggers whenever you die :3"));
        //level->modules.push_back(new Module("Frame Stepper", "frame-stepper", "Step the game through frames by tapping a button"));


        Client::instance->windows.push_back(level);


        Client::GetModule("noclip")->options.push_back(new Module("Tint on death", "noclip-death-tint", "Tints the screen red when you die in noclip"));
        Client::GetModule("noclip")->options.push_back(new SliderModule("Tint Opacity:", "tint-opacity", 0.25f));
        Client::GetModule("noclip")->options.push_back(new ColourModule("Tint Colour:", "noclip-tint-colour", ccc3(255, 0, 0)));


        Client::GetModule("kill-after")->options.push_back(new InputModule("Percent:", "death-percent", "100"));
        Client::GetModule("kill-after")->options.push_back(new InputModule("Time:", "death-time", "6.9"));

        Client::GetModule("coin-tracers")->options.push_back(new ColourModule("Line Colour:", "coin-tracers-colour", ccc3(255, 0, 0)));

        Client::GetModule("show-layout")->options.push_back(new Module("Keep Camera Triggers", "layout-retain-camera", "Keeps the 2.2 camera triggers in the level", true));
        Client::GetModule("show-layout")->options.push_back(new ColourModule("BG Colour:", "show-layout-bg", ccc3(40, 125, 255)));
        Client::GetModule("show-layout")->options.push_back(new ColourModule("Ground Colour:", "show-layout-g", ccc3(0, 102, 255)));


        auto decimals = new InputModule("Decimal Places:", "accurate-percentage-places", "2");
        decimals->allowedChars = "1234567890";
        Client::GetModule("accurate-percentage")->options.push_back(decimals);

        auto seed = new InputModule("Seed:", "rand-seed-seed", "69420");
        seed->allowedChars = "1234567890";
        seed->maxSize = 16;
        Client::GetModule("rand-seed")->options.push_back(seed);

        auto cdownT = new InputModule("Time:", "countdown-time", "3");
        cdownT->allowedChars = "1234567890";
        cdownT->maxSize = 4;
        Client::GetModule("pause-countdown")->options.push_back(cdownT);

        Client::GetModule("startpos-switcher")->options.push_back(new SliderModule("Opacity:", "startpos-opacity", 50.0f / 255.0f));
        //Client::GetModule("smart-startpos")->options.push_back(new SmartStartposUIModule());
        //Client::GetModule("smart-startpos")->optionSizeForce = Client::GetModule("smart-startpos")->options[0]->sizeForOptionsPage();

        Client::GetModule("custom-respawn-time")->options.push_back(new InputModule("Delay:", "respawn-time-delay", "4.2069"));

        Client::GetModule("auto-clicker")->options.push_back(new InputModule("Interval", "auto-clicker-delay", "6"));
        Client::GetModule("auto-clicker")->options.push_back(new InputModule("Hold For", "auto-clicker-hold-for", "3"));
        Client::GetModule("auto-clicker")->options.push_back(new Module("Player 1", "auto-clicker-player1", "Should it click for player 1", true));
        Client::GetModule("auto-clicker")->options.push_back(new Module("Player 2", "auto-clicker-player2", "Should it click for player 2", true));
        as<InputModule*>(Client::GetModule("auto-clicker")->options[0])->allowedChars = "0123456789";
        as<InputModule*>(Client::GetModule("auto-clicker")->options[1])->allowedChars = "0123456789";
    }

    static void SetupBypass()
    {
        Window* bypass = new Window();
        bypass->name = "Bypass";
        bypass->id = "bypass-window";
        //bypass->windowPos = ccp(50 + (50 + (Client::instance->tileSize.x)) * 0, 50);

        //bypass->modules.push_back(new Module("Slider Limit Bypass", "slider-limit", "Allows sliders to go beyond the limit of the slider. <cr>Doesn't work for scaling in the editor currently</c>"));
        
        Client::instance->windows.push_back(bypass);
    }

    static void SetupUniversal()
    {
        Window* bypass = new Universal();
        bypass->name = "Universal";
        bypass->id = "universal-window";
        //bypass->windowPos = ccp(50 + (50 + (Client::instance->tileSize.x)) * 0, 50);


        bypass->modules.push_back(new Module("Practice Music Sync", "practice-bypass", "Unlocks the Practice Music Sync button in the pause menu options"));
        bypass->modules.push_back(new Module("Unlock All Icons", "icon-bypass", "It... Unlocks all the icons, You should know what this does"));

        bypass->modules.push_back(new Module("Music Customiser Bypass", "music-bypass", "Unlocks the menu and practice music buttons"));
        bypass->modules.push_back(new Module("Fire in the hole!", "fire-in-the-hole", "Replaces every single sfx with\n<cr>FIRE IN THE HOLE!</c>"));

        bypass->modules.push_back(new Module("Auto Safe mode", "auto-safe-mode", "Automatically enables safe mode if you have cheats enabled", true));
        bypass->modules.push_back(new Module("Safe mode", "safe-mode", "Disables all progress on levels"));

        bypass->modules.push_back(new Module("Character Limit", "char-limit", "Removes the <cl>character limit</c> on input fields", false, "<cr>Warning!</c>\nThis mod has been <cl>known</c> to cause issues on <cg>some</c> devices such as:\n<cl>- Crashing when you type</c>\n- Crashing when the input field goes away.\nIf the game crashes pressing an ok button, <cr>Disable</c> this mod", true));
        bypass->modules.push_back(new Module("Character Filter", "char-filter", "Allows you to input <cg>any</c> character in input fields", false, "<cr>Warning!</c>\nThis mod has been <cl>known</c> to cause issues on <cg>some</c> devices such as:\n<cl>- Crashing when you type</c>\n- Crashing when the input field goes away.\nIf the game crashes pressing an ok button, <cr>Disable</c> this mod", true));

        bypass->modules.push_back(new Module("Main Levels", "main-level-bypass", "Allows you to play the main demon levels"));
        bypass->modules.push_back(new Module("Tower Levels", "tower-level-bypass", "Allows you to play the levels in the tower"));

        bypass->modules.push_back(new Module("Auto Song Download", "auto-song", "Automatically downloads songs when you open a level"));
        bypass->modules.push_back(new Module("Full Options Menu", "full-options", "Replaces the mini options menu in the pause menu with the full options menu"));

        bypass->modules.push_back(new Module("Physics Bypass", "tps-bypass", "Allows your physics to run at values other than 240"));
        bypass->modules.push_back(new Module("Comment History Bypass", "comment-history", "Allows you to view the comment history of any user, rob still hasn't fixed this"));

        bypass->modules.push_back(new Module("Slider Limit Bypass", "slider-limit", "Allows sliders to go beyond the limit of the slider"));
        bypass->modules.push_back(new Module("Transition Customiser", "custom-trans", "Adjust your scene transition"));

        bypass->modules.push_back(new Module("Pitch Shifter", "pitch-shifter", "Shift the pitch of the game audio"));
        bypass->modules.push_back(new Module("No Short Numbers", "no-short-nums", "Show the full value instead of the short value in level cells"));

        bypass->modules.push_back(new Module("Show Touches", "show-touches", "Shows a circle on the screen whenever you touch"));
        bypass->modules.push_back(new Module("Auto LDM", "auto-ldm", "Automatically enable Low Detail Mode in levels when you view them"));

        bypass->modules.push_back(new Module("Hide Pause Menu", "hide-pause-menu", "Hides the pause menu :3"));
        
        Client::instance->windows.push_back(bypass);

        auto tps = new InputModule("Ticks:", "tps-bypass-value", "240");
        tps->maxSize = 11;
        Client::GetModule("tps-bypass")->options.push_back(tps);

        Client::GetModule("custom-trans")->options.push_back(new TransCustomizerModule());
        //Client::GetModule("custom-trans")->options.push_back(nullptr);
        //Client::GetModule("custom-trans")->options.push_back(new InputModule("Time Mod (*)", "transition-time-modifier", "1.0"));
        //Client::GetModule("custom-trans")->options.push_back(new ColourModule("Fade Colour:", "transition-fade-colour", ccc3(0, 0, 0)));

        auto pitch = new InputModule("Pitch:", "pitch-shifter-value", "1.0");
        pitch->maxSize = 6;
        Client::GetModule("pitch-shifter")->options.push_back(pitch);
    }

    static void SetupCreator()
    {
        Window* creator = new Window();
        creator->name = "Creator";
        creator->id = "creator-window";
        //creator->windowPos = ccp(50 + (50 + (Client::instance->tileSize.x)) * 0, 50);

        creator->modules.push_back(new SetValueModule("Set Scale:", "set-scale"));
        creator->modules.push_back(new SetValueModule("Set Rotation:", "set-rot"));

        creator->modules.push_back(new Module("Copy Hack", "copy-hack", "Allows you to copy any level from the servers\nCode by <co>Firee</c>"));
        creator->modules.push_back(new Module("Show Level Password", "show-password", "Adds a button to the password input popup that shows the level password", true));

        creator->modules.push_back(new Module("No Copy Mark", "noc-hack", "Hides the (c) mark from your levels on publish.\nCode by <co>Firee</c>"));
        creator->modules.push_back(new Module("Level Edit", "level-edit", "Allows you to edit any level"));

        creator->modules.push_back(new Module("Verify Hack", "verify-hack", "Lets you upload levels without verifying them"));
        creator->modules.push_back(new Module("Default Song Bypass", "default-song-bypass", "Removes the main level song limit"));

        #ifdef GEODE_IS_WINDOWS
        creator->modules.push_back(new Module("Free Scroll", "free-scroll", "Allows you to scroll past the limits of the editor"));
        #endif
        creator->modules.push_back(new Module("No Custom Object Limit", "custom-obj-limit", "Removes the custom object limit"));

        creator->modules.push_back(new Module("Editor Wave Trail", "editor-wave-trail", "Shows the wave trail in the editor"));
        creator->modules.push_back(new Module("Smooth Editor Trail", "smooth-editor-trail", "Updates the editor trail at your screen refresh rate instead of 30 fps"));

        creator->modules.push_back(new Module("Editor Extension", "editor-extension", "Increases the length of the editor by <cs>17895</c> times :3"));
        creator->modules.push_back(new Module("Toolbox Button Bypass", "toolbox-buttons", "Unlimited <cc>Rows</c> and <cc>Columns</c> in your <cl>Object Toolbox</c>"));

        creator->modules.push_back(new Module("Scale Input", "scale-input-control", "Adds an input field to the object scale control", true));


        Client::instance->windows.push_back(creator);
    }

    static void SetupCosmetic()
    {
        Window* cosmetic = new Window();
        cosmetic->name = "Cosmetic";
        cosmetic->id = "cosmetic-window";
        //cosmetic->windowPos = ccp(50 + (50 + (Client::instance->tileSize.x)) * 3, 50);

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

        cosmetic->modules.push_back(new Module("Force Ghost On", "ghost-on", "Forces the ghost trail on"));
        cosmetic->modules.push_back(new Module("Force Ghost Off", "ghost-off", "Forces the ghost trail off"));

        cosmetic->modules.push_back(new Module("No Glow", "no-glow", "Disables Object Glow"));
        cosmetic->modules.push_back(new Module("No Respawn Blink", "no-blink", "Disables the blinking when the player respawns"));

        cosmetic->modules.push_back(new Module("No Circles", "no-circles", "Disables circle effects such as the one when you touch an orb"));
        cosmetic->modules.push_back(new Module("No Wave Pulse", "no-wave-pulse", "Disables wave pulse"));

        cosmetic->modules.push_back(new Module("Pulse Menu", "menu-pulse", "Pulses the <cg>Geometry Dash</c> logo on the main menu to the music"));
        cosmetic->modules.push_back(new Module("No \"Do Not\" flip", "just-dont", "Disables the attempt text and endscreen text from being flipped with the <cl>Do Not</c> option enabled."));

        //cosmetic->modules.push_back(new Module("Force Don't Fade", "dont-fade", "Forces all object to be Don't Fade"));
        //cosmetic->modules.push_back(new Module("Force Don't Enter", "dont-enter", "Forces all object to be Don't Enter"));

        #ifndef GEODE_IS_ANDROID32
        cosmetic->modules.push_back(new Module("No Player Rotation", "no-rot", "Disables rotation on players"));
        #endif
        cosmetic->modules.push_back(new Module("No Particles", "no-particles", "Disables particles, seems pretty obvious what this does imo"));
        cosmetic->modules.push_back(new Module("Hide Level", "no-level", "Disables the level visually, good for hitbox only showcases ig"));

        cosmetic->modules.push_back(new Module("Unlock Buttons", "unlock-buttons", "Visually Unlocks the greyed out buttons in the online page"));
        cosmetic->modules.push_back(new Module("No Wave Reset", "no-wave-reset", "Stops the wave trail from disappearing when you change gamemode"));

        cosmetic->modules.push_back(new Module("No Orb Pulse", "no-orb-pulse", "Disables orb's from pulsing, Doesn't work on main levels made before 2.2"));
        cosmetic->modules.push_back(new Module("Main Menu Gameplay", "main-menu-gameplay", "Allows you to control the icons on the main menu.\nI honestly have no idea what category to put it in so its cosmetic now :3"));

        cosmetic->modules.push_back(new Module("Hide Pause Button", "hide-pause-button", "Hides the pause button in game, requires reopening level to apply"));
        cosmetic->modules.push_back(new Module("Coins In Practice", "practice-coins-visual", "Allows you to visually collect coins in practice mode"));

        cosmetic->modules.push_back(new Module("No Robot Fire", "no-robot-fire", "Hides the fire under the robot while it's boosting"));
        cosmetic->modules.push_back(new Module("No Spider Dash Effect", "no-spider-dash", "Hides the spider dash effect when you teleport"));

        cosmetic->modules.push_back(new Module("Longer Trail", "longer-trail", "Lengthens your player's trail length by a factor of 3x"));
        cosmetic->modules.push_back(new Module("No Dash Fire", "no-dash-fire", "Hides the fire behind your icon when using a dash orb"));

        cosmetic->modules.push_back(new Module("Gold User Coins", "gold-user-coins", "Makes user coins appear as if they are gold robtop coins"));
        cosmetic->modules.push_back(new Module("No Ship Fire", "no-ship-fire", "Hides the fire behind the ship while it's flying"));

        cosmetic->modules.push_back(new Module("Ball Rotation Bug", "ball-rotation-bug", "EMULATES the old rotation bug when switching gamemodes from the ball"));


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
        //misc->windowPos = ccp(50 + (50 + (Client::instance->tileSize.x)) * 4, 50);

        //misc->modules.push_back(new Module("Use Full Options", "full-options", "Opens the full options menu instead of the mini options in the pause menu.\nAlso adds a button to the normal pause menu to open the mini options."));

        Client::instance->windows.push_back(misc);
    }

    static void SetupStatus()
    {
        #ifndef STATUS_TEXTS
        return;
        #endif

        Window* replay = new Labels();

        replay->modules.push_back(new StatusOpacity());
        replay->modules.push_back(new StatusScale());

        //replay->modules.push_back(new Module("Testmode", "status-testmode", "Show the test mode text if there's a startpos"));
        replay->modules.push_back(new Module("Labels In Editor", "labels-in-editor", "Shows your labels in the level editor <cr>[EXPERIMENTAL]</c>"));

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
        replay->modules.push_back(new Module("Session Time", "status-session", "Shows the time you've had the game open for in the format <cg>hh::mm::ss</c>"));
        replay->modules.push_back(new Module("CPS Counter", "status-cps", "Shows your clicks per second. Tints <cg>Green</c> while you are clicking"));
        replay->modules.push_back(new Module("Best Run", "best-run", "Shows your best run"));
        replay->modules.push_back(new Module("Clock", "status-clock", "Shows your current device time"));
        replay->modules.push_back(new Module("Percentage", "status-percentage", "Copies the text from the percentage label"));
        //replay->modules.push_back(new StatusMessage());


        Client::instance->windows.push_back(replay);

        for (auto mod : replay->modules)
        {
            if (mod->id != "labels-in-editor")
                mod->options.push_back(new DropdownModule({"Top Left", "Top Right", "Bottom Left", "Bottom Right"}, mod->id + "-side", 0));
        }

        auto messageOption = new InputModule("Message Text: ", "status-message-text", "Default Message");
        messageOption->allowedChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_=+[]{};:\'\",.<>/?|`~ ";
        messageOption->maxSize = 48; // its just a bit before it overflows on 16:9, perfect
        Client::GetModule("status-message")->options.push_back(messageOption);
        Client::GetModule("status-cps")->options.push_back(new Module("Total CPS", "status-cps-total", "Shows the total clicks in the attempt"));
        Client::GetModule("status-cps")->options.push_back(new Module("Instant Fade Transition", "status-cps-instant-fade", "Makes the green fade transition when clicking instant"));

        Client::GetModule("status-clock")->options.push_back(new Module("24 Hour Time", "status-clock-24h-time", "Uses 24 hour time instead of 12 hour time"));

        Client::GetModule("best-run")->options.push_back(new Module("Always Show From", "best-run-always-show-from", "Always shows the from percent even if it's from 0%"));
        Client::GetModule("best-run")->options.push_back(new Module("Show Decimal Places", "best-run-decimals", "Shows decimal places in the run", true));

        #ifdef STATUS_TEXTS
        StatusNode::postSetup(replay);
        #endif
    }

    static void SetupOptions(bool android)
    {
        //if (android)
        if (true)
        {
            auto cfg = new Config();

            cfg->modules.push_back(new SliderModule("Anim Speed:", "anim-speed", 0.5f));
            cfg->modules.push_back(new Module("Additional Border", "npesta-width", "Adds borders to the left and right sides of the mod menu to make the menu look better with some texture packs\nlike the <cg>npesta</c> pack"));
            cfg->modules.push_back(new Module("Blur Background", "menu-bg-blur", "Blurs the background of the mod menu"));

            #ifdef GEODE_IS_DESKTOP
            cfg->modules.push_back(new Module("Hide Button", "hide-btn", "Hides the button to open the menu, use the <cl>Tab</c> or <cl>Insert</c> key to open the mod menu", true));
            #endif
            
            cfg->modules.push_back(new Module("Draggable Button", "allow-dragging", "Allow dragging the ball on screen", true));
            cfg->modules.push_back(new Module("Save Button Position", "save-pos", "Saves the position of the ball on screen between sessions", true));
            cfg->modules.push_back(new Module("Instant Button Fade", "instant-fade", "Makes the mod menu button fade instantly"));
            //cfg->modules.push_back(new CompactMode());
            //cfg->modules.push_back(new AltModuleLocation());

            Client::instance->windows.push_back(cfg);
            return;
        }

        Window* replay = new Window();
        replay->name = "Config";
        replay->id = "options-window";
        //replay->windowPos = ccp(50 + (50 + (Client::instance->tileSize.x)) * 4, 50);

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
        creator->windowPos = ccp(50 + (50 + (Client::instance->tileSize.x)) * 4, 50);

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
        //theming->windowPos = ccp(50 + (50 + (Client::instance->tileSize.x)) * 5, 50);

        theming->modules.push_back(new ColourModule("Accent", "accent-colour", ccc3(10, 0, 0)));

        Client::instance->windows.push_back(theming);
    }

    static void setupDevtools()
    {
        return;

        Window* devtools = new Window();
        devtools->name = "Developer";
        devtools->id = "devtools-window";
        //devtools->windowPos = ccp(50 + (50 + (Client::instance->tileSize.x)) * 5, 50);

        devtools->modules.push_back(new Module("Recompile Shader", "recomp-shaders", "Recompiles blur shader"));

        Client::instance->windows.push_back(devtools);
    }

    static void registerIncompatibilitys()
    {
        #ifdef GEODE_IS_MACOS

        std::vector<std::string> macInc = {
            "show-layout",
            "no-wave",
            "no-particles",
            "solid-wave",
            "no-shake",
            "custom-trans",
            "menu-bg-blur",
        };

        for (auto hack : macInc)
        {
            auto mod = Client::GetModule(hack);

            if (mod)
            {
                mod->setIncompatible("This mod has not <cg>yet</c> been ported to <cl>macOS</c>");
                mod->enabled = false;
            }
        }

        #endif

        #ifdef GEODE_IS_IOS

        SpeedhackTransFix::instance->setIncompatible("This mod does not <cg>yet</c> support <cl>iOS</c>");
        SpeedhackTransFix::instance->enabled = false;

        Client::GetModule("custom-trans")->setIncompatible("This mod does not <cg>yet</c> support <cl>iOS</c>");

        #endif

        #ifndef GEODE_IS_WINDOWS
        //Client::GetModule("custom-obj-limit")->setIncompatible("This mod only supports <cl>Windows</c> <cg>currently</c>");
        #endif

        #ifndef QOLMOD_AUTOSONG
        Client::GetModule("auto-song")->setIncompatible("This mod has <cr>not yet</c> been ported to <cl>2.206</c>");
        #endif

        #ifndef QOLMOD_NOJUSTDONT
        Client::GetModule("just-dont")->setIncompatible("This mod has <cr>not yet</c> been ported to <cl>2.206</c>");
        #endif

        #ifndef QOLMOD_VERIFYHACK
        Client::GetModule("verify-hack")->setIncompatible("This mod has <cr>not yet</c> been ported to <cl>2.206</c>");
        #endif

        #ifndef QOLMOD_PULSINGMENU
        Client::GetModule("menu-pulse")->setIncompatible("This mod has <cr>not yet</c> been ported to <cl>2.206</c>");
        #endif

        #ifdef GEODE_IS_WINDOWS
        //Client::GetModule("tps-bypass")->setIncompatible("This mod has <cr>not yet</c> been ported to <cl>Windows 2.206</c>");
        //Client::GetModule("all-plat")->setIncompatible("This mod has <cr>not yet</c> been ported to <cl>2.206</c> on <cl>Windows</c>.");
        Client::GetModule("no-shake")->setIncompatible("This mod has <cr>not yet</c> been ported to <cl>2.206</c> on <cl>Windows</c>.");
        //Client::GetModule("custom-obj-limit")->setIncompatible("This mod has <cr>not yet</c> been ported to <cl>2.206</c> on <cl>Windows</c>.");

        #ifndef QOLMOD_FREESCROLL
        Client::GetModule("free-scroll")->setIncompatible("This mod has <cr>not yet</c> been ported to <cl>2.206</c>");
        #endif

        #endif

        #ifndef QOLMOD_SHOWLAYOUT
        Client::GetModule("show-layout")->setIncompatible("This mod has <cr>not yet</c> been ported to <cl>2.206</c>");
        #endif

        //Client::GetModule("rand-seed")->setIncompatible("This mod has <cr>not yet</c> been ported to <cl>2.206</c>");
        //Client::GetModule("show-trajectory")->setIncompatible("This mod has <cr>not yet</c> been ported to <cl>2.206</c>");

        #ifdef GEODE_IS_ARM_MAC
        Client::GetModule("tps-bypass")->setIncompatible("This mod has <cr>not yet</c> been ported to <cl>ARM Mac</c>");
        Client::GetModule("editor-wave-trail")->setIncompatible("This mod has <cr>not yet</c> been ported to <cl>ARM Mac</c>");
        Client::GetModule("editor-extension")->setIncompatible("This mod has <cr>not yet</c> been ported to <cl>ARM Mac</c>");
        #endif

        #ifdef GEODE_IS_MACOS
        Client::GetModule("smart-startpos")->setIncompatible("This mod has <cr>not yet</c> been ported to <cl>MacOS</c>");
        #endif

        #ifndef QOLMOD_ALL_MODES_PLATFORMER
        Client::GetModule("all-plat")->setIncompatible("This mod has <cr>not yet</c> been ported to <cl>2.206</c>.");
        #endif
    }

#pragma endregion
};