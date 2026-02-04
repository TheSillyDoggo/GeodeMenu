#include "AdvLabelBMFont.hpp"
#include "../../Localisation/LocalisationManager.hpp"
#include <FastBMFontConfig.hpp>

const std::string& AdvLabelStruct::getTotalString()
{
    return totalString;
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

AdvLabelBMFont* AdvLabelBMFont::createWithLocalisation(std::string localisationKey, std::string font)
{
    return createWithString(LocalisationManager::get()->getLocalisedString(localisationKey), font);
}

AdvLabelStruct AdvLabelBMFont::structFromString(std::string lbl, bool splitSpaces, bool showTags)
{
    AdvLabelStruct str;
    std::string c = "";
    std::vector<std::string> segments = {};
    str.splitSpaces = splitSpaces;
    str.totalString = lbl;

    if (lbl.starts_with("<bm>"))
    {
        lbl = lbl.substr(4);
        str.forceBM = true;
    }

    if (lbl.starts_with("<ttf>"))
    {
        lbl = lbl.substr(5);
        str.forceTTF = true;
    }

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

        if (ch == '\n')
        {
            if (!c.empty())
                segments.push_back(c);

            segments.push_back("\n");
            c = "";
            continue;
        }

        if (splitSpaces)
        {
            if (ch == ' ')
            {
                segments.push_back(c);
                c = "";
            }
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
        if (showTags) \
            str.parts.push_back(AdvLabelStruct::AdvPart({ AdvLabelStruct::AdvPartType::Label, segment, col })); \
        continue; \
    }

    for (auto segment : segments)
    {
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

        auto type = AdvLabelStruct::AdvPartType::Label;

        if (segment == "\n")
            type = AdvLabelStruct::AdvPartType::NewLine;

        str.parts.push_back(AdvLabelStruct::AdvPart({ type, segment, col }));
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
    bool useTTF = useTTFFont();
    visibleLabels.clear();

    this->usingTTFCurrently = useTTF;

    float x = 0;
    float y = 0;
    float height = 0;
    float width = 0;
    std::unordered_map<float, float> widthsForLine = {};
    float lnHeight = font == "goldFont.fnt" ? 26 : (font == "chatFont.fnt" ? 16.5f : 32.5f);

    for (auto lbl : labelsCached)
    {
        for (auto lbl2 : lbl.second)
        {
            lbl2->setVisible(false);
        }
    }

    int ttfsUsed = 0;
    int lblsUsed = 0;

    auto fon = LocalisationManager::get()->getAltFont();

    for (auto part : str.parts)
    {
        bool newLine = part.type == AdvLabelStruct::AdvPartType::NewLine;

        if (newLine)
        {
            widthsForLine.emplace(y, x);
            y += lnHeight + lineSpacing;
            x = 0;

            continue;
        }

        CCNode* node = nullptr;

        ccColor3B col = part.colour;

        if (!labelsCached.contains(fon))
            labelsCached.emplace(fon, std::vector<FallbackLabel*>({}));

        if (!labelsCached.contains(font))
            labelsCached.emplace(font, std::vector<FallbackLabel*>({}));

        if (useTTF)
        {
            FallbackLabel* lbl;

            if (labelsCached[fon].size() > ttfsUsed)
            {
                lbl = labelsCached[fon][ttfsUsed];
                lbl->setString(part.label.c_str());
            }
            else
            {
                lbl = FallbackLabel::create(part.label.c_str(), fon.c_str());
                if (LocalisationManager::get()->getCurrentLang()->getTrueTypeFallback())
                {
                    lbl->setTTFFallback(true);
                    lbl->setForceFallback(true);
                }
                labelsCached[fon].push_back(lbl);

                this->addChild(lbl);
            }

            visibleLabels.push_back(lbl);
            lbl->setUserFlag("no-colour", col == ccWHITE);

            lbl->setVisible(true);

            ttfsUsed++;
            lbl->setColor(col);
            lbl->setOpacity(part.opacity * getOpacity());
            
            if (lbl->getContentHeight() == 0)
                lbl->setScale(1.0f);
            else
                lbl->setScale(lnHeight / lbl->getContentHeight());

            node = lbl;
        }
        else
        {
            FallbackLabel* lbl = nullptr;

            if (labelsCached[font].size() > lblsUsed)
            {
                lbl = labelsCached[font][lblsUsed];
                lbl->setString(part.label.c_str());
            }
            else
            {
                lbl = FallbackLabel::create(part.label.c_str(), font.c_str());
                labelsCached[font].push_back(lbl);

                this->addChild(lbl);
            }

            visibleLabels.push_back(lbl);
            lbl->setUserFlag("no-colour", col == ccWHITE);

            lbl->setVisible(true);

            lblsUsed++;
            lbl->setColor(col);
            lbl->setOpacity(part.opacity * getOpacity());
            
            if (lbl->getContentHeight() == 0)
                lbl->setScale(1.0f);
            else
                lbl->setScale(lnHeight / lbl->getContentHeight());

            node = lbl;
        }

        node->setAnchorPoint(ccp(0, 0));

        if (maxWidth != 0)
        {
            if (x + node->getScaledContentWidth() > maxWidth)
            {
                widthsForLine.emplace(y, x);
                y += lnHeight + lineSpacing;
                x = 0;
            }
        }

        height = std::max<float>(node->getScaledContentHeight() + y, height);

        node->setPosition(ccp(x, y));
        x += node->getScaledContentWidth();
        width = std::max<float>(width, x);
    }

    widthsForLine.emplace(y, x);
    lineCount = widthsForLine.size();

    this->setContentSize(ccp(width, height));

    for (auto label : visibleLabels)
    {
        float al = 0;

        switch (alignment)
        {
            case kCCTextAlignmentLeft:
                break;

            case kCCTextAlignmentCenter:
                al = 0.5f;
                break;

            case kCCTextAlignmentRight:
                al = 1;
                break;
        }

        float w = (width - widthsForLine[label->getPositionY()]) * al;

        label->setPosition(ccp(label->getPositionX() + w, height - (label->getPositionY() + lnHeight)));
    }

    updateLabelQuick();
}

void AdvLabelBMFont::updateLabelQuick()
{
    for (auto label : visibleLabels)
    {
        if (label->getUserFlag("no-colour"))
            label->setColor(colour);
    }
}

bool AdvLabelBMFont::useTTFFont()
{
    CCBMFontConfiguration* conf;
    gd::set<unsigned int>* charSet;
    bool ttf;

    if (str.forceTTF)
        return true;

    if (str.forceBM)
        return false;

    switch (ttfUsage)
    {
        case AdvLabelTTFUsage::None:
            return false;

        case AdvLabelTTFUsage::Auto:
            conf = getConfiguration();

            charSet = conf->getCharacterSet();

            ttf = false;

            for (auto ch : str.getTotalString())
            {
                if (ch == '\n')
                    continue;
                
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

bool AdvLabelBMFont::doesLabelContainNonDisplayableCharacter()
{
    auto conf = getConfiguration();
    auto conf2 = getConfiguration(LocalisationManager::get()->getAltFont());
    auto charSet = conf2->getCharacterSet();

    for (auto ch : str.getTotalString())
    {
        if (ch == '\n')
            continue;

        if ((int)ch == -29)
            continue;
        
        if (!charSet->contains(ch))
        {
            return true;
        }
    }

    return false;
}

bool AdvLabelBMFont::initWithStruct(AdvLabelStruct lblStruct, std::string font)
{
    if (!CCNode::init())
        return false;

    ttfUsage = LocalisationManager::get()->getDefaultTTFUsage();

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
    setStruct(structFromString(newString, splitEverySpace, showTags));
}

const char* AdvLabelBMFont::getString(void)
{
    return str.getTotalString().c_str();
}

void AdvLabelBMFont::setFntFile(const char* fntFile)
{
    this->font = fntFile;

    updateLabel();
}

const char* AdvLabelBMFont::getFntFile()
{
    return font.c_str();
}

void AdvLabelBMFont::setTTFUsage(AdvLabelTTFUsage usage)
{
    this->ttfUsage = usage;
}

AdvLabelTTFUsage AdvLabelBMFont::getTTFUsage()
{
    return ttfUsage;
}

CCBMFontConfiguration* AdvLabelBMFont::getConfiguration()
{
    if (!bmConfigs.contains(font))
    {
        FastBMFontConfig::quickLoad(font.c_str());
        auto conf = FallbackLabel::create("", font.c_str());
        conf->m_uReference = 80085; // so we never lose the config

        bmConfigs.emplace(font, conf);
    }

    return bmConfigs[font]->getConfiguration();
}

CCBMFontConfiguration* AdvLabelBMFont::getConfiguration(std::string font)
{
    if (!bmConfigs.contains(font))
    {
        FastBMFontConfig::quickLoad(font.c_str());
        auto conf = FallbackLabel::create("", font.c_str());
        conf->m_uReference = 80085; // so we never lose the config

        bmConfigs.emplace(font, conf);
    }

    return bmConfigs[font]->getConfiguration();
}

std::vector<FallbackLabel*> AdvLabelBMFont::getVisibleLabels()
{
    return visibleLabels;
}

void AdvLabelBMFont::setAlignment(CCTextAlignment alignment)
{
    this->alignment = alignment;

    updateLabel();
}

bool AdvLabelBMFont::isUsingTTFCurrently()
{
    return usingTTFCurrently;
}

void AdvLabelBMFont::setMaxWidth(float width)
{
    this->maxWidth = width;
}

int AdvLabelBMFont::getLineCount()
{
    return lineCount;
}

void AdvLabelBMFont::setSplitEverySpace(bool split)
{
    this->splitEverySpace = split;
}

void AdvLabelBMFont::setLineSpacing(float spacing)
{
    this->lineSpacing = spacing;
}

void AdvLabelBMFont::setShowTags(bool show)
{
    this->showTags = show;
}

std::vector<CCNodeRGBA*> AdvLabelBMFont::getCharacterNodes()
{
    std::vector<CCNodeRGBA*> nodes = {};

    for (auto lbl : visibleLabels)
    {
        for (auto child : CCArrayExt<CCNodeRGBA*>(lbl->getChildren()))
        {
            if (child->isVisible())
                nodes.push_back(child);
        }
    }

    return nodes;
}