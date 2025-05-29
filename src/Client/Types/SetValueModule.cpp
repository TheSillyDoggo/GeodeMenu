#include "SetValueModule.hpp"

void SetValueModule::makeAndroid(CCNode* menu, CCPoint pos)
{
    return;

    auto label = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setScale(0.575f);
    label->setPosition(pos + ccp(-10, 0));

    inp = TextInput::create(115, "Value");
    inp->setPosition(pos + ccp(-20, 0) + ccp(230, 0));
    inp->setFilter(allowedChars);
    inp->setScale(0.8f);
    inp->getInputNode()->setID("IGNOREBYPASSES"_spr);

    auto setSpr = ButtonSprite::create("Set", "bigFont.fnt", "GJ_button_05.png");
    setSpr->setScale(0.65f);

    auto setBtn = CCMenuItemSpriteExtra::create(setSpr, menu, menu_selector(SetValueModule::onSet));
    setBtn->setUserData(this);
    setBtn->setPosition(pos + ccp(-20, 0) + ccp(308, 0));

    if (id == std::string("set-scale"))
    {
        CCSprite* ok = as<CCSprite*>(as<CCNode*>(CCLabelBMFont::create(".", "bigFont.fnt")));
        ok->setAnchorPoint(ccp(0.43f, 0.2f));
        
        auto btn = CCMenuItemToggler::create(ButtonSprite::create(reinterpret_cast<CCSprite*>(CCLabelBMFont::create("XY", "bigFont.fnt")), 30, 69, 30, 1, false, "GJ_button_05.png", true), ButtonSprite::create(ok, 30, 69, 30, 1, false, "GJ_button_05.png", true), menu, menu_selector(SetValueModule::onScaleToggle));
        btn->setPosition(pos + ccp(145, 0));
        btn->setScale(0.55f);
        btn->setUserData(this);
        ok->setScale(1.5f);

        inpX = TextInput::create(50, "X");
        inpX->setPosition(pos + ccp(-20, 0) + ccp(204, 0));
        inpX->setFilter(allowedChars);
        inpX->setScale(0.8f);
        inpX->setVisible(false);
        inpX->getInputNode()->setID("IGNOREBYPASSES"_spr);

        inpY = TextInput::create(50, "Y");
        inpY->setPosition(pos + ccp(-20, 0) + ccp(256, 0));
        inpY->setFilter(allowedChars);
        inpY->setScale(0.8f);
        inpY->setVisible(false);
        inpY->getInputNode()->setID("IGNOREBYPASSES"_spr);

        menu->addChild(btn);
        menu->addChild(inpX);
        menu->addChild(inpY);
    }

    menu->addChild(inp);
    menu->addChild(label);
    menu->addChild(setBtn);
}

void SetValueModule::onSet(CCObject* sender)
{
    /*auto mod = as<SetValueModule*>(as<CCNode*>(sender)->getUserData());

    if (LevelEditorLayer::get() && LevelEditorLayer::get()->m_editorUI && (LevelEditorLayer::get()->m_editorUI->m_selectedObject || LevelEditorLayer::get()->m_editorUI->m_selectedObjects->count() > 0))
    {
        float v = 1.0f;

        auto vx = numFromString<float>(mod->inp->getString());

        if (vx.isOk())
        {
            v = vx.value();
        }

        float x = 1.0f;

        auto xx = numFromString<float>(mod->inpX->getString());

        if (xx.isOk())
        {
            x = xx.value();
        }

        float y = 1.0f;

        auto yx = numFromString<float>(mod->inpY->getString());

        if (yx.isOk())
        {
            y = yx.value();
        }

        auto selObj = LevelEditorLayer::get()->m_editorUI->m_selectedObject;

        if (mod->id == std::string("set-scale"))
        {
            if (selObj)
            {
                if (mod->inp->isVisible())
                {
                    selObj->m_scaleX = v;
                    selObj->m_scaleY = v;
                    selObj->setScale(v);
                }
                else
                {
                    selObj->m_scaleX = x;
                    selObj->m_scaleY = y;
                    selObj->setScaleX(x);
                    selObj->setScaleY(y);
                }
            }
            else
            {
                for (auto selObj : CCArrayExt<GameObject*>(LevelEditorLayer::get()->m_editorUI->m_selectedObjects))
                {
                    if (mod->inp->isVisible())
                    {
                        selObj->m_scaleX = v;
                        selObj->m_scaleY = v;
                        selObj->setScale(v);
                    }
                    else
                    {
                        selObj->m_scaleX = x;
                        selObj->m_scaleY = y;
                        selObj->setScaleX(x);
                        selObj->setScaleY(y);
                    }
                }
            }
        }

        if (mod->id == std::string("set-rot"))
        {
            if (selObj)
            {
                selObj->setRotation(v);
            }
            else
            {
                for (auto obj : CCArrayExt<GameObject*>(LevelEditorLayer::get()->m_editorUI->m_selectedObjects))
                {
                    obj->setRotation(v);
                }
            }
        }
    }*/
}

void SetValueModule::onScaleToggle(CCObject* sender)
{
    auto mod = as<SetValueModule*>(as<CCNode*>(sender)->getUserData());

    mod->inp->setVisible(!mod->inp->isVisible());
    mod->inpX->setVisible(!mod->inpX->isVisible());
    mod->inpY->setVisible(!mod->inpY->isVisible());
}

SetValueModule::SetValueModule(std::string name, std::string id)
{
    this->name = name;
    this->id = id;

    addToCache();
}

void SetValueModule::save() { }

void SetValueModule::load() { }