#include "AdvLabelBMFont.hpp"

std::string AdvLabelStruct::getTotalString()
{
    std::string ss;

    for (auto part : parts)
    {
        ss.append(part.label);
    }

    return ss;
}

AdvLabelBMFont* AdvLabelBMFont::createWithStruct(AdvLabelStruct lblStruct, std::string font)
{
    auto pRet = new AdvLabelBMFont();

    if (pRet && pRet->initWithStruct(lblStruct, font))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

AdvLabelBMFont* AdvLabelBMFont::createWithString(std::string label, std::string font)
{
    return createWithStruct(structFromString(label), font);
}

AdvLabelStruct AdvLabelBMFont::structFromString(std::string lbl)
{
    AdvLabelStruct str;
    std::string c = "";
    std::vector<std::string> segments = {};

    for (auto ch : utils::string::split(lbl))
    {
        if (ch == '<')
        {
            if (!c.empty())
                segments.push_back(c);

            c = "<";
            continue;
        }

        if (ch == '>')
        {
            c += ">";
            segments.push_back(c);
            c = "";
            continue;
        }

        c += ch;
    }

    if (!c.empty())
        segments.push_back(c);

    ccColor3B col = ccWHITE;

    #define COLCHECK(tag, hex) \
    if (segment == tag) \
    { \
        col = cc3bFromHexString(hex).unwrap(); \
        continue; \
    }

    for (auto segment : segments)
    {
        log::warn("segment: {}", segment);

        COLCHECK("<ca>", "#9632ff")
        COLCHECK("<cb>", "#4a52e1")
        COLCHECK("<cc>", "#ffff96")
        COLCHECK("<cd>", "#ff96ff")
        COLCHECK("<cf>", "#96ffff")
        COLCHECK("<cg>", "#40e348")
        COLCHECK("<cj>", "#32c8ff")
        COLCHECK("<cl>", "#60abef")
        COLCHECK("<co>", "#ffa54b")
        COLCHECK("<cp>", "#ff00ff")
        COLCHECK("<cr>", "#ff5a5a")
        COLCHECK("<cs>", "#ffdc41")
        COLCHECK("<cy>", "#ffff00")
        COLCHECK("</c>", "#ffffff")

        str.parts.push_back(AdvLabelStruct::AdvPart({ AdvLabelStruct::AdvPartType::Label, segment, col }));
    }

    return str;
}

void AdvLabelBMFont::setStruct(AdvLabelStruct str)
{
    this->str = str;

    updateLabel();
}

void AdvLabelBMFont::updateLabel()
{
    this->removeAllChildren();

    bool useTTF = useTTFFont();

    float x = 0;
    float height = 0;

    for (auto part : str.parts)
    {
        CCNode* node = nullptr;

        ccColor3B col = part.colour;

        if (col == ccWHITE)
            col = getColor();

        if (useTTF)
        {
            auto spr = CCSprite::createWithTexture(CCLabelTTFCache::get()->getTexture(part.label));
            spr->setScale(32.5f / spr->getContentHeight());

            spr->setColor(col);
            spr->setOpacity(part.opacity * getOpacity());

            node = spr;
        }
        else
        {
            auto lbl = CCLabelBMFont::create(part.label.c_str(), font.c_str());
            lbl->setColor(col);
            lbl->setOpacity(part.opacity * getOpacity());

            node = lbl;
        }

        height = std::max<float>(node->getScaledContentHeight(), height);

        node->setAnchorPoint(ccp(0, 0));

        node->setPosition(ccp(x, 0));
        x += node->getScaledContentWidth();

        this->addChild(node);
    }

    this->setContentSize(ccp(x, height));
}

bool AdvLabelBMFont::useTTFFont()
{
    CCBMFontConfiguration* conf;
    gd::set<unsigned int>* charSet;
    bool ttf;

    switch (ttfUsage)
    {
        case AdvLabelTTFUsage::None:
            return false;

        case AdvLabelTTFUsage::Auto:
            if (bmConfigs.contains(font))
            {
                conf = bmConfigs[font];
                conf->m_uReference = 80085;
            }
            else
            {
                conf = CCBMFontConfiguration::create(font.c_str());
                conf->m_uReference = 80085; // so we never lose the config

                bmConfigs.emplace(font, conf);
            }

            charSet = conf->getCharacterSet();

            ttf = false;

            for (auto ch : str.getTotalString())
            {
                if (!charSet->contains(ch))
                {
                    ttf = true;
                    break;
                }
            }

            return ttf;

        case AdvLabelTTFUsage::Forced:
            return true;
    }

    return false;
}

bool AdvLabelBMFont::initWithStruct(AdvLabelStruct lblStruct, std::string font)
{
    if (!CCNode::init())
        return false;

    this->setAnchorPoint(ccp(0.5f, 0.5f));
    this->font = font;

    setStruct(lblStruct);
    return true;
}

void AdvLabelBMFont::limitLabelWidth(float width, float defaultScale, float minScale)
{
    this->setScale(std::clamp<float>(width / getContentWidth(), minScale, defaultScale));
}

void AdvLabelBMFont::setString(const char *newString)
{
    setStruct(structFromString(newString));
}

const char* AdvLabelBMFont::getString(void)
{
    return str.getTotalString().c_str();
}