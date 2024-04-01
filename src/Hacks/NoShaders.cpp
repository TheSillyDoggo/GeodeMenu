#include <Geode/Geode.hpp>
#include <Geode/modify/ShaderLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* noShad = nullptr;

class $modify (ShaderLayer)
{
    void performCalculations()
    {
        if (!noShad)
            noShad = Client::GetModule("no-shaders");
        //log::info("ShaderLayer::performCalculations");

        if (!noShad->enabled)
            ShaderLayer::performCalculations();
    }
};