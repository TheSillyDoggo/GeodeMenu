#include "LanguageSelectNode.hpp"
#include "../Utils/UnspeedhackedAction.hpp"
#include "../Utils/TranslationManager.hpp"

bool LanguageSelectNode::init()
{
    if (!CCNode::init())
        return false;

    this->setContentSize(CCDirector::get()->getWinSize());
    this->setAnchorPoint(CCPointZero);
    this->setKeypadEnabled(true);

    auto colBG = CCLayerColor::create(ccc4(0, 0, 0, 0));
    colBG->ignoreAnchorPointForPosition(false);
    colBG->setAnchorPoint(ccp(1, 1));
    colBG->runAction(CCFadeTo::create(0.5f, 100));

    node = CCMenu::create();
    node->ignoreAnchorPointForPosition(false);
    node->setContentSize(CCPointZero);
    node->setScale(0);
    node->runAction(UnspeedhackedAction::create(CCEaseElasticOut::create(CCScaleTo::create(0.5f, 1), 0.6f)));

    auto leftBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"), this, menu_selector(LanguageSelectNode::onLeft));
    leftBtn->setZOrder(420);
    node->addChildAtPosition(leftBtn, Anchor::BottomLeft, ccp(-190, 0));

    auto rightBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"), this, menu_selector(LanguageSelectNode::onRight));
    rightBtn->getNormalImage()->setScaleX(-1);
    rightBtn->setZOrder(420);
    node->addChildAtPosition(rightBtn, Anchor::BottomLeft, ccp(190, 0));

    node->setTouchPriority(-999999);

    if (!TranslationManager::get()->isLanguageLoaded())
        goToPage(0);
    else
        goToPage(Mod::get()->getSavedValue<std::string>("loaded-translation", "none"));

    this->addChild(colBG);
    this->addChildAtPosition(node, Anchor::Center);
    return true;
}

void LanguageSelectNode::goToPage(std::string fileName)
{
    auto langs = Client::get()->getLanguages();

    int i = 1;

    for (auto lang : langs)
    {
        if (lang.filename() == fileName)
            return goToPage(i);

        i++;
    }

    goToPage(0);
}

void LanguageSelectNode::goToPage(int page)
{
    this->page = page;

    if (layer)
    {
        layer->removeFromParent();
        layer = nullptr;
    }

    auto langs = Client::get()->getLanguages();

    if (page == 0)
        layer = TranslationCreditsLayer::create(matjson::parse("{ \"display_name_english\": \"Default\", \"display_name_native\": \"English\", \"contributors\": [] }").unwrapOr("{}"), "none");
    else
        layer = TranslationCreditsLayer::create(file::readJson(langs[page - 1]).unwrap(), langs[page - 1]);

    layer->setPosition(CCDirector::get()->getWinSize() * -0.5f);
    layer->stopAllActions();
    layer->setOpacity(0);
    layer->setKeypadEnabled(false);
    layer->setKeyboardEnabled(false);

    layer->baseLayer->stopActionByTag(69);
    layer->baseLayer->setScale(1);

    layer->ok->setTarget(this, menu_selector(LanguageSelectNode::onSubmit));

    node->addChild(layer, 420);

    auto c = ColourUtility::getChromaColour(0.5f * changedBy);

    layer->background->setColor(c);

    for (auto child : CCArrayExt<CCSprite*>(layer->ground->getChildByType<CCSpriteBatchNode>(0)->getChildByType<CCSprite>(0)->getChildren()))
    {
        child->setColor(ccc3(c.r * 0.6f, c.g * 0.6f, c.b * 0.6f));
    }
}

void LanguageSelectNode::onLeft(CCObject* sender)
{
    page--;
    changedBy--;

    if (page < 0)
        page = Client::get()->getLanguages().size();

    goToPage(page);
}

void LanguageSelectNode::onRight(CCObject* sender)
{
    page++;
    changedBy++;

    if (page > Client::get()->getLanguages().size())
        page = 0;

    goToPage(page);
}

void LanguageSelectNode::onSubmit(CCObject* sender)
{
    keyBackClicked();
}

void LanguageSelectNode::keyBackClicked()
{
    this->removeFromParent();
}

LanguageSelectNode* LanguageSelectNode::addToScene()
{
    auto pRet = LanguageSelectNode::create();

    CCScene::get()->addChild(pRet, 99999);

    return pRet;
}