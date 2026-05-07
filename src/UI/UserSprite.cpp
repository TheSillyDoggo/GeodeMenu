#include "UserSprite.hpp"

using namespace geode::prelude;
using namespace qolmod;

std::vector<UserSprite*> userSprites = {};

UserSprite* UserSprite::create(std::string str)
{
    auto pRet = new UserSprite();
    pRet->autorelease();
    pRet->updateSprite(str);
    return pRet;
}

void UserSprite::refreshAll()
{
    for (auto& spr : userSprites)
    {
        spr->refresh();
    }
}

void UserSprite::updateSprite(std::string str)
{
    this->str = str;

    if (!CCScene::get() || !CCDirector::get()->m_pobOpenGLView)
        return;

    if (CCScene::get()->getChildByType<LoadingLayer>(0))
        return;

    this->removeAllChildren();
    this->setContentSize(ccp(0, 0));
    
    if (str.starts_with("<sprite>"))
    {
        str = str.substr(strlen("<sprite>"));

        auto s = CCSprite::create(str.c_str());

        if (!s)
            return;

        s->setAnchorPoint(ccp(0, 0));
        s->setColor(getColor());
        s->setOpacity(getOpacity());

        this->setContentSize(s->getScaledContentSize());
        this->addChild(s);
    }
    else if (str.starts_with("<text>"))
    {
        str = str.substr(strlen("<text>"));

        auto l = CCLabelBMFont::create(str.c_str(), "bigFont.fnt");
        l->setScale(0.5f);
        l->setAnchorPoint(ccp(0, 0));
        l->setColor(getColor());
        l->setOpacity(getOpacity());

        this->setContentSize(l->getScaledContentSize());
        this->addChild(l);
    }
    else
    {
        auto s = CCSprite::createWithSpriteFrameName(str.c_str());

        if (!s)
            return;

        s->setAnchorPoint(ccp(0, 0));
        s->setColor(getColor());
        s->setOpacity(getOpacity());

        this->setContentSize(s->getScaledContentSize());
        this->addChild(s);
    }
}

void UserSprite::refresh()
{
    updateSprite(str);
}

void UserSprite::setOpacity(GLubyte opacity)
{
    CCNodeRGBA::setOpacity(opacity);

    refresh();
}

void UserSprite::setColor(const ccColor3B& color)
{
    CCNodeRGBA::setColor(color);

    refresh();
}


UserSprite::~UserSprite()
{
    userSprites.erase(std::find(
        userSprites.begin(),
        userSprites.end(),
        this
    ));
}

bool UserSprite::init()
{
    userSprites.push_back(this);
    return true;
}