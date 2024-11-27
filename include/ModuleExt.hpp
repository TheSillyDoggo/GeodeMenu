#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace QOLModExt
{
    enum class ModuleExtType
    {
        Boolean,
    };

    class ModuleExt
    {
        private:
            std::string name;
            std::string id;
            std::string description;

            bool canBeToggled = true;
            std::string notTogglableWarning;

            bool enabledByDefault;
            bool enabled;

            std::function<void(bool)> onToggle;

            ModuleExtType type = ModuleExtType::Boolean;

        public:
            ModuleExt(std::string id)
            {
                this->id = id;
                name = id;
                description = "This is the help text";
            }

            void setName(std::string name)
            {
                this->name = name;
            }

            std::string getName()
            {
                return name;
            }

            std::string getID()
            {
                return id;
            }

            void setDescription(std::string description)
            {
                this->description = description;
            }

            std::string getDescription()
            {
                return description;
            }

            void setEnabledByDefault(bool enabled)
            {
                this->enabledByDefault = enabled;
            }

            bool getEnabledByDefault()
            {
                return enabledByDefault;
            }

            void setCanBeToggled(bool can, std::string message)
            {
                this->canBeToggled = can;
                this->notTogglableWarning = message;
            }

            bool getCanBeToggled()
            {
                return canBeToggled;
            }

            std::string getCantBeToggledWarning()
            {
                return notTogglableWarning;
            }

            void setOnToggle(std::function<void(bool)> onToggle)
            {
                this->onToggle = onToggle;
            }

            std::function<void(bool)> getOnToggle()
            {
                return onToggle;
            }

            void setType(ModuleExtType type)
            {
                this->type = type;
            }

            ModuleExtType getType()
            {
                return type;
            }

            void setEnabled(bool enabled)
            {
                this->enabled = enabled;
            }

            bool getEnabled()
            {
                return enabled;
            }
    };
};