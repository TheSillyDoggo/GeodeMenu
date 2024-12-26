#include <Geode/Geode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/GameObject.hpp>
#include <../../../build/bindings/bindings/Geode/modify/PlayLayer.hpp>
#include <../../../build/bindings/bindings/Geode/modify/CCNode.hpp>
#include "../../Client/Client.h"

class $modify (PlayLayer)
{
    struct Fields {
        Module* mod = nullptr;
        Ref<CCNode> customNode = nullptr;
    };

    void addObject(GameObject* p0)
    {
        PlayLayer::addObject(p0);

        if (!m_fields->customNode)
        {
            m_fields->customNode = CCNode::create();
            m_fields->customNode->setID("IMPORTANT"_spr);

            this->m_objectLayer->addChild(m_fields->customNode);
        }

        if (Client::GetModuleEnabled("show-triggers"))
        {
            if (p0->m_objectType == GameObjectType::Modifier || p0->m_objectType == GameObjectType::Special)
            {
                m_fields->customNode->addChild(p0);
                p0->setVisible(true);
            }
        }
    }

    void onQuit()
    {
        if (m_fields->customNode)
            m_fields->customNode->setID("");

        PlayLayer::onQuit();
    }
};

class $modify (CCNode)
{
    static void onModify(auto& self) {
        std::vector<geode::Hook*> hooks;

        auto it = self.m_hooks.begin();
        std::advance(it, 0);

        hooks.push_back(it->second.get());

        Loader::get()->queueInMainThread([hooks] 
        {
            auto modu = Client::GetModule("show-triggers");

            for (auto hook : hooks)
            {
                modu->addHook(hook);
            }
        });
    }

    void removeFromParentAndCleanup(bool p0) // shitty workaround but it is what it is
    {
        if (!PlayLayer::get() || !this->getParent())
            return CCNode::removeFromParentAndCleanup(p0);

        if (this->getParent()->getID() != std::string("IMPORTANT"_spr))
            return CCNode::removeFromParentAndCleanup(p0);
    }
};