#pragma once

#include <Geode/Geode.hpp>

namespace QOLMod
{
    struct CategoryExt
    {
        public:
            std::string name = "";
            std::string modID = "";
            // should preferably be a 64x64 white icon with black outline
            std::string sprite = "";
            int priority = 0;

            void submit();
    };

    /*class AddCategoryEvent : public geode::Event
    {
        public:
            CategoryExt category;

            AddCategoryEvent(CategoryExt category)
            {
                this->category = category;
            }
    };

    inline void CategoryExt::submit()
    {
        auto e = AddCategoryEvent(*this);
        e.post();
    }*/
};