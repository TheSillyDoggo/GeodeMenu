#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/UILayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (StartposPlayLayer, PlayLayer)
{
    struct Fields {
        std::vector<StartPosObject*> objs;
        int selectedIndex = -1;

        CCMenu* menu;
        CCSprite* left;
        CCSprite* right;
        CCLabelBMFont* label;
    };

    void setStartpos(int index)
    {
        if (m_fields->objs.empty())
            return;

        if (index < -1)
            index = m_fields->objs.size() - 1;

        if (index >= m_fields->objs.size())
            index = -1;

        m_fields->selectedIndex = index;

        m_currentCheckpoint = nullptr;

        auto spos = index >= 0 ? m_fields->objs[index] : nullptr;

        setStartPosObject(spos);

        if (m_isPracticeMode)
            resetLevelFromStart();

        resetLevel();
        startMusic();

        updateUI();
    }

    void onLeft(CCObject*)
    {
        setStartpos(m_fields->selectedIndex - 1);
    }

    void onRight(CCObject*)
    {
        setStartpos(m_fields->selectedIndex + 1);
    }

    void addObject(GameObject* obj)
    {
        if (obj->m_objectID == 31)
        {
            m_fields->objs.push_back(as<StartPosObject*>(obj));

            //if (!as<StartPosObject*>(obj)->m_startSettings->m_disableStartPos)
                //m_fields->selectedIndex++;
        }

        PlayLayer::addObject(obj);
    }

    void createObjectsFromSetupFinished()
    {
        PlayLayer::createObjectsFromSetupFinished();

        if (m_fields->menu)
            m_fields->menu->removeFromParent();

        m_fields->menu = CCMenu::create();
        m_fields->menu->setContentSize(ccp(0, 0));
        m_fields->menu->setPosition(CCDirector::get()->getWinSize().width / 2, 50);

        m_fields->label = CCLabelBMFont::create("among us sex", "bigFont.fnt");

        m_fields->left = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
        m_fields->right = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
        m_fields->left->setScale(0.65f);
        m_fields->right->setScale(0.65f);

        m_fields->right->setFlipX(true);

        auto l = CCMenuItemSpriteExtra::create(m_fields->left, this, menu_selector(StartposPlayLayer::onLeft));
        auto r = CCMenuItemSpriteExtra::create(m_fields->right, this, menu_selector(StartposPlayLayer::onRight));

        m_fields->menu->addChildAtPosition(l, Anchor::Center, ccp(-65, 0));
        m_fields->menu->addChildAtPosition(r, Anchor::Center, ccp(65, 0));
        m_fields->menu->addChildAtPosition(m_fields->label, Anchor::Center);

        m_uiLayer->addChild(m_fields->menu);

        if (m_fields->objs.empty())
            m_fields->menu->setScale(0);

        updateUI();
    }

    void updateUI()
    {
        if (!m_fields->menu || !m_fields->label)
            return;

        m_fields->label->setString(fmt::format("{}/{}", m_fields->selectedIndex + 1, m_fields->objs.size()).c_str());
        m_fields->label->limitLabelWidth(100, 0.65f, 0);

        auto action = CCSequence::create(CCFadeTo::create(0.1f, 225), CCFadeTo::create(0.6f, 225), CCFadeTo::create(0.3f, 50), nullptr);
        auto action2 = CCSequence::create(CCFadeTo::create(0.1f, 225), CCFadeTo::create(0.6f, 225), CCFadeTo::create(0.3f, 50), nullptr);
        auto action3 = CCSequence::create(CCFadeTo::create(0.1f, 225), CCFadeTo::create(0.6f, 225), CCFadeTo::create(0.3f, 50), nullptr);
        
        m_fields->label->runAction(action);
        m_fields->left->runAction(action2);
        m_fields->right->runAction(action3);
    }
};

class $modify (UILayer)
{
    virtual void keyDown(enumKeyCodes key)
    {
        if (auto pl = PlayLayer::get())
        {
            if (key == enumKeyCodes::KEY_Q)
                as<StartposPlayLayer*>(pl)->setStartpos(as<StartposPlayLayer*>(pl)->m_fields->selectedIndex - 1);

            if (key == enumKeyCodes::KEY_E)
                as<StartposPlayLayer*>(pl)->setStartpos(as<StartposPlayLayer*>(pl)->m_fields->selectedIndex + 1);
        }

        UILayer::keyDown(key);
    }
};