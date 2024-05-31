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