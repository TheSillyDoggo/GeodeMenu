#include "SillyBaseLayer.h"

bool SillyBaseLayer::setup()
{
    m_mainLayer->setVisible(false);
    this->stopAllActions();
    this->setOpacity(0);

    if (!noBlur && Client::GetModuleEnabled("menu-bg-blur"))
    {
        blur = CCBlurLayer::create();
        blur->runAction(CCEaseIn::create(CCFadeTo::create(0.5f, 255), 2));
        this->addChild(blur);
    }

    this->runAction(CCFadeTo::create(1, 100));
    this->setKeypadEnabled(true);
    this->setTouchEnabled(false);

    auto l = CCMenu::create();
    l->setContentSize(size);
    l->setPosition(CCDirector::get()->getWinSize() / 2);
    l->ignoreAnchorPointForPosition(false);

    int theme = Mod::get()->getSavedValue<int>("theme", 5);

    std::stringstream ss;
    ss << "GJ_square0";
    ss << (theme < 0 ? 6 : theme);
    ss << ".png";

    auto panel = CCScale9Sprite::create(ss.str().c_str());
    panel->setContentSize(l->getContentSize());
    panel->setAnchorPoint(ccp(0, 0));
    panel->setID("panel");
    l->addChild(panel);

    as<CCNode*>(panel->getChildren()->objectAtIndex(0))->setZOrder(-2);

    if (auto grad = Loader::get()->getLoadedMod("thesillydoggo.gradientpages"))
    {
        auto size = panel->getContentSize();

        auto gradient = CCLayerGradient::create(ccc4(255, 255, 255, 255), ccc4(255, 255, 255, 255));
        gradient->setContentSize(size);
        gradient->setZOrder(-1);
        gradient->setID("gradient"_spr);

        if (grad->getSettingValue<bool>("use-custom-colours"))
        {
            gradient->setStartColor(grad->getSettingValue<ccColor3B>("primary-colour"));
            gradient->setEndColor(grad->getSettingValue<ccColor3B>("secondary-colour"));
        }
        else
        {
            gradient->setStartColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));
            gradient->setEndColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value()));
        }

        gradient->setPosition(CCDirector::get()->getWinSize() / 2);
        gradient->ignoreAnchorPointForPosition(false);

        if (grad->getSettingValue<bool>("reverse-order"))
            gradient->setScaleY(-1);

        auto darken = CCScale9Sprite::createWithSpriteFrameName((std::string("thesillydoggo.gradientpages/") + std::string("square-fill.png")).c_str());
        darken->setID("darken"_spr);
        darken->setContentSize(size - ccp(15, 15));
        darken->setZOrder(0);
        darken->setPosition(size / 2);

        auto outline = CCScale9Sprite::createWithSpriteFrameName((std::string("thesillydoggo.gradientpages/") + std::string("square-outline.png")).c_str());
        outline->setPosition(size / 2);
        outline->setContentSize(size);
        outline->setZOrder(1);
        outline->setID("outline"_spr);
        
        gradient->addChild(darken);
        gradient->addChild(outline);

        panel->addChild(gradient);

        gradient->setAnchorPoint(ccp(0, 0));
        gradient->setPosition(ccp(0, 0));

        gradient->setVisible(theme == -1);
    }

    if (theme == -2)
    {
        panel->setColor(ccc3(0, 0, 0));
        panel->setOpacity(175);

        auto out = CCScale9Sprite::create("GJ_square07.png");
        out->setContentSize(panel->getContentSize());
        out->setAnchorPoint(ccp(0, 0));
        panel->addChild(out);
    }

    auto title = CCLabelBMFont::create(_title.c_str(), "bigFont.fnt");
    title->setPosition(l->getContentSize() / 2 + ccp(0, (l->getContentSize().height / 2) - 15));
    title->setScale(0.6f);
    l->addChild(title);

    if (createWithOK)
    {
        ok = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK"), this, menu_selector(SillyBaseLayer::onClose));
        ok->setPosition(l->getContentSize() / 2 + ccp(0, -l->getContentSize().height / 2 + 23));
        l->addChild(ok);
    }

    if (animate)
    {
        l->setScale(0);
        l->runAction(CCEaseElasticOut::create(CCScaleTo::create(0.5f, 1), 0.6f));
    }

    this->addChild(l);

    baseLayer = l;

    this->customSetup();

    return true;
}

bool SillyBaseLayer::initWithSizeAndName(CCPoint size, std::string _title, bool createWithOK, bool animate, bool noBlur)
{
    this->_title = _title;
    this->createWithOK = createWithOK;
    this->animate = animate;
    this->noBlur = noBlur;
    this->size = size;

    if (!initAnchored(69, 420))
        return false;

    return true;
}

bool SillyBaseLayer::ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1)
{
    return false;
}

void SillyBaseLayer::ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1)
{

}

void SillyBaseLayer::ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1)
{

}

void SillyBaseLayer::ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1)
{

}

SillyBaseLayer::~SillyBaseLayer()
{

}