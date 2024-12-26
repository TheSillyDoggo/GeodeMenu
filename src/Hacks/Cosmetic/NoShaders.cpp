#include <Geode/Geode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/ShaderLayer.hpp>
#include <../../../build/bindings/bindings/Geode/modify/GJBaseGameLayer.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

Module* noShad = nullptr;

class $modify (ShaderLayer)
{
    void performCalculations()
    {
        
    }
    
    static void onModify(auto& self) {
        auto hook = self.getHook("ShaderLayer::performCalculations");

        Loader::get()->queueInMainThread([hook]
        {
            auto modu = Client::GetModule("no-shaders");
            modu->addHookRaw(hook);
        });
    }
};