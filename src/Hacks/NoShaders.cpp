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
        std::vector<geode::Hook*> hooks;

        hooks.push_back(self.getHook("ShaderLayer::performCalculations").unwrap());

        Loader::get()->queueInMainThread([hooks] 
        {
            auto modu = Client::GetModule("no-shaders");

            for (auto hook : hooks)
            {
                hook->setAutoEnable(false);

                if (!modu->enabled)
                    hook->disable();

                modu->hooks.push_back(hook);
            }
        });
    }
};