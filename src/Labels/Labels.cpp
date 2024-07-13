#ifdef STATUS_TEXTS

#include "Labels.h"
#include "../Hacks/Noclip/Noclip.hpp"

bool StatusNode::init()
{
    if (!CCNode::init())
        return false;

    instance = this;
    
    this->setID("status-node"_spr);
    this->setZOrder(69);
    this->scheduleUpdate();

    topLeft = CCMenu::create();
    topLeft->ignoreAnchorPointForPosition(false);
    topLeft->setID("top-left");
    this->addChild(topLeft);

    topRight = CCMenu::create();
    topRight->ignoreAnchorPointForPosition(false);
    topRight->setID("top-right");
    this->addChild(topRight);

    bottomLeft = CCMenu::create();
    bottomLeft->ignoreAnchorPointForPosition(false);
    bottomLeft->setID("bottom-left");
    this->addChild(bottomLeft);

    bottomRight = CCMenu::create();
    bottomRight->ignoreAnchorPointForPosition(false);
    bottomRight->setID("bottom-right");
    this->addChild(bottomRight);

    int count = 9;

    for (size_t i = 0; i < count; i++)
    {
        auto lbl = CCLabelBMFont::create("boobs", "bigFont.fnt");
        lbl->setAnchorPoint(ccp(0, 1));
        lbl->setPositionX(3);
        lbl->setTag(i);

        sLabels.push_back(lbl);
    }

    sLabels[0]->setString(".");
    as<CCNode*>(sLabels[0]->getChildren()->objectAtIndex(0))->setScale(2.25f);
    as<CCNode*>(sLabels[0]->getChildren()->objectAtIndex(0))->setAnchorPoint(ccp(0.2f, 0.35f));

    hidden = Mod::get()->getSavedValue<bool>("hide-labels");

    update(1.0f);
    updateVis();

    reorderSides();
    reorderPosition();

    return true;
}

void StatusNode::updateVis()
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
        sLabels[i]->setScale(0.5f * scale);
        sLabels[i]->setOpacity((int)round(255 * op));

        if (hidden)
            sLabels[i]->setVisible(false);
    }
}

void StatusNode::reorderSides()
{
    for (auto label : sLabels)
    {
        label->retain();
    }

    topLeft->removeAllChildrenWithCleanup(false);
    topRight->removeAllChildrenWithCleanup(false);
    bottomLeft->removeAllChildrenWithCleanup(false);
    bottomRight->removeAllChildrenWithCleanup(false);

    int i = 0;

    for (auto label : sLabels)
    {
        int side = as<DropdownModule*>(window->modules[i + 2]->options[0])->index;

        label->setAnchorPoint(ccp((side == 0 || side == 2) ? 0 : 1, (side == 2 || side == 3) ? 0 : 1));
        label->setAlignment((side == 0 || side == 2) ? CCTextAlignment::kCCTextAlignmentLeft : CCTextAlignment::kCCTextAlignmentRight);

        (side == 0 ? topLeft : (side == 1 ? topRight : (side == 2 ? bottomLeft : bottomRight)))->addChild(label);

        i++;

        label->release();
    }
}

void StatusNode::reorderPosition()
{
    float op = 0.9f, scale = 1.0f;

    auto o = numFromString<float>(StatusOpacity::instance->text);
    if (o.isOk())
        op = o.value();

    auto s = numFromString<float>(StatusScale::instance->text);
    if (s.isOk())
        scale = s.value();

    op = clamp<float>(op, 0.0f, 1.0f);

    int v = 0;

    for (size_t i = 0; i < bottomLeft->getChildrenCount(); i++)
    {
        as<CCNode*>(bottomLeft->getChildren()->objectAtIndex(i))->setPosition(ccp(3, 3 + (32.5f * scale * 0.5f) * v));

        if (as<CCNode*>(bottomLeft->getChildren()->objectAtIndex(i))->isVisible())
            v++;
    }

    v = 0;

    for (size_t i = 0; i < bottomRight->getChildrenCount(); i++)
    {
        as<CCNode*>(bottomRight->getChildren()->objectAtIndex(i))->setPosition(ccp(CCDirector::get()->getWinSize().width - 3, 3 + (32.5f * scale * 0.5f) * v));

        if (as<CCNode*>(bottomRight->getChildren()->objectAtIndex(i))->isVisible())
            v++;
    }

    v = 0;

    for (size_t i = 0; i < topLeft->getChildrenCount(); i++)
    {
        as<CCNode*>(topLeft->getChildren()->objectAtIndex(i))->setPosition(ccp(3, CCDirector::get()->getWinSize().height - (3 + (32.5f * scale * 0.5f) * v)));

        if (as<CCNode*>(topLeft->getChildren()->objectAtIndex(i))->isVisible())
            v++;
    }

    v = 0;

    for (size_t i = 0; i < topRight->getChildrenCount(); i++)
    {
        as<CCNode*>(topRight->getChildren()->objectAtIndex(i))->setPosition(ccp(CCDirector::get()->getWinSize().width - 3, CCDirector::get()->getWinSize().height - (3 + (32.5f * scale * 0.5f) * v)));

        if (as<CCNode*>(topRight->getChildren()->objectAtIndex(i))->isVisible())
            v++;
    }
}

class LabelModuleDelegate : public ModuleChangeDelegate
{
    virtual void onModuleChanged(bool enabled)
    {
        if (PlayLayer::get())
        {
            if (auto stn = StatusNode::get())
            {
                stn->reorderSides();
                stn->reorderPosition();
            }
        }
    }
};

void StatusNode::postSetup(Window* wnd)
{
    window = wnd;

    auto del = new LabelModuleDelegate();

    for (auto mod : wnd->modules)
    {
        mod->delegate = del;
        mod->options[0]->delegate = del;
    }
}

void StatusNode::update(float dt)
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

    if (!replay)
        replay = Client::GetModule("status-replay");

    if (!attempt)
        attempt = Client::GetModule("status-attempt");

    if (!message)
        message = Client::GetModule("status-message");

    if (!session)
        session = Client::GetModule("status-session");

    if (!cpsM)
        cpsM = Client::GetModule("status-cps");
        
    if (!attPL)
        attPL = static_cast<AttemptPlayLayer*>(PlayLayer::get());
    
    float v = 100 * as<NoclipPlayLayer*>(PlayLayer::get())->getNoclipAccuracy();
    

    sLabels[0]->setVisible(cheat->enabled);
    sLabels[1]->setVisible(fps->enabled);
    sLabels[2]->setVisible(noclip->enabled && accuracy->enabled);
    sLabels[3]->setVisible(noclip->enabled && deaths->enabled);
    sLabels[4]->setVisible(attempt->enabled);
    sLabels[5]->setVisible(replay->enabled);
    //sLabels[6]->setVisible(replay->enabled && (GJReplayManager::recording || GJReplayManager::playing));
    //sLabels[7]->setVisible(replay->enabled && (GJReplayManager::recording || GJReplayManager::playing));
    sLabels[6]->setVisible(message->enabled);
    sLabels[7]->setVisible(session->enabled);
    sLabels[8]->setVisible(cpsM->enabled);


    sLabels[2]->setString((numToString(v, 2) + std::string("%")).c_str());
    sLabels[3]->setString((numToString(as<NoclipPlayLayer*>(PlayLayer::get())->m_fields->d, 0) + (as<NoclipPlayLayer*>(PlayLayer::get())->m_fields->d == 1 ? std::string(" Death") : std::string(" Deaths"))).c_str());
    sLabels[4]->setString((std::string("Attempt ") + std::to_string(attPL->m_fields->attemptCount)).c_str());

    std::stringstream ss;
    //ss << "Frame: " << numToString(GJReplayManager::frame) << ", Delta: " << numToString(GJReplayManager::dt, 4);

    std::stringstream inp;
    //inp << GJReplayManager::replay.inputs.size() << (GJReplayManager::replay.inputs.size() == 1 ? " Input" : " Inputs") << ", " << GJReplayManager::replay.frames.size() << (GJReplayManager::replay.frames.size() == 1 ? " Frame" : " Frames");

    std::string b = (std::string("Frame Fixes: ") + (Mod::get()->getSavedValue<bool>("frame-fixes") ? "Enabled" : "Disabled") + std::string(", Click Fixes: ") + (Mod::get()->getSavedValue<bool>("click-fixes") ? "Enabled" : "Disabled"));
    sLabels[5]->setString(ss.str().c_str());
    //sLabels[6]->setString(b.c_str());
    //sLabels[7]->setString(inp.str().c_str());
    auto v2 = as<InputModule*>(message->options[1])->text.c_str();
    sLabels[6]->setString(v2);
    sLabels[7]->setString(formatTime(ColourUtility::totalSessionTime).c_str());

    if (as<NoclipPlayLayer*>(PlayLayer::get())->m_fields->isDead)
    {
        sLabels[2]->stopAllActions();
        sLabels[2]->setColor(ccc3(255, 0, 0));
        sLabels[2]->runAction(CCTintTo::create(0.5f, 255, 255, 255));

        sLabels[3]->stopAllActions();
        sLabels[3]->setColor(ccc3(255, 0, 0));
        sLabels[3]->runAction(CCTintTo::create(0.5f, 255, 255, 255));

        as<NoclipPlayLayer*>(PlayLayer::get())->m_fields->isDead = false;
    }

    sLabels[1]->setString(std::to_string((int)roundf(CCDirector::sharedDirector()->m_fFrameRate)).append(" FPS").c_str());

    for (size_t i = 0; i < cps.size(); i++)
    {
        cps[i] -= dt / CCScheduler::get()->getTimeScale();
    }

    cps.erase(std::remove_if(cps.begin(), cps.end(), [](float i){ return i < 0; }), cps.end());


    auto cpsTotalStr = cpsM->options[1]->enabled ? "/" + std::to_string(totalCPS) : "";

    sLabels[8]->setString((std::to_string(as<int>(cps.size()))).append(cpsTotalStr).append(" CPS").c_str());

    updateVis();
}

void StatusNode::updateCPS(float dt)
{
    
}

class $modify (GJBaseGameLayer) {
    void handleButton(bool down, int button, bool isPlayer1) {
        GJBaseGameLayer::handleButton(down, button, isPlayer1);
        if (!PlayLayer::get()) return;
        if (down) {
            if (button == 1 && isPlayer1) {
                if (auto stn = StatusNode::get()) {
                    stn->cps.push_back(1);
                    stn->totalCPS++;

                    stn->sLabels[8]->stopAllActions();
                    stn->sLabels[8]->setColor(ccc3(0, 255, 0));
                }
            }
        } else if (button == 1 && isPlayer1) {
            if (auto stn = StatusNode::get()) {
                stn->sLabels[8]->stopAllActions();
                stn->sLabels[8]->runAction(CCTintTo::create(0.1, 255, 255, 255));
            }
        }
    }
};

class $modify (PlayLayer)
{
    bool init(GJGameLevel* p0, bool p1, bool p2)
    {
        if (!PlayLayer::init(p0, p1, p2))
            return false;

        if (getChildByID("status-node"_spr))
            return true;

        auto stn = StatusNode::create();
        stn->attPL = static_cast<AttemptPlayLayer*>(PlayLayer::get());
        this->addChild(stn);
        
        return true;
    }
    void resetLevel() {
        PlayLayer::resetLevel();
        if(auto stn = StatusNode::get()) {
            stn->totalCPS = 0;
        }
    }
};

#endif