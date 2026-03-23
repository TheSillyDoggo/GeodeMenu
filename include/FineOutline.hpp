#pragma once

#include <Geode/loader/Event.hpp>
#include <Geode/loader/Dispatch.hpp>
#include <Geode/binding/PlayerObject.hpp>
#include <Geode/binding/SimplePlayer.hpp>
#include <ccTypes.h>

namespace alpha::fine_outline {
    enum class PlayerIcon {
        SELECTED = -1,
        ONE,
        TWO
    };
}

#define MY_MOD_ID "alphalaneous.fine_outline"

namespace alpha::fine_outline {

    namespace internal {
        inline void _setOutlineColorPlayerObject(PlayerObject* player, const cocos2d::ccColor3B& color)
        GEODE_EVENT_EXPORT_NORES(&_setOutlineColorPlayerObject, (player, color));

        inline cocos2d::ccColor3B _getOutlineColorPlayerObject(PlayerObject* player)
        GEODE_EVENT_EXPORT_NORES(&_getOutlineColorPlayerObject, (player));

        inline void _updateOutlinePlayerObject(PlayerObject* player)
        GEODE_EVENT_EXPORT_NORES(&_updateOutlinePlayerObject, (player));

        inline void _setOutlineColorSimplePlayer(SimplePlayer* player, const cocos2d::ccColor3B& color)
        GEODE_EVENT_EXPORT_NORES(&_setOutlineColorSimplePlayer, (player, color));
        
        inline cocos2d::ccColor3B _getOutlineColorSimplePlayer(SimplePlayer* player)
        GEODE_EVENT_EXPORT_NORES(&_getOutlineColorSimplePlayer, (player));

        inline void _updateOutlineSimplePlayer(SimplePlayer* player)
        GEODE_EVENT_EXPORT_NORES(&_updateOutlineSimplePlayer, (player));
    }

    template<class T>
    concept PlayerType =
        std::derived_from<T, PlayerObject> ||
        std::derived_from<T, SimplePlayer>;

    /// Set the outline color of a PlayerObject or SimplePlayer
    template<PlayerType T>
    inline void setOutlineColor(T* player, const cocos2d::ccColor3B& color) {
        if constexpr (std::is_base_of_v<PlayerObject, T>) {
            internal::_setOutlineColorPlayerObject(player, color);
        }
        else if constexpr (std::is_base_of_v<SimplePlayer, T>) {
            internal::_setOutlineColorSimplePlayer(player, color);
        }
    }

    /// Get the outline color of a PlayerObject or SimplePlayer
    template<PlayerType T>
    inline cocos2d::ccColor3B getOutlineColor(T* player) {
        if constexpr (std::is_base_of_v<PlayerObject, T>) {
            return internal::_getOutlineColorPlayerObject(player);
        }
        else if constexpr (std::is_base_of_v<SimplePlayer, T>) {
            return internal::_getOutlineColorSimplePlayer(player);
        }
    }

    /// Update the outline color of a PlayerObject or SimplePlayer
    /// This is useful if something else changed it for some reason
    template<PlayerType T>
    inline void updateOutline(T* player) {
        if constexpr (std::is_base_of_v<PlayerObject, T>) {
            internal::_updateOutlinePlayerObject(player);
        }
        else if constexpr (std::is_base_of_v<SimplePlayer, T>) {
            internal::_updateOutlineSimplePlayer(player);
        }
    }

    /// Check if the 2 player icons are separate with the "Separate Dual Icons" mod
    inline bool isSeparate()
    GEODE_EVENT_EXPORT_NORES(&isSeparate, ());

    /// Check if the second player is being edited in the Garage with the "Separate Dual Icons" mod
    inline bool isEditingP2()
    GEODE_EVENT_EXPORT_NORES(&isEditingP2, ());

    /// Returns true if the player icon type is using a custom color rather than a color index
    /// By default, it gets the selected icon's color if Separate dual icons is installed
    inline bool usesCustomColor(alpha::fine_outline::PlayerIcon player = alpha::fine_outline::PlayerIcon::SELECTED)
    GEODE_EVENT_EXPORT_NORES(&usesCustomColor, (player));

    /// Set if the player icon type is using a custom color rather than a color index
    inline void useCustomColor(alpha::fine_outline::PlayerIcon player, bool use)
    GEODE_EVENT_EXPORT_NORES(&useCustomColor, (player, use));

    /// Returns the custom color set for a player icon type
    /// By default, it gets the selected icon's color if Separate dual icons is installed
    inline cocos2d::ccColor3B getCustomColor(alpha::fine_outline::PlayerIcon player = alpha::fine_outline::PlayerIcon::SELECTED)
    GEODE_EVENT_EXPORT_NORES(&getCustomColor, (player));

    /// Sets the custom color for a player icon type
    inline void setCustomColor(alpha::fine_outline::PlayerIcon player, const cocos2d::ccColor3B& color)
    GEODE_EVENT_EXPORT_NORES(&setCustomColor, (player, color));

    /// Gets the color index for a player icon type
    /// By default, it gets the selected icon's color if Separate dual icons is installed
    inline int getColorIndex(alpha::fine_outline::PlayerIcon player = alpha::fine_outline::PlayerIcon::SELECTED)
    GEODE_EVENT_EXPORT_NORES(&getColorIndex, (player));

    /// Sets the color index for a player icon type
    inline void setColorIndex(alpha::fine_outline::PlayerIcon player, int color)
    GEODE_EVENT_EXPORT_NORES(&setColorIndex, (player, color));

    /// Gets the color for a player icon type (returns the custom one if custom, else returns the color for color index)
    /// By default, it gets the selected icon's color if Separate dual icons is installed
    inline cocos2d::ccColor3B getColor(alpha::fine_outline::PlayerIcon player = alpha::fine_outline::PlayerIcon::SELECTED)
    GEODE_EVENT_EXPORT_NORES(&getColor, (player));
}