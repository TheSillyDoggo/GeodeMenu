#include "EndscreenStatLabel.hpp"
#include "../Level/Noclip/Noclip.hpp"

EndscreenStatLabel* EndscreenStatLabel::create(EndscreenStatType type)
{
    auto pRet = new EndscreenStatLabel();

    if (pRet && pRet->init(type))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool EndscreenStatLabel::init(EndscreenStatType type)
{
    if (!CCMenu::init())
        return false;

    this->type = type;
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(ccp(0.5f, 0.5f));

    label = AdvLabelBMFont::createWithString(getString(GJBaseGameLayer::get()), "goldFont.fnt");
    label->setTTFUsage(AdvLabelTTFUsage::None);
    label->updateLabel();
    label->setScale(0.8f);

    if (isClickable())
    {
        auto btn = CCMenuItemSpriteExtra::create(label, this, menu_selector(EndscreenStatLabel::onCallback));
        btn->m_scaleMultiplier = 1.1f;
        btn->setPosition(label->getScaledContentSize() / 2);

        this->addChild(btn);
    }
    else
    {
        label->setPosition(label->getScaledContentSize() / 2);

        this->addChild(label);
    }

    this->setContentSize(label->getScaledContentSize());
    return true;
}

std::string EndscreenStatLabel::getString(GJBaseGameLayer* layer)
{
    switch (type)
    {
        case EndscreenStatType::Attempts:
            return fmt::format("Attempts: {}", layer->m_attempts);

        case EndscreenStatType::Time:
        {
            std::chrono::milliseconds duration(static_cast<long long>(layer->m_gameState.m_totalTime * 1000));

            auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
            duration -= hours;
            auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
            duration -= minutes;
            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);

            if (hours.count() > 0)
            {
                return fmt::format("Time: {:02}:{:02}:{:02}", hours.count(), minutes.count(), seconds.count());
            }
            else
            {
                return fmt::format("Time: {:02}:{:02}", minutes.count(), seconds.count());
            }
        }

        case EndscreenStatType::Jumps:
            return fmt::format("Jumps: {}", static_cast<PlayLayer*>(layer)->m_jumps);

        case EndscreenStatType::Points:
            return fmt::format("Points: {}", layer->m_points);

        case EndscreenStatType::NoclipDeaths:
            return fmt::format("Deaths: {}", static_cast<NoclipBaseGameLayer*>(layer)->getNoclipDeaths(NoclipPlayerSelector::All));

        case EndscreenStatType::NoclipAccuracy:
            return fmt::format("Noclip: {:.2f}%", static_cast<NoclipBaseGameLayer*>(layer)->getNoclipAccuracy(NoclipPlayerSelector::All) * 100);
    
        default:
            return "";
    }
}

bool EndscreenStatLabel::isClickable()
{
    if (type == EndscreenStatType::NoclipDeaths)
        return true;

    if (type == EndscreenStatType::NoclipAccuracy)
        return true;

    return false;
}

void EndscreenStatLabel::setCallback(std::function<void()> callback)
{
    this->callback = callback;
}

void EndscreenStatLabel::onCallback(CCObject* sender)
{
    if (callback)
        callback();
}