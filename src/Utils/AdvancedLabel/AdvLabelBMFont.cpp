#include "AdvLabelBMFont.hpp"
#include "../../Localisation/LocalisationManager.hpp"

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

AdvLabelBMFont* AdvLabelBMFont::createWithLocalisation(std::string localisationKey, std::string font)
{
    return createWithString(LocalisationManager::get()->getLocalisedString(localisationKey), font);
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
    bool useTTF = useTTFFont();

    float x = 0;
    float height = 0;

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
        CCNode* node = nullptr;

        ccColor3B col = part.colour;

        if (col == ccWHITE)
            col = getColor();

        if (useTTF)
        {
            CCLabelBMFont* lbl;

            if (labelsCached[fon].size() > ttfsUsed)
            {
                lbl = labelsCached[fon][ttfsUsed];
                lbl->setString(part.label.c_str());
            }
            else
            {
                lbl = CCLabelBMFont::create(part.label.c_str(), fon.c_str());
                labelsCached[fon].push_back(lbl);

                this->addChild(lbl);
            }

            lbl->setVisible(true);

            ttfsUsed++;
            float height = font == "goldFont.fnt" ? 26 : (font == "chatFont.fnt" ? 16.5f : 32.5f);

            lbl->setColor(col);
            lbl->setOpacity(part.opacity * getOpacity());
            lbl->setScale(height / lbl->getContentHeight());

            node = lbl;
        }
        else
        {
            CCLabelBMFont* lbl = nullptr;

            if (labelsCached[font].size() > lblsUsed)
            {
                lbl = labelsCached[font][lblsUsed];
                lbl->setString(part.label.c_str());
            }
            else
            {
                lbl = CCLabelBMFont::create(part.label.c_str(), font.c_str());
                labelsCached[font].push_back(lbl);

                this->addChild(lbl);
            }

            lbl->setVisible(true);

            lblsUsed++;
            lbl->setColor(col);
            lbl->setOpacity(part.opacity * getOpacity());

            node = lbl;
        }

        height = std::max<float>(node->getScaledContentHeight(), height);

        node->setAnchorPoint(ccp(0, 0));

        node->setPosition(ccp(x, 0));
        x += node->getScaledContentWidth();
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
        auto conf = CCBMFontConfiguration::create(font.c_str());
        conf->m_uReference = 80085; // so we never lose the config

        bmConfigs.emplace(font, conf);
    }

    return bmConfigs[font];
}