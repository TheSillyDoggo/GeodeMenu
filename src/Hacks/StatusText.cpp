#define STATUS_TEXTS

#ifdef STATUS_TEXTS

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

#include "Noclip.cpp"

using namespace geode::prelude;

class StatusNode : public CCNode
{
    public:
        static StatusNode* create() {
            auto ret = new (std::nothrow) StatusNode;
            if (ret && ret->init()) {
                ret->autorelease();
                return ret;
            }
            delete ret;
            return nullptr;
        }

        //NoclipLayer* v;

        bool mods;

        static inline Module* fps = nullptr;
        static inline Module* cheat = nullptr;
        static inline Module* accuracy = nullptr;
        static inline Module* deaths = nullptr;

        static inline Module* noclip = nullptr;

        CCLabelBMFont* tl;
        CCLabelBMFont* tr;

        std::vector<CCLabelBMFont*> sLabels = {};

        bool init()
        {
            if (!CCNode::init())
                return false;
            
            this->setID("status-node"_spr);
            this->scheduleUpdate();

            return true;
        }

        void updateVis()
        {
            float op = 0.9f, scale = 1.0f;

            auto o = numFromString<float>(StatusOpacity::instance->text);
            if (o.isOk())
                op = o.value();

            auto s = numFromString<float>(StatusScale::instance->text);
            if (s.isOk())
                scale = s.value();

            op = clamp<float>(op, 0.0f, 1.0f);

            int y = 0;

            for (size_t i = 0; i < sLabels.size(); i++)
            {
                sLabels[i]->setPositionY((CCDirector::get()->getWinSize().height - (32.5f * y) * (0.5f * scale)) - 1);

                sLabels[i]->setScale(0.5f * scale);

                sLabels[i]->setOpacity((int)round(255 * op));

                if (sLabels[i]->isVisible())
                {
                    y++;
                }
            }
            
        }

        void update(float dt)
        {
            if (!cheat)
                cheat = Client::GetModule("cheat-indicator");
            
            if (!fps)
                fps = Client::GetModule("status-fps");

            if (!accuracy)
                accuracy = Client::GetModule("status-accuracy");

            if (!deaths)
                deaths = Client::GetModule("status-deaths");

            if (!noclip)
                noclip = Client::GetModule("noclip");

            
            float v = 100 * (1 - (PlayLayer::get()->m_gameState.m_unk1f8 == 0 ? 0 : as<NoclipLayer*>(PlayLayer::get())->m_fields->t / static_cast<float>(PlayLayer::get()->m_gameState.m_unk1f8)));
            

            sLabels[0]->setVisible(cheat->enabled);
            sLabels[1]->setVisible(fps->enabled);
            sLabels[2]->setVisible(noclip->enabled && accuracy->enabled);
            sLabels[3]->setVisible(noclip->enabled && deaths->enabled);


            sLabels[1]->setString((numToString(1 / (dt / CCScheduler::get()->getTimeScale()), 0) + std::string(" FPS")).c_str());
            sLabels[2]->setString((numToString(v, 2) + std::string("%")).c_str());
            sLabels[3]->setString((numToString(as<NoclipLayer*>(PlayLayer::get())->m_fields->d, 0) + (as<NoclipLayer*>(PlayLayer::get())->m_fields->d == 1 ? std::string(" Death") : std::string(" Deaths"))).c_str());

            if (as<NoclipLayer*>(PlayLayer::get())->m_fields->isDead)
            {
                sLabels[2]->stopAllActions();
                sLabels[2]->setColor(ccc3(255, 0, 0));
                sLabels[2]->runAction(CCTintTo::create(0.5f, 255, 255, 255));

                sLabels[3]->stopAllActions();
                sLabels[3]->setColor(ccc3(255, 0, 0));
                sLabels[3]->runAction(CCTintTo::create(0.5f, 255, 255, 255));

                as<NoclipLayer*>(PlayLayer::get())->m_fields->isDead = false;
            }

            updateVis();
        }
};

class $modify (PlayLayer)
{
    bool init(GJGameLevel* p0, bool p1, bool p2)
    {
        if (!PlayLayer::init(p0, p1, p2))
            return false;

        if (this->getChildByID("status-text-menu"_spr))
            return true;

        auto stn = StatusNode::create();

        auto menu = CCMenu::create();
        menu->setID("status-text-menu"_spr);
        menu->setPosition(ccp(0, 0));
        menu->setContentSize(CCDirector::get()->getWinSize());
        menu->setAnchorPoint(ccp(0, 0));
        menu->ignoreAnchorPointForPosition(false);

        int count = 4;

        for (size_t i = 0; i < count; i++)
        {
            auto lbl = CCLabelBMFont::create("penis", "bigFont.fnt");
            lbl->setAnchorPoint(ccp(0, 1));
            lbl->setPositionX(3);
            menu->addChild(lbl);

            stn->sLabels.push_back(lbl);
        }

        stn->sLabels[0]->setString(".");
        as<CCNode*>(stn->sLabels[0]->getChildren()->objectAtIndex(0))->setScale(2.25f);
        as<CCNode*>(stn->sLabels[0]->getChildren()->objectAtIndex(0))->setAnchorPoint(ccp(0.2f, 0.35f));

        menu->addChild(stn);

        this->addChild(menu, 69420);
        
        return true;
    }
};

#endif