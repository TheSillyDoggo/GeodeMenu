#pragma once
#ifndef QOLMODEXT_HPP
#define QOLMODEXT_HPP

#include <Geode/Geode.hpp>
#include "WindowExt.hpp"

using namespace geode::prelude;

namespace QOLModExt
{
    class PushWindowEvent : public Event
    {
        public:
            WindowExt* window;

            PushWindowEvent(WindowExt* window)
            {
                this->window = window;
            }
    };

    class ToggleModuleEvent : public Event
    {
        public:
            std::string id;
            bool enabled;

            ToggleModuleEvent(std::string id, bool enabled)
            {
                this->id = id;
                this->enabled = enabled;
            }
    };

    class CommentEmojisOverrideLabelCreatePrivateEventPleaseDoNotUseOrIWillTouchYou : public Event
    {
        public:
            std::function<CCLabelBMFont*()> function;

            CommentEmojisOverrideLabelCreatePrivateEventPleaseDoNotUseOrIWillTouchYou(std::function<CCLabelBMFont*()> function)
            {
                this->function = function;
            }
    };

    /// @brief Creates a window and returns it
    /// @param id An id for the window, this is not shown to the user and is currently not used internally. This may change, please do not change the ID
    /// @return WindowExt*
    inline WindowExt* createWindow(std::string id)
    {
        auto window = new WindowExt(id);

        return window;
    }

    inline ModuleExt* createModule(std::string id)
    {
        auto mod = new ModuleExt(id);

        return mod;
    }

    /// @brief Adds a window and its modules to the UI.
    /// @param window 
    inline void pushWindow(WindowExt* window)
    {
        auto e = PushWindowEvent(window);
        e.post();
    }

    inline void setModuleEnabled(std::string moduleID, bool enabled)
    {
        auto e = ToggleModuleEvent(moduleID, enabled);
        e.post();
    }
};

#endif