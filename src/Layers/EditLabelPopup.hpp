#pragma once

#include <Geode/Geode.hpp>
#include "../Client/Module.h"
#include "../Hacks/SafeMode/SafeMode.hpp"

#include "SillyBaseLayer.h"

using namespace geode::prelude;

class EditLabelPopup : public SillyBaseLayer
{
    public:
        LabelModule* module;
        bool advanced;
        int currentPage = 0;
        std::vector<CCNode*> pages;

        virtual void customSetup();

        void updatePage();

        void onPage(CCObject* sender);
        void onClose(CCObject* sender);

        static EditLabelPopup* create(LabelModule* module, bool advanced);
        static EditLabelPopup* addToScene(LabelModule* module, bool advanced);
};