#include "LabelBMFont.hpp"
#include <Utils.hpp>

using namespace qolmod;
using namespace geode::prelude;

LabelBMFont* LabelBMFont::create(const char* str, const char* fnt)
{
    auto pRet = new LabelBMFont();

    if (pRet && pRet->init(str, fnt))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool LabelBMFont::init(const char* str, const char* fnt)
{
    if (!CCSpriteBatchNode::init())
        return false;

    ensureCount(64);
    setString(str);
    setFntFile(fnt);

    return true;
}

LabelBMFont::~LabelBMFont()
{
    for (auto spr : fontSprites)
    {
        spr->release();
    }
}

void LabelBMFont::updateLabel()
{
    this->removeAllChildren();

    if (!config)
        return;

    unsigned int i = 0;
    CCSprite* spr = nullptr;
    tCCFontDefHashElement* element = nullptr;
    bool inTag = false;
    std::wstring tag;
    float nextCharX = 0;
    float nextCharY = 0;
    ccColor3B colour = whiteColour;

    for (auto ch : string)
    {
        if (inTag)
        {
            if (ch == '>')
            {
                inTag = false;
                tag += ch;
                if (settings.useColourTags)
                    colour = handleTag(tag);
                continue;
            }

            tag += ch;
            continue;
        }
        else
        {
            if (ch == '<')
            {
                inTag = true;
                tag = '<';
                continue;
            }
        }

        spr = getCharSprite(i);

        if (config->m_pCharacterSet->contains((int)ch))
        {
            unsigned int key = ch;
            HASH_FIND_INT(config->m_pFontDefDictionary, &key, element);
            if (! element)
            {
                if (ch != ' ')
                {
                    // CCLOGWARN("cocos2d::CCLabelBMFont: characer not found %d", c);
                }
                continue;
            }

            spr->setTextureRect(CC_RECT_PIXELS_TO_POINTS(element->fontDef.rect));
            spr->refreshTextureRect();
        }
        spr->setColor(colour);
        spr->setPosition(ccp(nextCharX, nextCharY) + CC_POINT_PIXELS_TO_POINTS(ccp(element->fontDef.xOffset, element->fontDef.yOffset)));
        nextCharX += CC_POINT_PIXELS_TO_POINTS(ccp(element->fontDef.xAdvance, 0)).x;

        this->setContentSize(ccp(nextCharX, spr->getContentHeight()));
        this->insertQuadFromSprite(spr, i);
        i++;
    }
}

cocos2d::CCSprite* LabelBMFont::getCharSprite(unsigned int index)
{
    ensureCount(index);

    return fontSprites[index];
}

void LabelBMFont::ensureCount(unsigned int count)
{
    while (fontSprites.size() < count)
    {
        auto spr = CCSprite::create();
        spr->retain();
        spr->setBatchNode(this);
        spr->setAnchorPoint(ccp(0, 0));

        fontSprites.push_back(spr);
    }
}

#define COLCHECK($col, $ret) \
if (tag == $col) \
    return cc3bFromHexString($ret).unwrap();

ccColor3B LabelBMFont::handleTag(std::wstring tag)
{
    COLCHECK(L"<ca>", "#9632ff")
    COLCHECK(L"<cb>", "#4a52e1")
    COLCHECK(L"<cc>", "#ffff96")
    COLCHECK(L"<cd>", "#ff96ff")
    COLCHECK(L"<cf>", "#96ffff")
    COLCHECK(L"<cg>", "#40e348")
    COLCHECK(L"<cj>", "#32c8ff")
    COLCHECK(L"<cl>", "#60abef")
    COLCHECK(L"<co>", "#ffa54b")
    COLCHECK(L"<cp>", "#ff00ff")
    COLCHECK(L"<cr>", "#ff5a5a")
    COLCHECK(L"<cs>", "#ffdc41")
    COLCHECK(L"<cy>", "#ffff00")

    return whiteColour;
}

void LabelBMFont::setString(const char *label)
{
    this->string = qolmod::utils::toWideString(label);
    this->utf8String = label;
    updateLabel();
}

const char* LabelBMFont::getString(void)
{
    return utf8String.c_str();
}

void LabelBMFont::setFntFile(const char* fntFile)
{
    this->fntFile = fntFile;

    this->config = cocos2d::FNTConfigLoadFile(fntFile);

    if (config)
        this->setTexture(CCTextureCache::get()->addImage(config->getAtlasName(), false));

    updateLabel();
}

const char* LabelBMFont::getFntFile()
{
    return fntFile.c_str();
}



/*#include <Geode/modify/MenuLayer.hpp>

class $modify (MenuLayer)
{
    bool init()
    {
        MenuLayer::init();

        auto l = LabelBMFont::create("<cc>me</c>ow <cy>私</c>の<cp>日本語</c>は<cc>下手</c>だ", "kosugimaru.fnt"_spr);
        // auto l = LabelBMFont::create("hello :cat: meow", "bigFont.fnt");
        l->setPosition(getContentSize() / 2);
        this->addChild(l, 67);

        return true;
    }
};*/