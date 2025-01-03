#include <Geode/Geode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/GJBaseGameLayer.hpp>
#include <../../../build/bindings/bindings/Geode/modify/PlayLayer.hpp>
#include <../../../build/bindings/bindings/Geode/modify/ShaderLayer.hpp>
#include <../../../build/bindings/bindings/Geode/modify/LevelEditorLayer.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

class $modify (ShaderLayer)
{
    virtual void visit()
    {
        
    }

    QOLMOD_MOD_ALL_HOOKS("no-level")
};

class $modify (GJBaseGameLayer)
{
    virtual void update(float p0)
    {
        GJBaseGameLayer::update(p0);

        if (m_objectLayer && m_objectLayer->getParent())
            m_objectLayer->getParent()->setVisible(false);
    }

    QOLMOD_MOD_ALL_HOOKS("no-level")
};