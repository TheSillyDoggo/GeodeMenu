#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayLayer)
{
    static void onModify(auto& self) {
        (void)self.setHookPriority("PlayLayer::storeCheckpoint", 9999999);

        std::vector<geode::Hook*> hooks;

        if (self.m_hooks.empty())
            return;

        for (auto hook : self.m_hooks)
        {
            hooks.push_back(hook.second.get());
        }
        
        Loader::get()->queueInMainThread([hooks]
        {
            auto modu = Client::GetModule("no-checkpoint-limit");
            for (auto hook : hooks)
            {
                if (hook)
                {
                    modu->addHook(hook);
                }
            }
        });
    }

    void storeCheckpoint(CheckpointObject* cp)
    {
        m_checkpointArray->addObject(cp);

        PlayLayer::addToSection(cp->m_physicalCheckpointObject);
    }
};
