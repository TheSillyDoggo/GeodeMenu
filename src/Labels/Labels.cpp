#ifdef STATUS_TEXTS

#include "Labels.h"

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

    hidden = Mod::get()->getSavedValue<bool>("hide-labels");

    labels.push_back(LabelNode::create(new LabelModule("Test #{number * 2}, Hello {name}!")));
    topLeft->addChild(labels[0]);

    update(1.0f);
    updateVis();

    reorderSides();
    reorderPosition();

    return true;
}

StatusNode* StatusNode::create()
{
    auto ret = new StatusNode();

    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}

StatusNode* StatusNode::get()
{
    return instance;
}

void StatusNode::update(float dt)
{
    auto timeScale = CCScheduler::get()->getTimeScale() / SpeedhackTop::instance->getFloatValue();

    _timeLeft -= dt / timeScale;
    _accum += 1 / (dt / timeScale);
    _frames++;

    if (_timeLeft <= 0) {
        this->fps = _accum / _frames;

        _timeLeft = _updateInterval;
        _accum = 0;
        _frames = 0;
    }

    for (auto label : labels)
    {
        label->update(dt);
    }
}

void StatusNode::updateVis()
{
    
}

void StatusNode::reorderSides()
{
    
}

void StatusNode::reorderPosition()
{
    
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

void StatusNode::updateCPS(float dt)
{
    
}

void LabelPlayLayer::resetLevel()
{
    PlayLayer::resetLevel();

    m_fields->attemptCount++;
}

bool LabelPlayLayer::init(GJGameLevel* p0, bool p1, bool p2)
{
    if (!PlayLayer::init(p0, p1, p2))
        return false;

    auto stn = StatusNode::create();
    stn->attPL = this;
    m_uiLayer->addChild(stn);

    return true;
}

class $modify (PlayerObject)
{
    void pushButton(PlayerButton p0)
    {
        PlayerObject::pushButton(p0);

        /*if (p0 == PlayerButton::Jump && PlayLayer::get())
        {
            if (auto stn = StatusNode::get())
            {
                stn->cps.push_back(1);

                stn->sLabels[8]->stopAllActions();
                stn->sLabels[8]->setColor(ccc3(0, 255, 0));
            }
        }*/
    }

    void releaseButton(PlayerButton p0)
    {
        PlayerObject::releaseButton(p0);

        /*if (p0 == PlayerButton::Jump && PlayLayer::get())
        {
            if (auto stn = StatusNode::get())
            {
                stn->sLabels[8]->stopAllActions();
                stn->sLabels[8]->runAction(CCTintTo::create(1, 255, 255, 255));
            }
        }*/
    }
};

#endif