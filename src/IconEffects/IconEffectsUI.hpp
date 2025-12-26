#pragma once

#include "../Client/CategoryNode.hpp"

class IconEffectsUI : public CategoryNode
{
    public:
        CREATE_FUNC(IconEffectsUI)

        bool init();
};

SUBMIT_CATEGORY("Icon Effects", IconEffectsUI)
SUBMIT_CATEGORY("Profiles", IconEffectsUI)