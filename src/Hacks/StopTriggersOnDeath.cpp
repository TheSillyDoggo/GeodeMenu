#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GJBaseGameLayer)
{
    virtual void update(float dt)
    {
        if (m_player1 && m_player1->m_isDead)
            return;

        if (m_player2 && m_player2->m_isDead)
            return;

        GJBaseGameLayer::update(dt);
    }

    static void onModify(auto& self)
    {
        auto hook = self.getHook("GJBaseGameLayer::update");
        (void)self.setHookPriority("GJBaseGameLayer::update", 99999999);

        Loader::get()->queueInMainThread([hook]
        {
            auto modu = Client::GetModule("stop-triggers-on-death");
            modu->addHookRaw(hook);
        });
    }
};