#include <Geode/Geode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/PlayLayer.hpp>
#include <../../../build/bindings/bindings/Geode/modify/EndLevelLayer.hpp>
#include "../../Client/Client.h"

#ifdef QOLMOD_NOJUSTDONT

using namespace geode::prelude;

class $modify (PlayLayer)
{
    void createObjectsFromSetupFinished()
    {
        PlayLayer::createObjectsFromSetupFinished();

        if (!Client::GetModuleEnabled("just-dont"))
            return;

        Loader::get()->queueInMainThread([this] {
            if (m_attemptLabel)
                m_attemptLabel->setScaleY(1);
        });
    }
};

class $modify (EndLevelLayer)
{
    void customSetup()
    {
        EndLevelLayer::customSetup();

        if (!Client::GetModuleEnabled("just-dont"))
            return;

        Loader::get()->queueInMainThread([this] {
            m_mainLayer->sortAllChildren();

            auto spr = m_mainLayer->getChildByType<CCSprite>(-1);

            if (spr)
                spr->setFlipX(false);
        });
    }
};

#endif