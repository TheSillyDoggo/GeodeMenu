#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/CCArray.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (EditorUI)
{
    void onNewCustomItem(cocos2d::CCObject* sender)
    {
        CCArray* objs = m_selectedObjects;

        if (m_selectedObjects->count() == 0)
        {
            objs = CCArray::create();
            objs->addObject(m_selectedObject);
        }

        GameManager::get()->addNewCustomObject(copyObjects(objs, false, false));
        m_selectedObjectIndex = 0;
        reloadCustomItems();
    }

    QOLMOD_MOD_ALL_HOOKS("custom-obj-limit")
};