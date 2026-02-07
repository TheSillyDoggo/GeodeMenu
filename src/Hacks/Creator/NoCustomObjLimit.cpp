#include "../../Client/Module.hpp"
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class CustomObjLimit : public Module
{
    public:
        MODULE_SETUP(CustomObjLimit)
        {
            setID("custom-obj-limit");
            setCategory("Creator");
        }
};

SUBMIT_HACK(CustomObjLimit);

class $modify (EditorUI)
{
    void onNewCustomItem(cocos2d::CCObject* sender)
    {
        if (!CustomObjLimit::get()->getRealEnabled())
            return EditorUI::onNewCustomItem(sender);

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
};