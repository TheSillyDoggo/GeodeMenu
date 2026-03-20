#pragma once

#include <Geode/loader/Event.hpp>
#include <Geode/loader/Dispatch.hpp>
#include <Geode/cocos/menu_nodes/CCMenu.h>
#include <ccTypes.h>

#define MY_MOD_ID "thesillydoggo.qolmod"

using CategoryCreateFunc = std::function<void(cocos2d::CCMenu*)>;

namespace qolmod::ext
{
    struct CategoryData
    {
        unsigned int apiVersion = 1;
        std::string displayName = "";
        std::string iconSpr = "";
        std::string categoryID = "";
        CategoryCreateFunc customFunc = nullptr;
    };

    inline void addCustomCategory(CategoryData data)
    GEODE_EVENT_EXPORT_NORES(&addCustomCategory, (data));
};