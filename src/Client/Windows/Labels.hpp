#pragma once

#include "../Client.h"
#include "../../Labels/Labels.h"
#include "../../Labels/LabelModule.hpp"

class Labels : public Window
{
    public:
        Labels()
        {
            name = "Labels";
            id = "labels-window";

            loadConfig();
        }

        std::vector<CCLabelBMFont*> labels = {};
        TextArea* error;

        void cocosCreate(CCMenu* menu);

        void onHide(CCObject*);
        void onToggleLabel(CCObject* sender);
        void onLabelOptions(CCObject* sender);

        void loadConfig();
        void saveConfig();
};