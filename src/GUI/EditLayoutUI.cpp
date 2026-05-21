#include "EditLayoutUI.hpp"
#include "BetterButtonSprite.hpp"
#include "EasyBG.hpp"
#include <BetterAlertLayer.hpp>
#include <Button.hpp>
#include <LayoutModule.hpp>

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

    auto spr = BetterButtonSprite::createWithLocalisation(ccp(54.25f, 30), "ui/ok-button", "goldFont.fnt", "GJ_button_01.png");
    auto btn = Button::create(spr, this, menu_selector(EditLayoutUI::onClose));
    menu->addChild(btn);

    auto stencil = CCScale9Sprite::create("GJ_square01.png");
    stencil->setAnchorPoint(ccp(0.5f, 0.5f));
    stencil->setContentSize(m_size + ccp(-1, -1));

    auto clip = CCClippingNode::create(stencil);
    clip->setAlphaThreshold(0.03f);

    auto node = CCNode::create();
    node->setScale(0.8f);
    node->setContentSize(ccp(569, 320));
    node->setAnchorPoint(ccp(0.5f, 0.5f));

    playerLayer = CCLayer::create();

    auto player = PlayerObject::create(GameManager::get()->m_playerFrame.value(), GameManager::get()->m_playerShip.value(), nullptr, playerLayer, false);

    #ifdef BLOODBATH
    auto nodeBG = CCSprite::create("bloodbath.png"_spr);
    
    player->toggleFlyMode(true, true);
    player->setPosition(ccp(625.822f, 144.094f) + ccp(-416.322f, -80.000f));
    player->setRotation(-18.076f);
    #else
    auto nodeBG = CCSprite::create("acu.png"_spr);
    
    player->setScale(0.6f);
    player->setPosition(ccp(1116.205f, 173.434f) + ccp(-906.705f, -24.134f));
    player->setRotation(-299.499f);
    #endif

    nodeBG->setScale(320.0f / nodeBG->getContentHeight());
    nodeBG->setAnchorPoint(ccp(0, 0));

    node->addChild(nodeBG);
    node->addChild(player, 1);

    clip->addChild(stencil);
    clip->addChild(node);

    previewNode = module->createPreviewNode();
    previewOutline = CCDrawNode::create();

    if (auto l = typeinfo_cast<CCLayer*>(previewNode))
        l->setTouchEnabled(false);

    grab = GrabNodeLayer::create();
    grab->setNodeToGrab(grab);
    grab->setNodeContainerThing(node);

    node->addChild(previewNode, 6);
    node->addChild(previewOutline, 7);
    node->addChild(grab, 7);

    m_mainLayer->addChildAtPosition(clip, Anchor::Center);
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));
    m_mainLayer->addChildAtPosition(titleBG, Anchor::Top, ccp(0, -18));
    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, -18));
    return true;
}

void EditLayoutUI::updateOutline()
{
    previewOutline->clear();
    previewOutline->setPosition(previewNode->getPosition());

    auto cs = previewNode->getScaledContentSize() + CCSizeMake(10, 10);
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

    if (grab->getDragging())
        previewNode->setPosition(grab->getPosition());
    else
        grab->setPosition(previewNode->getPosition());
    /*auto col = ColourUtils::get()->getPastel("preview-background");

    nodeBG->setColor(col);
    ground->updateGround01Color(ccc3(col.r * 0.7f, col.g * 0.7f, col.b * 0.7f));*/
}