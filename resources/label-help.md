# Variables

Variables are special values that you can put in the format of your label.

Variables can be used by typing {}, and putting the variable name inbetween the two {}.
like this: "im playing {level_name} by {level_creator}!!!"
which would show as: "im playing Bloodbath by Riot!!!"

# Functions

todo: write

# Info

These are all the variables provided by QOLMod and what they do:

**isLevel** | If you are in the level (not the editor)
**isEditor** | If you are playtesting from the editor

**attempt** | Current Attempt
**fps** | Average Frames Per Second for the last second

**player1_cps** | Clicks Per Second of Player 1
**player2_cps** | Clicks Per Second of Player 2
**total_cps** | Both of these numbers added together

**player1_clicks** | Total clicks of Player 1
**player2_clicks** | Total clicks of Player 2
**total_clicks** | Both of these numbers added together

**noclip_deaths** | How many times you've died in noclip
**noclip_accuracy** | How much time you've spent dead with noclip on

**normal_best** | Your best percentage in normal mode
**practice_best** | Your best percentage in practice mode

**level_name** | The name of the level
**level_creator** | The name of the creator of the level
**level_likes** | The amount of likes for the level
**level_downloads** | The amount of downloads for the level
**level_description** | The description of the level
**level_upload** | The time the level was uploaded
**level_update** | The time the level was last updated
**level_id** | The id of the level
**level_verified** | If the level is verified
**level_object_count** | The amount of objects in the level, does not show values above 65535 because of a bug
**level_version** | The version of the level
**level_game_version** | The version of the game the level was created in

## Level Only (Shows as **null** in editor)

**bestRun_from** | The percentage that your best run was from
**bestRun_to**   | The percentage that your best run was to
**percentage** | Current percentage in the level
**last_percentage** | The last percentage you died at
**run_from** | Where your run started from