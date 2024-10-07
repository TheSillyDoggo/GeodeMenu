#pragma once

#include <Geode/Geode.hpp>
#include "BestRun.hpp"
#include "../Client/Client.h"
#include <rift.hpp>
#include "LabelModule.hpp"

using namespace geode::prelude;

class LabelNode : public CCLabelBMFont
{
    public:
        LabelModule* mod;
        rift::Script* script;

        bool init(LabelModule* mod);

        virtual void update(float dt);

        std::string getFormatString();

        static LabelNode* create(LabelModule* mod);
};