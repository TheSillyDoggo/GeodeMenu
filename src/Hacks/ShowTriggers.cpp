#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

class $modify (PlayLayer)
{
    CCLayer* mainLayer = nullptr;
    Module* mod = nullptr;

    void addObject(GameObject* p0)
    {
        PlayLayer::addObject(p0);

        if (!m_fields->mod)
            m_fields->mod = Client::GetModule("show-triggers");

        if (m_fields->mod && m_fields->mod->enabled)
        {
            if (!m_fields->mainLayer)
            {
                //this->sortAllChildren();
                if (auto mainNode = getChildOfType<CCNode>(this, 1))
                {
                    if (auto l = getChildOfType<CCLayer>(mainNode, 0))
                    {
                        m_fields->mainLayer = l;
                    }
                }
            }
            
            if (p0->m_objectType == GameObjectType::Modifier || p0->m_objectType == GameObjectType::Special)
                m_fields->mainLayer->addChild(p0);
        }
    }
};

class $modify (GameObject)
{
    virtual void setVisible(bool p0)
    {
        if (m_objectType == GameObjectType::Modifier || m_objectType == GameObjectType::Special)
            GameObject::setVisible(true);
        else
            GameObject::setVisible(p0);
    }
};