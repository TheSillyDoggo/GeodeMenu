#include "StartposSwitcherUI.hpp"
#include "../Hacks/Utils/PlayLayer.hpp"
#include <Button.hpp>
#include <RealtimeAction.hpp>

using namespace qolmod;

StartposSwitcherUI* StartposSwitcherUI::create(GJBaseGameLayer* gameLayer, bool updatingEnabled)
{
    auto pRet = new StartposSwitcherUI();
    pRet->updatingEnabled = updatingEnabled;
    pRet->gameLayer = gameLayer;

    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool StartposSwitcherUI::init()
{
    if (!CCMenu::init())
        return false;

    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(ccp(0.5f, 0.5f));

    label = CCLabelBMFont::create("6 / 7", "bigFont.fnt");
    label->setScale(0.65f);

    nameLabel = CCLabelBMFont::create("(Unnamed)", "bigFont.fnt");
    nameLabel->limitLabelWidth(70, 0.35f, 0);

    leftSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    rightSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    rightSpr->setFlipX(true);

    leftSpr->setScale(0.65f);
    rightSpr->setScale(0.65f);

    auto leftBtn = qolmod::Button::create(leftSpr, this, menu_selector(StartposSwitcherUI::onSwitchBtn));
    auto rightBtn = qolmod::Button::create(rightSpr, this, menu_selector(StartposSwitcherUI::onSwitchBtn));
    
    leftBtn->setTag(-1);
    rightBtn->setTag(1);

    leftPercent = CCLabelBMFont::create("", "bigFont.fnt");
    leftPercent->setScale(0.45f);
    leftPercent->setAnchorPoint(ccp(1, 0.5f));

    rightPercent = CCLabelBMFont::create("", "bigFont.fnt");
    rightPercent->setScale(0.45f);
    rightPercent->setAnchorPoint(ccp(0, 0.5f));

    this->setContentSize(ccp((65 + leftSpr->getScaledContentWidth() / 2) * 2, leftSpr->getScaledContentHeight()));

    updateUI();
    runOpacityAction();

    this->addChildAtPosition(label, Anchor::Center);
    this->addChildAtPosition(nameLabel, Anchor::Center, ccp(0, -8.5f));
    this->addChildAtPosition(leftBtn, Anchor::Left, ccp(leftSpr->getScaledContentWidth() / 2, 0));
    this->addChildAtPosition(rightBtn, Anchor::Right, ccp(-rightSpr->getScaledContentWidth() / 2, 0));
    this->addChildAtPosition(leftPercent, Anchor::Left, ccp(-25 + leftSpr->getScaledContentWidth() / 2, 0));
    this->addChildAtPosition(rightPercent, Anchor::Right, ccp(25 - rightSpr->getScaledContentWidth() / 2, 0));
    return true;
}

void StartposSwitcherUI::onSwitchBtn(CCObject* sender)
{
    CCObject* newObj = nullptr;
    int index = gameLayer->m_startPosObjects->indexOfObject(gameLayer->m_startPosObject);

    switch (sender->getTag())
    {
        case -1: {
            if (!gameLayer->m_startPosObject)
                newObj = gameLayer->m_startPosObjects->lastObject();
            else
            {
                if (gameLayer->m_startPosObject == gameLayer->m_startPosObjects->firstObject())
                    newObj = nullptr;
                else
                    newObj = gameLayer->m_startPosObjects->objectAtIndex(index - 1);
            }
            break;
        };
        case 1: {
            if (!gameLayer->m_startPosObject)
                newObj = gameLayer->m_startPosObjects->firstObject();
            else
            {
                if (gameLayer->m_startPosObject == gameLayer->m_startPosObjects->lastObject())
                    newObj = nullptr;
                else
                    newObj = gameLayer->m_startPosObjects->objectAtIndex(index + 1);
            }
            break;
        };
    
        default:
            break;
    }

    gameLayer->setActiveStartPosObject(static_cast<StartPosObject*>(newObj));

    if (gameLayer->isRunning())
    {
        auto pl = static_cast<PlayLayer*>(gameLayer);
        if (pl->m_isPracticeMode)
            pl->resetLevelFromStart();

        pl->PlayLayer::resetLevel();
        pl->startMusic();
    }

    runOpacityAction();
}

void StartposSwitcherUI::updateDisplayedOpacity(GLubyte parentOpacity)
{
    label->setOpacity(parentOpacity);
    nameLabel->setOpacity(parentOpacity);
    leftSpr->setOpacity(parentOpacity);
    rightSpr->setOpacity(parentOpacity);
    leftPercent->setOpacity(parentOpacity);
    rightPercent->setOpacity(parentOpacity);
}

float StartposSwitcherUI::getPercentForObject(GameObject* go)
{
    if (!go || !PlayLayer::get())
        return 0;

    auto len = PlayLayer::get()->m_levelLength;

    if (len == 0)
        len = 1;

    return (go->getPositionX() / len) * 100;
}

void StartposSwitcherUI::updateUI()
{
    updateDisplayedOpacity(std::lerp<float>(regularOpacity, 1.0f, opacityMode) * 255);
    if (!updatingEnabled)
    {
        leftPercent->setString("");
        rightPercent->setString("");
        label->setString("6 / 7");
        return;
    }

    uint16_t cur = gameLayer->m_startPosObject ? (gameLayer->m_startPosObjects->indexOfObject(gameLayer->m_startPosObject) + 1) : 0;

    label->setString(fmt::format("{} / {}", cur, gameLayer->m_startPosObjects->count()).c_str());
    label->limitLabelWidth(80, 0.65f, 0);

    nameLabel->setVisible(showName);
    nameLabel->setString(fmt::format("({})", gameLayer->m_startPosObject ? gameLayer->m_startPosObject->getDisplayName() : "None").c_str());
    nameLabel->limitLabelWidth(70, 0.35f, 0);

    if (usePercentage)
    {
        /*
        // TODO: IMPLEMENT IN 2.209
        float percent = getPercentForObject(index == 0 ? nullptr : startPoses[index - 1]);

        label->setString(fmt::format("{}%", utils::numToString<float>(percent, 2)).c_str());

        index = currentStartpos - 1;
        if (index == -1)
            index = startPoses.size();

        percent = getPercentForObject(index == 0 ? nullptr : startPoses[index - 1]);
        
        leftPercent->setString(fmt::format("{}%", utils::numToString<float>(percent, 2)).c_str());

        index = currentStartpos + 1;
        if (index == startPoses.size() + 1)
            index = 0;

        percent = getPercentForObject(index == 0 ? nullptr : startPoses[index - 1]);
        
        rightPercent->setString(fmt::format("{}%", utils::numToString<float>(percent, 2)).c_str());*/
    }
    else
    {
        leftPercent->setString("");
        rightPercent->setString("");
    }

    if (PlayLayerUtils::getUtils()->m_fields->startPositions.size() == 0)
        setScale(0);
}

void StartposSwitcherUI::updateTweenAction(float value, const char* key)
{
    opacityMode = value;
}

void StartposSwitcherUI::runOpacityAction()
{
    this->stopActionByTag(80085);
    auto seq = CCSequence::create(
        CCActionTween::create(0.08f, "opacity", opacityMode, 1.0f),
        CCDelayTime::create(1.5f),
        CCEaseOut::create(CCActionTween::create(0.2f, "opacity", 1.0f, 0.0f), 2.0f),
        nullptr
    );
    auto act = RealtimeAction::create(seq);
    act->setTag(80085);
    this->runAction(act);
}