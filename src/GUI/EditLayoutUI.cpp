#include "EditLayoutUI.hpp"
#include "BetterButtonSprite.hpp"
#include "EasyBG.hpp"
#include <BetterAlertLayer.hpp>
#include <Button.hpp>
#include <LayoutModule.hpp>
#include <RealtimeAction.hpp>

using namespace geode::prelude;
using namespace qolmod;

EditLayoutUI* EditLayoutUI::create(LayoutModule* module)
{
    auto pRet = new EditLayoutUI();

    CCSize size = CCDirector::get()->getWinSize() * 0.8f;
    pRet->module = module;

    if (pRet && pRet->initAnchored(size.width, size.height))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool EditLayoutUI::setup()
{
    this->scheduleUpdate();

    m_bgSprite->setVisible(false);

    auto outline = CCScale9Sprite::create("GJ_square07.png");
    outline->setContentSize(m_size);
    outline->setAnchorPoint(ccp(0.5f, 0.5f));
    outline->setZOrder(69);

    m_buttonMenu->setVisible(false);
    m_mainLayer->addChildAtPosition(outline, Anchor::Center);

    auto title = AdvLabelBMFont::createWithLocalisation("edit-layout/title", "goldFont.fnt");
    title->setScale(0.7f);

    auto titleBG = EasyBG::create();
    titleBG->setTargettingNode(title);
    titleBG->setTargettingOffset(ccp(4, 4));

    auto menu = CCMenu::create();
    menu->setZOrder(80085);

    auto spr = BetterButtonSprite::createWithLocalisation(ccp(54.25f, 30), "ui/ok-button", "goldFont.fnt", "GJ_button_01.png");
    auto btn = Button::create(spr, this, menu_selector(EditLayoutUI::onClose));
    menu->addChild(btn);

    auto stencil = CCScale9Sprite::create("GJ_square01.png");
    stencil->setAnchorPoint(ccp(0.5f, 0.5f));
    stencil->setContentSize(m_size + ccp(-1, -1));

    auto clip = CCClippingNode::create(stencil);
    clip->setAlphaThreshold(0.03f);

    selectDarken = CCLayerColor::create(ccc4(0, 0, 0, 0));
    selectDarken->ignoreAnchorPointForPosition(false);

    auto bg = createBackground(true);
    bg->setScale(std::max<float>(stencil->getContentWidth() / bg->getContentWidth(), stencil->getContentHeight() / bg->getContentHeight()));

    clip->addChild(bg);
    clip->addChild(selectDarken);

    anchorPointsNode = CCDrawNode::create();
    anchorPointsNode->setBlendFunc(ccBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA }));
    anchorPointsNode->setOpacity(0);

    addAnchorPoint(35, true);
    addAnchorPoint(CCDirector::get()->getWinSize().width / 2, false);
    addAnchorPoint((151.125f / 2 + 22), false);
    addAnchorPoint(CCDirector::get()->getWinSize().width - (151.125f / 2 + 22), false);

    addAnchorPoint(CCDirector::get()->getWinSize().height / 2, true);
    addAnchorPoint(CCDirector::get()->getWinSize().height - 35, true);

    auto node = CCNode::create();
    node->setScale(0.8f);
    node->setContentSize(CCDirector::get()->getWinSize());
    node->setAnchorPoint(ccp(0.5f, 0.5f));

    node->addChild(anchorPointsNode);

    clip->addChild(node);

    previewNodeParent = CCNode::create();

    previewNode = module->createPreviewNode();
    previewNode->setPosition(ccp(0, 0));
    previewOutline = CCDrawNode::create();

    previewNodeParent->setPosition(module->getPosition());
    previewNodeParent->addChild(previewNode);

    if (auto l = typeinfo_cast<CCLayer*>(previewNode))
        l->setTouchEnabled(false);

    grab = GrabNodeLayer::create();
    grab->setNodeToGrab(grab);
    grab->setNodeContainerThing(node);

    node->addChild(previewNodeParent, 6);
    node->addChild(previewOutline, 7);
    node->addChild(grab, 7);

    m_mainLayer->addChildAtPosition(clip, Anchor::Center);
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, -5.0f));
    m_mainLayer->addChildAtPosition(titleBG, Anchor::Top, ccp(0, 15));
    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, 15));
    return true;
}

void EditLayoutUI::updateOutline()
{
    previewOutline->clear();
    previewOutline->setPosition(previewNodeParent->getPosition());
    previewNodeParent->setContentSize(previewNode->getContentSize());

    auto cs = previewNodeParent->getScaledContentSize() + CCSizeMake(10, 10);
    auto bl = ccp(-cs.width / 2, -cs.height / 2);
    auto br = ccp(cs.width / 2, -cs.height / 2);
    auto tl = ccp(-cs.width / 2, cs.height / 2);
    auto tr = ccp(cs.width / 2, cs.height / 2);

    grab->setContentSize(cs);

    auto col = grab->getDragging() ? ccc4f(0, 1, 0, 1) : ccc4f(0, 1, 1, 1);

    previewOutline->drawSegment(bl, br, 0.8f, col);
    previewOutline->drawSegment(br, tr, 0.8f, col);
    previewOutline->drawSegment(tr, tl, 0.8f, col);
    previewOutline->drawSegment(tl, bl, 0.8f, col);
}

void EditLayoutUI::update(float dt)
{
    updateOutline();
    updateAnchorPoints();

    if (grab->getDragging())
    {
        fixToScreenBounds();
        fixToAnchorPoints();

        previewNodeParent->setPosition(grab->getPosition());
        module->setPosition(grab->getPosition());
    }
    else
        grab->setPosition(previewNodeParent->getPosition());

    if (lastFrameSelected != grab->getDragging())
    {
        lastFrameSelected = grab->getDragging();

        selectDarken->stopAllActions();
        selectDarken->runAction(RealtimeAction::create(CCFadeTo::create(0.25f, lastFrameSelected ? 75 : 0)));

        anchorPointsNode->stopAllActions();
        anchorPointsNode->runAction(RealtimeAction::create(CCFadeTo::create(0.25f, lastFrameSelected && snapEnabled ? 150 : 0)));

        previewNodeParent->stopAllActions();
        if (lastFrameSelected)
            previewNodeParent->runAction(RealtimeAction::create(CCEaseInOut::create(CCScaleTo::create(0.25f, 1.08f), 2)));
        else
            previewNodeParent->runAction(RealtimeAction::create(CCEaseBackOut::create(CCScaleTo::create(0.25f, 1.0f))));
    }
}

void EditLayoutUI::fixToAnchorPoints()
{
    if (!snapEnabled)
        return;

    auto point = grab->getPosition();

    for (auto& anchor : anchorPoints)
    {
        if (anchor.second)
        {
            if (std::abs(point.y - anchor.first) < anchorSnapDis)
                point.y = anchor.first;
        }
        else
        {
            if (std::abs(point.x - anchor.first) < anchorSnapDis)
                point.x = anchor.first;
        }
    }

    grab->setPosition(point);
}

void EditLayoutUI::fixToScreenBounds()
{
    auto point = grab->getPosition();

    point.x = std::clamp<float>(point.x, 0, CCDirector::get()->getWinSize().width);
    point.y = std::clamp<float>(point.y, 0, CCDirector::get()->getWinSize().height);

    grab->setPosition(point);
}

void EditLayoutUI::addAnchorPoint(float position, bool horizontal)
{
    anchorPoints.push_back(std::make_pair(position, horizontal));
}

void EditLayoutUI::updateAnchorPoints()
{
    anchorPointsNode->clear();

    for (auto& anchor : anchorPoints)
    {
        auto colour = ccc4f(1, 0, 0, (float)anchorPointsNode->getOpacity() / 255.0f);

        CCPoint point1 = ccp(0, 0);
        CCPoint point2 = ccp(0, 0);
        float dis = 0;

        if (anchor.second)
        {
            point1 = ccp(0, anchor.first);
            point2 = ccp(CCDirector::get()->getWinSize().width, anchor.first);
            dis = std::abs(grab->getPositionY() - anchor.first);
        }
        else
        {
            point1 = ccp(anchor.first, 0);
            point2 = ccp(anchor.first, CCDirector::get()->getWinSize().height);
            dis = std::abs(grab->getPositionX() - anchor.first);
        }

        colour.a *= 1.0f - std::clamp<float>(std::clamp<float>(dis / anchorShowDis, 0, 0.7f) / 0.7f, 0, 1);

        anchorPointsNode->drawSegment(point1, point2, 1.0f, colour);
    }
}

CCNode* EditLayoutUI::createBackground(bool acu)
{
    auto imageNode = CCNode::create();
    imageNode->setContentSize(ccp(569, 320));
    imageNode->setAnchorPoint(ccp(0.5f, 0.5f));

    auto playerLayer = CCLayer::create();
    playerLayer->setVisible(false);
    imageNode->addChild(playerLayer);

    auto player = PlayerObject::create(GameManager::get()->m_playerFrame.value(), GameManager::get()->m_playerShip.value(), nullptr, playerLayer, false);
    CCSprite* nodeBG = nullptr;

    if (acu)
    {
        nodeBG = CCSprite::create("acu.png"_spr);
    
        player->setScale(0.6f);
        player->setPosition(ccp(1116.205f, 173.434f) + ccp(-906.705f, -24.134f));
        player->setRotation(-299.499f);
    }
    else
    {
        nodeBG = CCSprite::create("bloodbath.png"_spr);
    
        player->toggleFlyMode(true, true);
        player->setPosition(ccp(625.822f, 144.094f) + ccp(-416.322f, -80.000f));
        player->setRotation(-18.076f);
    }

    player->setColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));
    player->setSecondColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value()));

    if (GameManager::get()->m_playerGlow)
    {
        player->enableCustomGlowColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerGlowColor.value()));
        player->updatePlayerGlow();
    }

    nodeBG->setScale(320.0f / nodeBG->getContentHeight());
    nodeBG->setAnchorPoint(ccp(0, 0));

    imageNode->addChild(nodeBG);
    imageNode->addChild(player, 3);

    return imageNode;
}