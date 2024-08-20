#include "CountdownLayer.hpp"

CountdownLayer* countdownInstance;

CountdownLayer* CountdownLayer::get()
{
    return countdownInstance;
}

bool CountdownLayer::init()
{
    if (!CCLayer::init())
        return false;

    auto down = as<InputModule*>(Client::GetModule("pause-countdown")->options[0])->getIntValue();

    countdownInstance = this;
    count = std::clamp<int>(down, 1, down);

    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);
    this->schedule(schedule_selector(CountdownLayer::onDecrement), 1);

    label = CCLabelBMFont::create(fmt::format("{}", count).c_str(), "goldFont.fnt");
    label->setPosition(CCDirector::get()->getWinSize() / 2);

    applyAnimation();

    if (auto gjbgl = GJBaseGameLayer::get())
    {
        this->addChild(gjbgl->m_uiLayer->m_pauseBtn->getParent());
    }

    this->addChild(label);
    return true;
}

void CountdownLayer::onDecrement(float)
{
    count--;
    label->setString(fmt::format("{}", count).c_str());
    applyAnimation();

    if (count == 0)
    {
        onCountReachedZero();
    }
}

void CountdownLayer::onCountReachedZero()
{
    PlayLayer::get()->resume();

    this->removeFromParent();
}

void CountdownLayer::applyAnimation()
{
    label->setScale(2.2f);
    label->runAction(CCEaseElasticOut::create(CCScaleTo::create(0.5f, 1.5f)));

    label->setOpacity(0);
    label->runAction(CCEaseInOut::create(CCFadeTo::create(0.5f, 255), 2));
}

void CountdownLayer::keyBackClicked()
{
    PlayLayer::get()->resume();
    PlayLayer::get()->pauseGame(false);

    this->removeFromParent();
}

void CountdownLayer::visit()
{
    label->visit();
}

CountdownLayer::~CountdownLayer()
{
    countdownInstance = nullptr;
}