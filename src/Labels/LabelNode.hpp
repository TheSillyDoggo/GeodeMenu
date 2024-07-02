#pragma once

#include <Geode/Geode.hpp>
#include "../Client/Client.h"
#include "LabelModule.hpp"
#include <rift.hpp>

using namespace geode::prelude;

class LabelNode : public CCLabelBMFont
{
    public:
        LabelModule* module;
        rift::Script* script;        

        static LabelNode* create(LabelModule* module);

        bool init(LabelModule* module);

        virtual void update(float delta);
};