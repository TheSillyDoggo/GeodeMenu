#pragma once

#include <Geode/loader/EventExport.hpp>
#define MY_MOD_ID "cvolton.level-id-api"

#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/GJLevelList.hpp>

namespace EditorIDs {
    /**
     * @brief Get the editor ID of a level or list.
     *
     * @param level The level to get the ID of.
     *
     * @return The ID of the level, or m_levelID if the level is not editor.
     */
    GEODE_EVENT_EXPORT(int, getID, (GJGameLevel* level), (level));

    /**
     * @brief Get the editor ID of a level or list.
     *
     * @param list The list to get the ID of.
     *
     * @return The ID of the level, or m_levelID if the level is not editor.
     */
    GEODE_EVENT_EXPORT(int, getID, (GJLevelList* list), (list));

    /**
     * @brief Get the editor ID of a level or list.
     *
     * @param level The level to get the ID of.
     * @param autoAssign Whether to assign an ID if it has not been assigned automatically
     *
     * @return The ID of the level, or m_levelID if the level is not editor.
     */
    GEODE_EVENT_EXPORT(int, getID, (GJGameLevel* level, bool autoAssign), (level, autoAssign));

    /**
     * @brief Get the editor ID of a level or list.
     *
     * @param list The list to get the ID of.
     * @param autoAssign Whether to assign an ID if it has not been assigned automatically
     *
     * @return The ID of the level, or m_levelID if the level is not editor.
     */
    GEODE_EVENT_EXPORT(int, getID, (GJLevelList* list, bool autoAssign), (list, autoAssign));

    /**
     * @brief Get a level by its editor ID.
     * 
     * @param id The editor ID of the level.
     *
     * @return The level with the given ID, or nullptr if no level has that ID.
     */
    GEODE_EVENT_EXPORT(GJGameLevel*, getLevelByID, (int id), (id));

    /**
     * @brief Get a list by its editor ID.
     * 
     * @param id The editor ID of the list.
     *
     * @return The list with the given ID, or nullptr if no list has that ID.
     */
    GEODE_EVENT_EXPORT(GJLevelList*, getListByID, (int id), (id));
}

//#undef MY_MOD_ID