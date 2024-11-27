#pragma once

#include <Geode/Geode.hpp>
#include "ModuleExt.hpp"

using namespace geode::prelude;

namespace QOLModExt
{
    class WindowExt
    {
        private:
            std::string name;
            std::string id;
            int priority;
            std::vector<ModuleExt*> modules;
            std::function<void(CCMenu*, ScrollLayer*)> createPostHook;
        
        public:
            // Please don't use this
            WindowExt(std::string id)
            {
                this->id = id;
                this->name = id;
            }

            std::string getID()
            {
                return id;
            }

            void setName(std::string name)
            {
                this->name = name;
            }

            std::string getName()
            {
                return name;
            }

            void setPriority(int priority)
            {
                this->priority = priority;
            }

            int getPriority()
            {
                return priority;
            }

            void addModule(ModuleExt* module)
            {
                modules.push_back(module);
            }

            std::vector<ModuleExt*> getModules()
            {
                return modules;
            }

            // Runs after the menu is created, for adding your own nodes
            void setPostCreateHook(std::function<void(CCMenu*, ScrollLayer*)> hook)
            {
                this->createPostHook = hook;
            }
    };
};