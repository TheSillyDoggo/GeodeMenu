#include "FastBMFontConfig.hpp"

using namespace geode::prelude;

FastBMFontConfig* FastBMFontConfig::create(const char* fntFile)
{
    auto pRet = new FastBMFontConfig();

    if (pRet && pRet->init(fntFile))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool FastBMFontConfig::init(const char* fntFile)
{
    m_pKerningDictionary = nullptr;
    m_pFontDefDictionary = nullptr;
    
    m_pCharacterSet = this->parseConfigFile(fntFile);

    return m_pCharacterSet;
}

float faster_stof(std::string_view sv, size_t start, size_t count)
{
    float value;
    auto [ptr, ec] = std::from_chars(sv.data() + start, sv.data() + start + count, value);
    if (ptr != sv.data() + start && ec == std::errc()) return value;
    return 0.0f;
}

gd::set<unsigned int>* FastBMFontConfig::parseConfigFile(const char* fntFile)
{
    auto ccfu = CCFileUtils::get();

    gd::set<unsigned int>* charsSet = new gd::set<unsigned int>();
    auto fullpath = ccfu->fullPathForFilename(fntFile, false);

    std::ifstream file(fullpath);
    std::vector<char> buffer(1024);

    unsigned int elementIter = 0;
    unsigned int kerningIter = 0;
    tCCFontDefHashElement* elementPool = nullptr;
    tCCKerningHashElement* kerningPool = nullptr;

    while (file.getline(buffer.data(), buffer.size()))
    {
        std::string_view line(buffer.data(), file.gcount());

        if (line.starts_with("page id"))
        {
            auto beg = line.find('"') + 1;
            auto end = line.find('"', beg);
            // auto page = std::string(line.substr(beg, end - beg)).c_str();
            std::string page = "bigFont.png";

            m_sAtlasName = ccfu->fullPathFromRelativeFile(page.c_str(), fntFile);
            continue;
        }
        if (line.starts_with("common lineHeight"))
        {
            auto beg = line.find('=') + 1;
            auto end = line.find(' ', beg);

            m_nCommonHeight = faster_stof(line, beg, end-beg);
            continue;
        }
        if (line.starts_with("chars count="))
        {
            int max = faster_stof(line, line.find('=') + 1, 8);

            elementPool = static_cast<tCCFontDefHashElement*>(malloc(sizeof(tCCFontDefHashElement) * max));
            continue;
        }
        if (line.starts_with("kernings count="))
        {
            int max = faster_stof(line, line.find('=') + 1, 8);

            kerningPool = static_cast<tCCKerningHashElement*>(malloc(sizeof(tCCKerningHashElement) * max));
            continue;
        }
        if (line.starts_with("kerning first"))
        {
            int amount = 0;
            int first = 0;
            int second = 0;

            auto beg = line.find('=') + 1;
            auto end = line.find(' ', beg);
            first = faster_stof(line, beg, end-beg);

            beg = line.find('=', end) + 1;
            end = line.find(' ', beg);
            second = faster_stof(line, beg, end-beg);

            beg = line.find('=', end) + 1;
            end = line.find(' ', beg);
            amount = faster_stof(line, beg, end-beg);

            tCCKerningHashElement* element = &kerningPool[kerningIter];
            kerningIter++;
            *element = {};

            element->amount = amount;
            element->key = (first<<16) | (second&0xffff);
            HASH_ADD_INT(m_pKerningDictionary, key, element);
            continue;
        }
        if (line.starts_with("char "))
        {
            if (!elementPool)
            {
                log::error("This shouldn't have happened!\n{}: {}", fntFile, line);
                continue;
            }

            tCCFontDefHashElement* element = &elementPool[elementIter];
            elementIter++;
            *element = {};
            
            #define KEYPAIR(var) \
            size_t GEODE_CONCAT(var, beg) = 0; \
            size_t GEODE_CONCAT(var, end) = 0;

            KEYPAIR(charID);
            KEYPAIR(x);
            KEYPAIR(y);
            KEYPAIR(width);
            KEYPAIR(height);
            KEYPAIR(xoffset);
            KEYPAIR(yoffset);
            KEYPAIR(xadvance);

            charIDbeg = line.find('=') + strlen("=");
            charIDend = line.find('x', charIDbeg);

            xbeg = charIDend + strlen("x=");
            xend = xbeg + 6;

            ybeg = xend + strlen("y=");
            yend = ybeg + 6;

            widthbeg = yend + strlen("width=");
            widthend = widthbeg + 6;

            heightbeg = widthend + strlen("height=");
            heightend = heightbeg + 6;

            xoffsetbeg = heightend + strlen("xoffset=");
            xoffsetend = xoffsetbeg + 6;

            yoffsetbeg = xoffsetend + strlen("yoffset=");
            yoffsetend = yoffsetbeg + 6;

            xadvancebeg = yoffsetend + strlen("xadvance=");
            xadvanceend = xadvancebeg + 6;


            element->fontDef.charID = faster_stof(line, charIDbeg, charIDend-charIDbeg - 1);
            element->fontDef.rect.origin.x =    faster_stof(line, xbeg, xend-xbeg);
            element->fontDef.rect.origin.y =    faster_stof(line, ybeg, yend-ybeg);
            element->fontDef.rect.size.width =  faster_stof(line, widthbeg, widthend-widthbeg);
            element->fontDef.rect.size.height = faster_stof(line, heightbeg, heightend-heightbeg);
            element->fontDef.xOffset =          faster_stof(line, xoffsetbeg, xoffsetend-xoffsetbeg);
            element->fontDef.yOffset =          faster_stof(line, yoffsetbeg, yoffsetend-yoffsetbeg);
            element->fontDef.xAdvance =         faster_stof(line, xadvancebeg, xadvanceend-xadvancebeg);

            element->key = element->fontDef.charID;
            HASH_ADD_INT(m_pFontDefDictionary, key, element);

            charsSet->insert(element->fontDef.charID);
            continue;
        }
        if (line.starts_with("info face"))
        {
            // gd doesnt use padding so i dont need to impl
            continue;
        }
    }
    
    file.close();
    return charsSet;
}

bool hookEnabled = false;

CCBMFontConfiguration* FastBMFontConfiguration::create(const char *FNTfile)
{
    // if (hookEnabled)
        // return FastBMFontConfig::create(FNTfile);

    return CCBMFontConfiguration::create(FNTfile);
}

void FastBMFontConfig::quickLoad(const char* fntFile)
{
    hookEnabled = true;
    cocos2d::FNTConfigLoadFile(fntFile);
    hookEnabled = false;
}