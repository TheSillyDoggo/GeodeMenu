#include "FrameStepper.hpp"

#define MAX_STORED_STATES 240

void SteppedBaseGameLayer::storeState()
{
    if (!typeinfo_cast<PlayLayer*>(this))
        return;

    if (m_player1 && m_player1->m_isDead)
        return;

    auto fields = m_fields.self();
    
    if (fields->states.size() + 1 > MAX_STORED_STATES)
        fields->states.erase(fields->states.begin());

    auto cp = as<PlayLayer*>(base_cast<GJBaseGameLayer*>(this))->createCheckpoint();

    fields->states.push_back(cp);

    as<SteppedUILayer*>(m_uiLayer)->updateUI();
}

void SteppedBaseGameLayer::restoreState()
{
    if (!typeinfo_cast<PlayLayer*>(this))
        return;

    auto fields = m_fields.self();

    if (fields->states.size() == 0)
        return;

    if (auto cp = fields->states.at(fields->states.size() - 1))
    {
        auto pl = as<PlayLayer*>(base_cast<GJBaseGameLayer*>(this));
        
        pl->storeCheckpoint(cp);

        pl->resetLevel();
        
        pl->removeCheckpoint(false);
    }

    fields->states.pop_back();

    as<SteppedUILayer*>(m_uiLayer)->updateUI();
}

void SteppedBaseGameLayer::checkRepellPlayer()
{
    GJBaseGameLayer::checkRepellPlayer();

    storeState();
}

void SteppedBaseGameLayer::update(float dt)
{
    if (getPausedUpdate())
        return;

    GJBaseGameLayer::update(dt);
}

void SteppedBaseGameLayer::stepFrame()
{
    m_fields->steppingUpdate = true;

    GJBaseGameLayer::update(1.0f / (Client::GetModuleEnabled("tps-bypass") ? as<InputModule*>(Client::GetModule("tps-bypass")->options[0])->getFloatValue() : 240.0f));

    m_fields->steppingUpdate = false;
}

bool SteppedBaseGameLayer::getPausedUpdate()
{
    return m_fields->steppingUpdate ? false : m_fields->paused;
}

bool SteppedUILayer::init(GJBaseGameLayer* bgl)
{
    if (!UILayer::init(bgl))
        return false;

    auto menu = CCMenu::create();
    menu->ignoreAnchorPointForPosition(false);
    menu->setContentSize(ccp(135, 40));

    auto bg = CCScale9Sprite::create("square02_001.png");
    bg->setScale(1.0f / 3);
    bg->setContentSize(menu->getContentSize() * 3);
    bg->setAnchorPoint(CCPointZero);
    bg->setOpacity(100);

    menu->setPosition(ccp(135 / 2, 40 / 2) + ccp(25, 25));

    m_fields->pause = RepeatableMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_pauseBtn_001.png"), this, menu_selector(SteppedUILayer::onTogglePaused));
    m_fields->pause->getNormalImage()->setScale(0.8f);

    m_fields->prev = RepeatableMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png"), this, menu_selector(SteppedUILayer::onPreviousFrame));
    m_fields->prev->getNormalImage()->setScale(0.8f);

    auto next = RepeatableMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png"), this, menu_selector(SteppedUILayer::onStepFrame));
    next->getNormalImage()->setScale(0.8f);
    as<CCSprite*>(next->getNormalImage())->setFlipX(true);

    menu->addChildAtPosition(m_fields->pause, Anchor::Center);
    menu->addChildAtPosition(next, Anchor::Right, ccp(-18, 0));
    menu->addChildAtPosition(m_fields->prev, Anchor::Left, ccp(18, 0));
    menu->addChild(bg, -1);

    this->addChild(menu, 69);

    m_fields->menu = menu;
    m_fields->next = next;

    updateUI();

    return true;
}

void SteppedUILayer::onTogglePaused(CCObject*)
{
    if (m_gameLayer)
    {
        auto fields = as<SteppedBaseGameLayer*>(m_gameLayer)->m_fields.self();

        fields->paused = !fields->paused;

        if (fields->paused)
            FMODAudioEngine::get()->pauseAllMusic(true);
        else
            FMODAudioEngine::get()->resumeAllMusic();
    }

    updateUI();
}

void SteppedUILayer::onPreviousFrame(CCObject* sender)
{
    if (m_gameLayer)
        as<SteppedBaseGameLayer*>(m_gameLayer)->restoreState();
    
    updateUI();
}

void SteppedUILayer::onStepFrame(CCObject*)
{
    if (m_gameLayer)
        as<SteppedBaseGameLayer*>(m_gameLayer)->stepFrame();

    updateUI();
}

void SteppedUILayer::updateUI()
{
    if (!m_gameLayer)
        return;

    m_fields->prev->setEnabled(as<SteppedBaseGameLayer*>(m_gameLayer)->m_fields->states.size() != 0);
    m_fields->prev->setColor(m_fields->prev->isEnabled() ? ccWHITE : ccc3(150, 150, 150));

    if (!typeinfo_cast<PlayLayer*>(m_gameLayer))
    {
        m_fields->prev->setEnabled(false);
        m_fields->prev->setColor(ccc3(150, 150, 150));
    }

    as<CCSprite*>(m_fields->pause->getNormalImage())->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName(as<SteppedBaseGameLayer*>(m_gameLayer)->m_fields->paused ? "GJ_playEditorBtn_001.png" : "GJ_pauseEditorBtn_001.png"));

    m_fields->menu->setVisible(Client::GetModuleEnabled("frame-stepper"));

    auto position = ccp(Mod::get()->getSavedValue<float>("frame-stepper-position.x", 135 / 2 + 25), Mod::get()->getSavedValue<float>("frame-stepper-position.y", 40 / 2 + 25));
    auto scale = Mod::get()->getSavedValue<float>("frame-stepper-scale", 1);

    m_fields->menu->setPosition(position);
    m_fields->menu->setScale(scale);
}

bool SteppedKeyboardDispatcher::dispatchKeyboardMSG(enumKeyCodes key, bool isKeyDown, bool isKeyRepeat)
{
    CCKeyboardDispatcher::dispatchKeyboardMSG(key, isKeyDown, isKeyRepeat);

    if (isKeyDown || isKeyRepeat)
    {
        if (auto bgl = GJBaseGameLayer::get())
        {
            if (auto ui = bgl->m_uiLayer)
            {
                if (key == enumKeyCodes::KEY_N)
                {
                    as<SteppedUILayer*>(ui)->onPreviousFrame(nullptr);
                }

                if (key == enumKeyCodes::KEY_M)
                {
                    as<SteppedUILayer*>(ui)->onStepFrame(nullptr);
                }

                if (key == enumKeyCodes::KEY_B)
                {
                    as<SteppedUILayer*>(ui)->onTogglePaused(nullptr);
                }
            }
        }
    }

    return true;
}

class FrameStepperUIDelegate : public ModuleChangeDelegate
{
    virtual void initOptionsLayer(CCLayer* options)
    {
        options->addChild(EditPositionLayer::create(EditPositionType::FrameStepper));
    }
};

$execute
{
    Loader::get()->queueInMainThread([]
    {
        Client::GetModule("frame-stepper")->delegate = new FrameStepperUIDelegate();
        Client::GetModule("frame-stepper")->onToggle = [](bool enabled)
        {
            if (GJBaseGameLayer::get())
            {
                as<SteppedUILayer*>(GJBaseGameLayer::get()->m_uiLayer)->m_fields->menu->setVisible(enabled);
                as<SteppedUILayer*>(GJBaseGameLayer::get()->m_uiLayer)->updateUI();
            }
        };
    });
}