#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

using DragDropCallback = std::function<void(std::vector<std::string>)>;

class DragDrop
{
    private:
        std::unordered_map<std::string, DragDropCallback> callbacks;

        DragDrop();
    public:
        static DragDrop* get();

        void addListener(std::string id, DragDropCallback callback);
        void removeListener(std::string id);

        void invoke(std::vector<std::string> paths);
};