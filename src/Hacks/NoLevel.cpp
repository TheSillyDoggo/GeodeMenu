#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/ShaderLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* noLevel = nullptr;

class $modify (ShaderLayer)
{
    virtual bool init()
    {
        noLevel = Client::GetModule("no-level");

        //return ShaderLayer::init();
        return false;
    }

    virtual void visit()
    {
        if (!noLevel->enabled)
        {
            this->setVisible(true);

            //ShaderLayer::visit();
        }
    }
};

class $modify (GJBaseGameLayer)
{
    virtual void update(float p0)
    {
        GJBaseGameLayer::update(p0);

        if (noLevel)
            m_objectLayer->getParent()->setVisible(!noLevel->enabled);
    }
};