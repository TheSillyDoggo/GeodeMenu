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
        level->modules.push_back(new Module("Coin Finder", "coin-tracers", "Draws a line to secret / user coins in the level"));

        level->modules.push_back(new Module("No Mirror Portal", "no-reverse", "Disables mirror portals, only bad players use this"));
        level->modules.push_back(new Module("Instant Mirror Portal", "instant-reverse", "Makes the mirror portal transition instant"));        

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

        level->modules.push_back(new Module("Frame Stepper", "frame-stepper", "Step the game through frames by tapping a button"));
        level->modules.push_back(new Module("Allow Pause Buffering", "pause-buffering", "Removes the fix for pause buffering (the old sakupen circles thing)"));

        level->modules.push_back(new Module("Force Hide Player", "hide-player", "Forces all players to be invisible"));
        level->modules.push_back(new Module("Force Show Player", "show-player", "Forces all players to be visible"));

        level->modules.push_back(new Module("Hitbox Multiplier", "hitbox-multiplier", "Change the size of hotboxes"));
        level->modules.push_back(new Module("Freeze Attempts", "freeze-attempts", "Freezes your attempt count stat and on level"));

        level->modules.push_back(new Module("No Static Camera", "no-static", "Disables static camera"));
        level->modules.push_back(new Module("Legacy Upside Down Physics", "legacy-upside-down", "Forces the level to have the upside down <cl>gravity bug</c> from before <cc>2.2</c>"));

        level->modules.push_back(new Module("Icon Kit in Pause", "pause-icon-kit", "Adds a button to open the icon kit in the pause menu", true));

        //level->modules.push_back(new Module("Gamemode Switcher", "gamemode-switcher", "Adds a button to the bottom of the pause menu to change your gamemode"));


        Client::instance->windows.push_back(level);


        Client::GetModule("noclip")->options.push_back(new Module("Tint on death", "noclip-death-tint", "Tints the screen red when you die in noclip"));
        Client::GetModule("noclip")->options.push_back(new SliderModule("Tint Opacity:", "tint-opacity", 0.25f));
        Client::GetModule("noclip")->options.push_back(new ColorModule("Tint Color:", "noclip-tint-Color", ccc3(255, 0, 0)));
        Client::GetModule("noclip")->options.push_back(nullptr);
        Client::GetModule("noclip")->options.push_back(new Module("Minimum Accuracy", "noclip-min-accuracy-toggle", "Allows you to die if your noclip accuracy drops below this value"));
        Client::GetModule("noclip")->options.push_back(new InputModule("Min Accuracy:", "noclip-min-accuracy", "95.0"));
        Client::GetModule("noclip")->options.push_back(new Module("Player 1", "noclip-player1", "Apply noclip to player 1", true));
        Client::GetModule("noclip")->options.push_back(new Module("Player 2", "noclip-player2", "Apply noclip to player 2", true));


        Client::GetModule("kill-after")->options.push_back(new InputModule("Percent:", "death-percent", "100"));
        Client::GetModule("kill-after")->options.push_back(new InputModule("Time:", "death-time", "6.9"));

        Client::GetModule("coin-tracers")->options.push_back(new ColorModule("Line Color:", "coin-tracers-Color", ccc3(255, 0, 0)));

        Client::GetModule("show-layout")->options.push_back(new Module("Keep Camera Triggers", "layout-retain-camera", "Keeps the 2.2 camera triggers in the level", true));
        Client::GetModule("show-layout")->options.push_back(new ColorModule("BG Color:", "show-layout-bg", ccc3(40, 125, 255)));
        Client::GetModule("show-layout")->options.push_back(new ColorModule("Ground Color:", "show-layout-g", ccc3(0, 102, 255)));


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
        Client::GetModule("frame-stepper")->options.push_back(new Module("unused", "unused-module", "this is here because i need a module for the popup"));
        //Client::GetModule("smart-startpos")->options.push_back(new SmartStartposUIModule());
        //Client::GetModule("smart-startpos")->optionSizeForce = Client::GetModule("smart-startpos")->options[0]->sizeForOptionsPage();

        Client::GetModule("custom-respawn-time")->options.push_back(new InputModule("Delay:", "respawn-time-delay", "4.2069"));

        Client::GetModule("auto-clicker")->options.push_back(new InputModule("Interval", "auto-clicker-delay", "6"));
        Client::GetModule("auto-clicker")->options.push_back(new InputModule("Hold For", "auto-clicker-hold-for", "3"));
        Client::GetModule("auto-clicker")->options.push_back(new Module("Player 1", "auto-clicker-player1", "Should it click for player 1", true));
        Client::GetModule("auto-clicker")->options.push_back(new Module("Player 2", "auto-clicker-player2", "Should it click for player 2", true));
        as<InputModule*>(Client::GetModule("auto-clicker")->options[0])->allowedChars = "0123456789";
        as<InputModule*>(Client::GetModule("auto-clicker")->options[1])->allowedChars = "0123456789";

        if (auto hitboxMult = Client::GetModule("hitbox-multiplier"))
        {
            hitboxMult->options.push_back(new InputModule("Player", "hitbox-multiplier-player", "1.0"));
            hitboxMult->options.push_back(nullptr);

            hitboxMult->options.push_back(new InputModule("Solid", "hitbox-multiplier-solid", "0.5"));
            hitboxMult->options.push_back(nullptr);

            hitboxMult->options.push_back(new InputModule("Hazard", "hitbox-multiplier-hazard", "0.5"));
        }
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
        bypass->modules.push_back(new Module("Treasure Room Bypass", "treasure-room-bypass", "Allows you to enter the treasure room without having <cc>5</c> demon keys"));

        bypass->modules.push_back(new Module("Vault Of Secrets Bypass", "vault-of-secrets-bypass", "Allows you to enter the vault of secrets without having <cc>50</c> diamonds"));
        bypass->modules.push_back(new Module("The Challenge Bypass", "the-challenge-bypass", "Allows you to enter the vault of secrets without having <cc>50</c> diamonds"));

        bypass->modules.push_back(new Module("Basement Bypass", "basement-bypass", "Allows you to enter the monsters basement without completing <cc>The Challenge</c>"));
        bypass->modules.push_back(new Module("Basement Key Bypass", "basement-key-bypass", "Allows you to unlock the keys in the monsters basement"));

        bypass->modules.push_back(new Module("Treasure Room Chest Bypass", "chest-unlock-bypass", "Allows you to open any chest in the <cc>treasure room</c>"));
        bypass->modules.push_back(new Module("Unlock All Gauntlets", "unlock-gauntlets", "Allows you to open any level in the gauntlet"));

        bypass->modules.push_back(new Module("December Menu Snow", "december-snow", "Shows snow on the main menu during <cc>december</c>.", true));
        bypass->modules.push_back(new Module("End Screen Cheat Indicator", "end-screen-cheat-indicator", "Shows the cheat indicator on the <cc>end screen</c>.", true));
        Client::instance->windows.push_back(bypass);

        auto tps = new InputModule("Ticks:", "tps-bypass-value", "240");
        tps->maxSize = 11;
        Client::GetModule("tps-bypass")->options.push_back(tps);

        Client::GetModule("custom-trans")->options.push_back(new TransCustomizerModule());
        Client::GetModule("custom-trans")->optionSizeForce = ccp(350, 180);
        //Client::GetModule("custom-trans")->options.push_back(nullptr);
        //Client::GetModule("custom-trans")->options.push_back(new InputModule("Time Mod (*)", "transition-time-modifier", "1.0"));
        //Client::GetModule("custom-trans")->options.push_back(new ColorModule("Fade Color:", "transition-fade-Color", ccc3(0, 0, 0)));

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

        // creator->modules.push_back(new SetValueModule("Set Scale:", "set-scale"));
        // creator->modules.push_back(new SetValueModule("Set Rotation:", "set-rot"));

        creator->modules.push_back(new Module("Copy Hack", "copy-hack", "Allows you to copy any level from the servers\nCode by <co>Firee</c>"));
        creator->modules.push_back(new Module("Show Level Password", "show-password", "Adds a button to the password input popup that shows the level password", true));

        creator->modules.push_back(new Module("No Copy Mark", "noc-hack", "Hides the (c) mark from your levels on publish.\nCode by <co>Firee</c>"));
        creator->modules.push_back(new Module("Level Edit", "level-edit", "Allows you to edit any level"));

        creator->modules.push_back(new Module("Verify Hack", "verify-hack", "Lets you upload levels without verifying them"));
        creator->modules.push_back(new Module("Default Song Bypass", "default-song-bypass", "Removes the main level song limit"));

        creator->modules.push_back(new Module("Free Scroll", "free-scroll", "Allows you to scroll past the limits of the editor"));
        creator->modules.push_back(new Module("No Custom Object Limit", "custom-obj-limit", "Removes the custom object limit"));

        creator->modules.push_back(new Module("Editor Wave Trail", "editor-wave-trail", "Shows the wave trail in the editor"));
        creator->modules.push_back(new Module("Smooth Editor Trail", "smooth-editor-trail", "Updates the editor trail at your screen refresh rate instead of 30 fps"));

        creator->modules.push_back(new Module("Editor Extension", "editor-extension", "Increases the length of the editor by <cs>17895</c> times :3"));
        creator->modules.push_back(new Module("Toolbox Button Bypass", "toolbox-buttons", "Unlimited <cc>Rows</c> and <cc>Columns</c> in your <cl>Object Toolbox</c>"));

        creator->modules.push_back(new Module("Scale Input", "scale-input-control", "Adds an input field to the object scale control", true));
        creator->modules.push_back(new Module("Reset Percentage On Save", "reset-percentage-on-save", "Resets <cc>percentage</c> on <cl>local levels</c> whenever you save in the editor"));

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
        cosmetic->modules.push_back(new Module("Solid Wave Trail", "solid-wave", "Makes the wave trail a solid Color"));

        cosmetic->modules.push_back(new Module("Transparent BG", "trans-bg", "Disables the Coloring on the gradient backgrounds"));
        cosmetic->modules.push_back(new Module("Transparent Lists", "trans-lists", "Disables the Coloring on all cells"));

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
        cosmetic->modules.push_back(new Module("No Trail Blending", "no-trail-blending", "Disables blending on the normal player trail"));

        cosmetic->modules.push_back(new Module("Percentage Level Font", "percentage-level-font", "Changes the font of the <cc>percentage / time</c> label to be the <cl>level font</c>"));
        cosmetic->modules.push_back(new Module("No Plat Jump Anim", "plat-jump-anim", "Disables the squish animation when jumping in <cc>platformer mode</c>"));

        cosmetic->modules.push_back(new Module("No Lightning", "no-lightning", "Disables the lightning when touching certain portals"));

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
        Client::instance->windows.push_back(new Labels());
    }

    static void SetupOptions(bool android)
    {
        //if (android)
        if (true)
        {
            auto cfg = new Config();

            auto sbox = new Module("Search Box", "ui-search-box", "Adds a search box to the bottom of the ui instead of <cc>version info</c>.");
            sbox->onToggle = [](bool enabled){
                
            };

            cfg->modules.push_back(sbox);
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
    }

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

        theming->modules.push_back(new ColorModule("Accent", "accent-Color", ccc3(10, 0, 0)));

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
            "free-scroll",
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

        Client::GetModule("menu-bg-blur")->setIncompatible("Blur BG is temporarily disabled because i broke it :(.");

        if (auto nodeIDs = Loader::get()->getInstalledMod("geode.node-ids"))
        {
            if (!nodeIDs->isOrWillBeEnabled())
                Client::GetModule("unlock-gauntlets")->setIncompatible("This mod requires the <cc>Node IDs</c> mod to be enabled!");
        }
        else
        {
            Client::GetModule("unlock-gauntlets")->setIncompatible("This mod requires the <cc>Node IDs</c> mod to be installed!");
        }
    }

#pragma endregion
};