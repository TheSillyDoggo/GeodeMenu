#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/CCNode.hpp>
#include "../Client/Client.h"

class $modify (PlayLayer)
{
    Module* mod = nullptr;
    Ref<CCNode> customNode;

    void addObject(GameObject* p0)
    {
        PlayLayer::addObject(p0);

        if (!m_fields->customNode)
        {
            m_fields->customNode = CCNode::create();
            m_fields->customNode->setID("IMPORTANT"_spr);

            this->m_objectLayer->addChild(m_fields->customNode);
        }

        if (!m_fields->mod)
            m_fields->mod = Client::GetModule("show-triggers");

        if (m_fields->mod && m_fields->mod->enabled)
        {            
            if (p0->m_objectType == GameObjectType::Modifier || p0->m_objectType == GameObjectType::Special)
                m_fields->customNode->addChild(p0);
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
                if (hook)
                {
                    hook->setAutoEnable(false);

                    if (!modu->enabled)
                        hook->disable();

                    modu->hooks.push_back(hook);
                }
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