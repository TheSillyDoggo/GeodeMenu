#pragma once

#include <Geode/Geode.hpp>
#include <Button.hpp>

using namespace geode::prelude;

struct DropdownItem
{
    std::string icon = "";
    std::string label = "";
};

class DropdownList : public CCMenu
{
    protected:
        std::unordered_map<qolmod::Button*, DropdownItem> items = {};
        int selectedIndex = 0;
        CCScale9Sprite* bg = nullptr;

    public:
        static DropdownList* create(std::vector<DropdownItem> items);

        void addItem(DropdownItem item);

        void setSelected(DropdownItem item);
        void setSelectedIndex(int index);

        DropdownItem getSelected();
        int getSelectedIndex();

        void updateSelection();

        bool init(std::vector<DropdownItem> items);
};