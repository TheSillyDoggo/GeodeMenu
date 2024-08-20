#ifdef STATUS_TEXTS

#include "Labels.h"
#include "../Hacks/Noclip/Noclip.hpp"
#include "../Hacks/SafeMode/SafeMode.hpp"

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
    topLeft->setAnchorPoint(ccp(0, 1));
    topLeft->setPosition(CCDirector::get()->getWinSize() * ccp(0, 1) + ccp(2, 0));
    topLeft->setLayout(getLayout());
    topLeft->setID("top-left");

    topRight = CCMenu::create();
    topRight->ignoreAnchorPointForPosition(false);
    topRight->setAnchorPoint(ccp(1, 1));
    topRight->setPosition(CCDirector::get()->getWinSize() * ccp(1, 1) + ccp(-2, 0));
    topRight->setLayout(getLayout()->setCrossAxisAlignment(AxisAlignment::End)->setCrossAxisLineAlignment(AxisAlignment::End));
    topRight->setID("top-right");

    bottomLeft = CCMenu::create();
    bottomLeft->ignoreAnchorPointForPosition(false);
    bottomLeft->setAnchorPoint(ccp(0, 0));
    bottomLeft->setPosition(ccp(2, 2));
    bottomLeft->setLayout(getLayout()->setAxisReverse(false)->setAxisAlignment(AxisAlignment::Start));
    bottomLeft->setID("bottom-left");

    bottomRight = CCMenu::create();
    bottomRight->ignoreAnchorPointForPosition(false);
    bottomRight->setAnchorPoint(ccp(1, 0));
    bottomRight->setPosition(CCDirector::get()->getWinSize() * ccp(1, 0) + ccp(-2, 2));
    bottomRight->setLayout(getLayout()->setAxisReverse(false)->setAxisAlignment(AxisAlignment::Start)->setCrossAxisAlignment(AxisAlignment::End)->setCrossAxisLineAlignment(AxisAlignment::End));
    bottomRight->setID("bottom-right");

    bottomCenter = CCMenu::create();
    bottomCenter->ignoreAnchorPointForPosition(false);
    bottomCenter->setAnchorPoint(ccp(0.5f, 0));
    bottomCenter->setPosition(CCDirector::get()->getWinSize() * ccp(0.5f, 0) + ccp(0, 2));
    bottomCenter->setLayout(getLayout()->setAxisReverse(false)->setAxisAlignment(AxisAlignment::Start)->setCrossAxisAlignment(AxisAlignment::Center)->setCrossAxisLineAlignment(AxisAlignment::Center));
    bottomCenter->setID("bottom-center");

    hidden = Mod::get()->getSavedValue<bool>("hide-labels");

    for (auto label : window->modules)
    {
        labels.push_back(LabelNode::create(as<LabelModule*>(label)));
    }

    reorderSides();
    update(1.0f);

    this->addChild(topLeft);
    this->addChild(topRight);
    this->addChild(bottomLeft);
    this->addChild(bottomRight);
    this->addChild(bottomCenter);
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
    auto timeScale = CCScheduler::get()->getTimeScale() / (SpeedhackEnabled::instance->enabled ? SpeedhackTop::instance->getFloatValue() : 1);

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

    topLeft->updateLayout();
    topRight->updateLayout();
    bottomLeft->updateLayout();
    bottomRight->updateLayout();
    bottomCenter->updateLayout();
}

void StatusNode::reorderSides()
{
    for (auto label : labels)
    {
        label->retain();
    }

    topLeft->removeAllChildren();
    topRight->removeAllChildren();
    bottomLeft->removeAllChildren();
    bottomRight->removeAllChildren();
    bottomCenter->removeAllChildren();
    
    for (auto label : labels)
    {
        getNodeForSide(label->module->getSide())->addChild(label);

        label->release();
    }
}

CCNode* StatusNode::getNodeForSide(LabelSide side)
{
    switch(side)
    {
        default:
            return topLeft;

        case LabelSide::TopRight:
            return topRight;

        case LabelSide::BottomLeft:
            return bottomLeft;

        case LabelSide::BottomRight:
            return bottomRight;

        case LabelSide::BottomCenter:
            return bottomCenter;
    }
}

AxisLayout* StatusNode::getLayout()
{
    return AxisLayout::create()->setAxis(Axis::Column)->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End)->setCrossAxisAlignment(AxisAlignment::Start)->setCrossAxisLineAlignment(AxisAlignment::Start)->setAutoScale(false)->setGap(1);
}

StatusNode::~StatusNode()
{
    instance = nullptr;
}

// delegate :3

void LabelModuleDelegate::onModuleChanged(bool enabled)
{
    if (PlayLayer::get(); auto stn = StatusNode::get())
    {
        stn->reorderSides();
    }
}

void StatusNode::postSetup(Window* wnd)
{
    window = wnd;

    auto del = new LabelModuleDelegate();

    for (auto mod : wnd->modules)
    {
        if (mod->id != "labels-in-editor")
        {
            mod->delegate = del;
            mod->options[0]->delegate = del;
        }
    }
}

// hooks :3

void LabelPlayLayer::resetLevel()
{
    PlayLayer::resetLevel();

    m_fields->attemptCount++;

    if (auto status = StatusNode::get())
    {
        status->totalClicks = 0;
    }
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
                stn->totalClicks++;

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

                if (Client::GetModuleEnabled("status-cps-instant-fade"))
                    stn->sLabels[8]->setColor(ccc3(255, 255, 255));
                else
                    stn->sLabels[8]->runAction(CCTintTo::create(1, 255, 255, 255));
            }
        }*/
    }
};

void AttemptBaseGameLayer::resetLevelVariables()
{
    if (LevelEditorLayer::get() ? !LevelEditorLayer::get()->m_editorUI->m_playtestStopBtn->isVisible() : true)
        m_fields->attemptCount++;

    if (StatusNode::get())
        StatusNode::get()->totalClicks = 0;

    GJBaseGameLayer::resetLevelVariables();

    SafeMode::get()->resetOnNewAttempt();
}

#endif