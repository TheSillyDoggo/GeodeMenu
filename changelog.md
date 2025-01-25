# 1.7.9

- Removed Comment Emojis integration to fix lag
- Improved Performance
- Added **Noclip SFX on Death**

# 1.7.8

- Fixed Speedhack being extremely laggy
- Fixed Timewarp Trigger / Speedhack breaking CPS + FPS Labels
- Removed Uncomplete Level cuz it was broken. Will come back in future
- Added New **Path** related functions to Label formatting
- Added Customizable Speedhack Presets
- Added **Comment Emojis Integration to Labels**
- Updated Translations
-- Fixed Korean Translations (thanks @nyaongi)
-- Added Arabic Translations (thanks @adamorocco)
-- Added Right-To-Left language support
-- Fixed translations not being used in some places
-- Fixed some UI elements being misplaced after changing language
-- Fixed some text's incorrectly being in the fallback font

# 1.7.7

- Improved label reordering
- Fixed Crash using Chest Unlock Bypass
- Removed "him" from the credits secret
- Added **Gold Chest Bypass** to Chest Unlock Bypass
- Added **Localization / Translations**!!
-- The language can be changed in the **Config** tab of the mod menu

# 1.7.6

- Fixed level creator appearing as blank in labels for official levels
- Fixed lag (thanks @prevter)
- Added **Label Import / Export to file buttons**

# 1.7.5

- Fixed December Menu Snow not working
- Fixed Unlock Gauntlets not working in some cases
- Fixed rare crash on completing levels
- Added API for mod developers to add tabs to the QOLMod UI

# 1.7.5-beta.6

- **Added Labels back on Android**
-- (i broke it in the last update)

# 1.7.5-beta.5

- **Added Labels back on Android**

# 1.7.5-beta.4

- Fixed crash when closing the mod popup
- **Temporarily disabled labels on android because it caused crashes!!!**

# 1.7.5-beta.3

- Fixed not being able to input decimal places in the transition speed customizer UI
- Fixed crash when pressing the info button on the label format page
- Added support for Geode 4.0.0-beta.1
- **Added support for Intel macOS**

# 1.7.5-beta.2

- Fixed crash closing the mod menu
- Fixed crash when adding labels
- Fixed label toggle box appearing in the wrong position when theres only 1 label
- Minor label UI changes
- Removed editor set scale and rotation

# 1.7.5-beta.1

- Fixed best run not working
- Custom open menu keybinds are not supported in this version
- **Added support for 2.2074**

# 1.7.4

- Fixed UI Button being really hard to hit when speedhack is really high
- Improved Show Touches to show the touch trail and not disappear early
- Fixed Frame Stepper not stepping correctly with tps bypass enabled
- Added arrows to reorder labels
- Added toggles to hide labels
- Added opacity slider to label events
- Added run info to labels
- Fixed Unlock Buttons ungreyscaling some wrong buttons
- Temporarily disabled blur bg because i broke it :broken_heart:
- Added **Editor Reset Percentage on Save**
- Added **Hitbox Multiplier**
- Added **Freeze Attempts**
- Added **Custom Transition Time**
- Added **Instant Mirror Portal**
- Added **Legacy Upside Down Physics**
- Added **Icon Kit in Pause Menu**
- Added **Unlock Gauntlets**
- Added **End Screen Cheat Indicator**

# 1.7.3

- Added new variables to labels
- Wrote docs for labels that can be viewed with the (i) button the format page

# 1.7.2

- Fixed memory leak while playing a level

# 1.7.1

- Fixed not being able to type decimals in label inputs
- Fixed opacity being saved as the scale
- Fixed crash when searching in the mod menu
- Fixed Duplicate Labels when loading v1.7.0 for the first time
- Fixed icons with the id 16 and 17 being unlocked with practice unlocker hack enabled
- Fixed Startpos Switcher starting at 0 (isn't fixed on macOS because of crashes)
- Removed Speedhack Transition Fix because it was very buggy
- Made Speedhack Gameplay actually be gameplay only

# 1.7.0

- Added incompatibility with [Text Select](https://github.com/camila314/text-select/) on android
- Made Icon Effects change colour of objects that use player colour in the level
- Made Noclip Accuracy not increase once the level ends
- Frame Stepper no longer saves ticks if you are dead
- Frame Stepper buttons now repeat presses when being held down
- Fixed Safe Mode not setting the label to "Safe Mode :3" if the level has coins
- Added New Geode Themes to the menu
- Fixed crash in editor with betteredit enabled
- Added **Blur Menu BG** on macOS
- Added **No Particles** on macOS
- Added **Transition Customizer** on macOS
- Added **Solid Wave Trail** and **No Wave Trail** on macOS
- Added **Hide Button in Editor**
- Added **Safe Mode Info Button**
- Added **Pause Buffering**
- Added **No Platformer Jump Animation**
- Added **Force Show Player**
- Added **Force Hide Player**
- Added **No Lightning**
- Added **Chest Unlock Bypass**
- Added **Noclip Minimum Accuracy**
- Added **Noclip Per Player**
- Rewrote **Show Layout** to be better, allowing it to be ported to Windows and macOS

# 1.6.10

- Fixed UI Button speed fading way too fast
- Ported **No Camera Shake** To Windows and macOS
- Ported **Pulsing Menu** To Windows and macOS

# 1.6.9

- Fixed lag in game caused by custom fonts
- Rewrote Copy Hack to work better
- Fixed Monster Show Hitbox fix being broken with scaling
- Fixed Event Trigger not detecting player 2 jump
- Added **Custom Icon Effect Speed**

# 1.6.8

- Fixed checking for updates text being positioned wrong when the Loading Circle is visible
- Fixed freeze when opening the font picker menu for the first time
- Fixed Noclip Accuracy being wrong a lot of the time
- Fixed Show Trajectory unnecessarily running a lot more times than necessary
- Fixed Click Sounds dying with Show Trajectory enabled
- Fixed Show Hitboxes not showing the hitbox of 2 monster objects (you know, those guys..)
- Made Update Available label clickable if an update is available, clicking it opens the mod popup to update the mod
- Added **Frame Stepper**

# 1.6.7

- Fixed Speedhack not working with physics bypass enabled in cbf
- Removed the search bar replacing it with version info as it was in older versions
- If you want to bring it back you can in the <cc>Config</c> tab
- Added **Toolbox Button Bypass**
- Added **Scale Text Input**
- Added **Free Scroll**
- Added **No Trail Blending**
- Added **Percentage Level Font**
- Added **Label Font Customizer**
- Added **Treasure Room Bypass**
- Added **Vault of Secrets Bypass**
- Added **The Challenge Bypass**
- Added **Basement Bypass**
- Added **Basement Key Bypass**

# 1.6.6

- Changed Safe Mode to show the endscreen instead of kicking out of the level
- Added **Editor Extension**

# 1.6.5

- Rewrote old bad code to help improve code readibility
- Fixed CPS Counter not being reset to white when restarting
- Fixed blur background looking weird when a blending layer is visible on screen (like loading circles)
- Ported **Physics Bypass** to Windows
- Ported **Custom Object Limit Bypass** to All Platforms
- Added **Stop Triggers on Death**

# 1.6.4

- Removed Percentage Label because it caused the game to crash whenever loading a level on some devices (for some reason idk)

# 1.6.3

- Fixed Custom Message Label text being cut off to 12 characters
- Fixed Auto Clicker not being considered a cheat by auto safe mode
- Added **Percentage Label**
- Ported **Show Trajectory** to 2.206
- This is a COMPLETE REWRITE of Show Trajectory, Fixing many issues such as portals activating early and the trail disappearing sometimes

# 1.6.2

- Fixed infamous all modes platformer crash
- Fixed Hitbox Trail saving the trail when show hitboxes is disabled
- Fixed incompatibility with XDBot rewrite
- Lowered Max Hitbox Trail count to 200 instead of 500 for performance reasons
- Added **Decimals to Best Run**
- Added **Hide From % from 0 to Best Run**

# 1.6.1

- Fixed game loading slowly if speedhack was turned to really low (<0.01)
- Fixed Editor Wave Trail changing the position while not the wave
- Transparent BG Now updates live when you change it
- Added **Hide Pause Menu**
- Added **Auto Clicker**

# 1.6.0

- Fixed Transparent BG not working on apple devices
- Fixed the button not appearing in a new scene on apple devices
- Fixed Coin Collect Circle effect being the wrong colour with Gold User Coins
- Added **Fake Ball Rotation Bug**
- Added **Mod Keybinds**

# 1.5.9

- Added **Gold User Coins**
- Added **No Ship Fire**
- Added **24 Hour Time** for clock label

# 1.5.8

- Fixed not being able to use checkpoints in platformer with all modes platformer enabled
- Fixed crash when playtesting with **Force Ghost On** enabled
- Coins are now collected if you complete a level with 1 attempt practice complete

# 1.5.7

- Fixed Mouse Cursor not disappearing when closing the menu while in a level
- Fixed not being able to move in platformer mode on arm / m1 macOS
- Added **Force Ghost Trail On and Off**

# 1.5.6

- Fixed Force Platformer not showing platformer UI on mobile
- Fixed All Modes Platformer causing crash playtesting
- Fixed Crash With All Modes Platformer
- Fixed Crash when tapping really early on the loading screen with Show Touches enabled
- Made Pause Countdown have a minimum countdown of 1 second
- Added The Pause Button to the Pause Countdown menu to repause the game on mobile
- Added **Clock Label**

# 1.5.5

- Fixed Crashing almost all the time on macOS / iOS
- Fixed mouse cursor not appearing when opening up the mod menu while in a level on macOS

# 1.5.4

- Fixed Dropdown's not being clickable
- Fixed not being able to search for module options
- Added **Customizable Noclip Tint Colour**
- Added **CPS Counter instant colour fade**
- Added **Customizable Pause Countdown Time**
- **Added [EXPERIMENTAL] Intel macOS support**

# 1.5.3

- Fixed Crash pressing Q or E in the editor with startpos switcher enabled
- Fixed the Button disappearing when switching scenes
- Fixed Noclip not working in the editor
- Changed the Button to use textures instead of labels
- Changed Windows to export function symbols for easier crash fixing
- Added **Labels In Editor**
- **Added [EXPERIMENTAL] ARM / M1 macOS support**

# 1.5.2

- Fixed Crash removing checkpoints with checkpoint limit bypass enabled (thanks @hiimjustin000)
- Actually fixed not being able to exit practice mode with Comfirm Practice enabled
- Fixed not being able to scroll in the qolmod popup
- Fixed dying in noclip breaking the cps counter
- FINALLY fixed the touch issues with popups
- Added **No Dash Fire**

# 1.5.1

- Fixed Globed already loaded level bug
- Fixed not being able to exit practice mode with Comfirm Practice enabled
- Fixed not being able to open a level in the editor
- Added **Editor Wave Trail**
- Added **Smooth Editor Trail**
- Added **No Checkpoint Limit**

# 1.5.0

- Fixed Show Hitboxes Colours not loading correctly
- Fixed Transparent Lists not applying to Leaderboards
- Fixed Shaders breaking Noclip Tint On Death
- Fixed Unlock Buttons applying on list pages
- Fixed Confirm Practice Mode not working for exiting practice mode
- Fixed the FPS label text appearing as (a body part on girls that i dont think hjfod will let me say on new index) for the first few seconds
- Fixed No Glow not working
- Ported **All Modes Platformer** on Windows
- Ported **Random Seed**
- Added **No Robot Fire**
- Added **Best In Percentage**
- Added **Auto LDM**
- Added **Auto Collect Coins**
- Added **No Spider Dash Effect**
- Added **Longer Trail**
- Added **Suicide**
- Added **Best Run Label**

# 1.4.9

- Fixed CPS and FPS counter being broken
- Fixed dying in noclip causing fps to be wrong
- Fixed Transparent Lists not applying to Map Packs
- Added **One Attempt Practice Complete**
- Added **Coins In Practice**

# 1.4.8

- Removed all imgui code so that wine players can use qolmod
- Fixed multiple CPS Counter bugs
- Added Total CPS option to CPS Counter
- Changed how FPS is counted, should be more accurate
- Removed F12 keybind by default
- Fixed more touch issues
- Renamed TPS Bypass to Physics Bypass
- Added **Show Touches**

# 1.4.7

- Fixed the UI Button disappearing if you have Transition Customizer enabled
- Lowered the Hitbox Trail limit to hopefully fix lag
- Slider Limit Bypass now works on the scale slider in the editor
- Added **Hide Pause Button**
- Added **Pause Countdown**

# 1.4.6

- Fixed Touch Issues (now uses geode::Popup<>)
- Fixed Startpos switcher not switching using keybinds if you have the mod **Custom Keybinds** installed
- Noclip no longer triggers safe mode if you haven't died this attempt
- Quests in Pause is now enabled by default
- Fixed Noclip Accuracy being VERY wrong
- Fixed Instant Respawn killing the player twice
- Added **No Short Numbers**
- Added **Respawn Time Customizer**
- Added **Smart Startpos**

# 1.4.5

- Fixed Separate Dual Icons and Pride Trails not working
- Fixed Crash pressing physical keyboard keys on Android
- Fixed Startpos Switcher not sorting startpos's by X position
- Fixed Speedhack Triggering Safe Mode even if it is above 1.0
- Fixed Hitbox Trail not working
- Fixed Incompatible Modules not being able to be disabled
- Lowered Default Position of Startpos Switcher UI
- Added Moveable Startpos Switcher UI
- Rewrote Safe Mode system
- Improved Performance by making Force Platformer only apply on level open
- Added Main Menu Gameplay

# 1.4.4

- Fixed a game softlock when opening the RecordKeyPopup on mobile
- Fixed the gradient theme not working
- Fixed not being able to click in the shop
- Fixed CPS Label not working on Windows
- Added Main Level Song Bypass (thanks @Capeling)
- **Added Startpos Switcher**

# 1.4.4-beta.5

- Fixed pressing presets in speedhack section not saving the value
- Fixed the mod not loading on the latest geode beta

# 1.4.4-beta.4

- Fixed pressing the mod settings button crashing the game
- Fixed the mod not loading on the latest geode alpha

# 1.4.4-beta.3

- Fixed the mod not loading on the latest geode alpha

# 1.4.4-beta.2

- Fixed tags not showing on the mod index page
- Fixed **No Respawn Blink** not working on the second player
- Fixed being able to click invisible inputs
- Added Show Layout on android
- Added Pulsing Menu on android

# 1.4.4-beta.1

- Fixed Trail And Wave Trail having the wrong default colour in icon effects
- Cheat indicator now properly works if Auto Safe Mode is disabled
- Fixed Transitions being the wrong speed on macos
- Added Menu Keybind customizer in the mod settings
- Adjusted Pastel Colour code to be more optimised
- Removed hide endscreen button as it was added in 2.206
- Fixed Conditional Auto Retry triggering in practice or platformer mode
- Changed mod id from **TheSillyDoggo.Cheats** to **thesillydoggo.qolmod** to comply with new index
- **Added support for 2.206**

# 1.4.3

- Fixed Bass being boosted on some devices
- Added Trail and Wave Trail options to icon effects
- Added option to customize the colours used in the fade icon effect mode
- Added No Orb Pulse
- Fixed Hide Endscreen putting the button at the bottom of the end screen
- Fixed Options Button being in the wrong Place with Full Options Menu enabled
- Icon Effects automatically disable if you have incompatible mods enabled

# 1.4.2

- Added Individual X/Y scale mode in Creator Set Scale
- Performance Improvements
- Added Show Level Password
- Added Pitch Shifter
- Fixed Being unable to scroll in some layers

# 1.4.1

- Added new 'Darken' theme
- Added Blur Mod Menu BG (May cause lag on devices with weak gpus)
- Added No Wave Trail Remove
- Added Editor Set Scale and Set Rotation functions
- Removed the [?] button from the universal tab
- Moved Transition Customizer to its own module
- Misc UI Related Bug Fixes / Changes

# 1.4.0

- Added Conditional Auto-Retry
- Added Auto Practice Mode
- Added Unlock Buttons
- Added Disable in-game
- Redesigned the button tab in the config menu, this also resulted in Hide Button being moved to the menu tab
- Show Layout shows Hidden objects
- Added an option to customize show layout bg and ground colours
- Fixed Show Layout making the ground's white
- Show Layout now disables pulse triggers
- Fixed Show Triggers making the triggers disappear once they've been on screen
- Fixed a bug that makes cataclysm and a few other levels impossible
- Fixed Incompatibility with better pause and being able to click through some popups
- Fixed Gradient Appearing white

# 1.3.9

- Fixed Crash Playing Levels a second time

# 1.3.8

- Fixed crash on MacOS
- Removed Pulse Scene because of unnecessary lag
- Fixed Vertical sliders with slider limit bypass
- Fixed timewarp messing with the fps counter
- Added Clicks Per Second Counter
- Added No **"Do Not"** Flip
- Mod is now build with a pdb, which basically means crashlogs will show what line in qolmod crashed

# 1.3.7

- Label Corners now save the corner
- Fixed Crash Playing Levels on Windows
- Fixed Attempt Count saying nullptr every attempt
- Increased the amount of numbers you can input in tps bypass to 11
- FPS Counter now only updates every half of a second instead of every frame
- Added No Transition and Verify Hack On MacOS

# 1.3.6

- Added Random Seed
- Improved Performance of a lot of stuff
- Added Mod Descriptions to Settings Popups
- Added ability to move labels to different corners
- Added TPS Bypass
- Added Main Level Bypass
- Added Tower Level Bypass
- Added Slider Limit Bypass
- Added **[<cl>EXPERIMENTAL</c>]** Mac Support

# 1.3.5

- Show Trajectory shows where you will die
- Readded Show Layout
- Fixed another crash opening mod menu
- Optimised the on screen button
- Made the mod menu popup scrollable
- Fixed Spelling mistake in confirm restart
- Added All Modes Platformer
- Added Comment History Bypass
- Added Game Time label
- Temporarily removed replay because of bugs
- Redesigned Config Menu
- Added Search Box

# 1.3.4

- Added customizable message label
- Temporarily removed show layout because of editor crashes
- Added Show Trajectory (Beta)
- Added Instant Fade (Makes the mod menu button instantly disappear instead of slowly fading)
- Safe Mode Kicks you out of the level now instead of showing the end screen, it was really broken
- Removed Debug Button from Speedhack
- Added Speedhack Presets
- Added Speedhack transition fixes

# 1.3.3

- Fixed windows not being able to open the mod menu, sorry
- Added No Wave Pulse
- Added Jump Hack

# 1.3.2

- Fixed startup crash

# 1.3.1

- Fixed bug where fixed hitboxes were shown in the editor even is show hitboxes was disabled
- Fixed crash playing user levels

# 1.3.0

- Added Coin Finder
- Fixed crash opening mod menu
- Added Fixed Hitboxes for players
- Added Accurate Percentage
- Added Classic Percentage (before 2.2)
- Added Show Layout
- Added Attempts Status Text

# 1.2.9

- Added hitbox colour customizer
- Fixed hitboxes in mirror portal mode
- Added no particles on android
- Added Kill at % (also supports time for platformer)
- Added Noclip tint on death

# 1.2.8

- Fixed practice hitboxes being broken
- Added Replay status text
- Readded thicker hitboxes
- Added fill hitboxes
- Made hitbox trail work for 2 player
- Show Hitboxes is now considered a cheat

# 1.2.7

- Added Hitbox Trail
- Added Quests in Pause

# 1.2.6

- Added Show Triggers in normal mode
- Fixed editor crash

# 1.2.5

- Added Pulsing menu and pulsing scene
- Added Creator category for creator hacks
- Fixed descriptions being openable multiple times
- Added Custom Object Limit Bypass (pc only for now)
- Added Free Scroll

# 1.2.4

- Added Noclip Deaths and Noclip Accuracy

# 1.2.3

- Fixed transparent bg causing textureldr to crash
- Fixed Typo
- Added status texts

# 1.2.2

- Fixed Icon Effects
- made alpha happy ^w^
- and some other stuff i dont want to write down

# 1.2.1

- Fixed Speedhack
- Added Verify Hack
- Improved Safe Mode

# 1.2.0

- Added auto song download
- Fixed Crash Opening Mod menu
- Fixed Instant Restart being broken on android 32
- Added No reverse portal
- Fixed input nodes
- Added full options menu
- Added hide player
- Added no respawn blink
- Added All modes platformer (windows only)
- Added level uncomplete
- Added show hitboxes and show hitboxes on death
- Added no static camera
- Added scale button slider
- Removed fps bypass since it was VERY broken, replaces with full options menu
- Fixed Spider and Robot with icon effects
- Added <cl>Gradient Pages</c> integration
- Added TAB + Insert hotkeys to open the mod menu
- Added Replay Bot (beta)

# 1.1.4

- Added additional borders for npesta texture pack users
- Fixed RobTop levels with the level edit fix
- Added Force Object Visibility
- Added No Glow
- ~~Added Noclip Deaths and Noclip Accuracy~~ it's delayed because this shits annoying to fix
- Added Alerts to the text bypasses to let the user know that they can crash
- Added Instant Restart
- Added Transition Customizer
- Fixed Solid Wave Trail white being broken
- Temporarily removed thicker hitboxes due to lag :(

# 1.1.3

- Fixed Level Edit breaking level page, for anyone who used this before the update: Press the ? button in the levels tab while in the broken level. Do this fast as the button WILL BE REMOVED IN THE FUTURE
- Added Force Trail On and Force Trail Off
- Added No Camera Shake
- Added Force Platformer On Android
- Moved Confirm Practice and Confirm Restart From Universal to Level
- Added Menu Animations, From Left, From Right, From Top, From Bottom and Scale
- Added FPS Bypass (Beta)
- Added Noclip broken slope fix
- Added ~~Herobrine~~ Zulguroth

# 1.1.2

- Added Level Edit
- Removed boob dropdown (sad day)
- Fixed lag with Transparent BG

# 1.1.1

- Added hide endscreen
- Fixed button not being clickable on the pause menu
- Re enable text bypasses
- Fix incorrect menu speed during transition
- Re-added the text bypasses due to hopefully fixing the crashes
- Added Transparent BG & Transparent Lists
- Added Confirm Practice and Confirm Restart
- Fixed show hitboxes breaking (vanilla one)
- Added thicker hitboxes

# 1.1.0

- Added Icon Effects (RGB Icons)
- Added no Wave Trail
- Removed the text limit bypasses due to crashes, sorry :/
- Created an official [Discord Server](https://discord.gg/DfQSTEnQKK)

# 1.0.3

- Added back the button wheh in gameplay
- Fixed pause crash with speedhack

# 1.0.2

- Fixed crash when playing user levels
- Added character filter and character limit bypass
- ~~Fixed balls duplicating~~ nvm i broke it :(
- Pause menu is still broken, if you can't click any buttons press esc or the back button on your phone (the arrow)
- Added No Particles, No Shaders, Text Limit Bypass, Text Character Bypass and Speedhack Music

# 1.0.1

- Fixed force priority issues in newest geode build
- Fixed absolllutes credits button being bigger on medium and low graphics
- Added trash button next to speedhack input
- Fixed crashes on some phones when changing menus

# 1.0.0

- Initial Release