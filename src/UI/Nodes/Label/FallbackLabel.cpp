#include "FallbackLabel.hpp"
#include "TTFCache.hpp"

using namespace geode::prelude;

FallbackLabel* FallbackLabel::create(const char* text, const char* fnt)
{
    auto pRet = new FallbackLabel();

    if (pRet && pRet->init(text, fnt))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool FallbackLabel::init(const char* text, const char* fnt)
{
    if (!CCLabelBMFont::initWithString(text, fnt))
        return false;

    return true;
}

void FallbackLabel::visit()
{
    if (ttfSprite)
    {
        ttfSprite->setOpacity(getOpacity());
        ttfSprite->setColor(getColor());

        this->transform();
        return ttfSprite->visit();
    }

    CCLabelBMFont::visit();
}

void FallbackLabel::updateLabel()
{
    if (ttfSprite)
    {
        ttfSprite->removeFromParent();
        ttfSprite = nullptr;
    }

    if (shouldUseFallback(getString(), getConfiguration()->getCharacterSet()))
    {
        if (getTTFFallback())
        {
            ttfSprite = CCSprite::createWithTexture(TTFCache::get()->getTexture(getString()));
            ttfSprite->setAnchorPoint(ccp(0, 0));
            ttfSprite->setPosition(ccp(0, 0));
            this->setContentSize(ttfSprite->getScaledContentSize());

            return;
        }
        else
        {
            std::string oldFnt = getFntFile();

            m_sFntFile = getFallbackFnt();
            CCLabelBMFont::updateLabel();
            m_sFntFile = oldFnt;
            return;
        }
    }

    CCLabelBMFont::updateLabel();
}