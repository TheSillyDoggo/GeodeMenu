/*#include <Geode/Geode.hpp>
#include <Geode/modify/CCBMFontConfiguration.hpp>

using namespace geode::prelude;
using namespace std::chrono;

void* __address = 0x0;

float fast_stof(std::string_view sv)
{
    float value;
    auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
    if (ec == std::errc()) return value;
    return 0.0f;
}

float faster_stof(std::string_view sv, size_t start, size_t count)
{
    float value;
    auto [ptr, ec] = std::from_chars(sv.data() + start, sv.data() + start + count, value);
    if (ec == std::errc()) return value;
    return 0.0f;
}

gd::set<unsigned int>* HK__parseConfigFile(CCBMFontConfiguration* self, const char *FNTfile)
{
    auto start = steady_clock::now();

    auto ccfu = CCFileUtils::get();

    gd::set<unsigned int>* charsSet = new gd::set<unsigned int>();
    auto fullpath = ccfu->fullPathForFilename(FNTfile, false);

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

            self->m_sAtlasName = ccfu->fullPathFromRelativeFile(std::string(line.substr(beg, end - beg)).c_str(), FNTfile);
            continue;
        }
        if (line.starts_with("common lineHeight"))
        {
            auto beg = line.find('=') + 1;
            auto end = line.find(' ', beg);

            self->m_nCommonHeight = faster_stof(line, beg, end-beg);
            continue;
        }
        if (line.starts_with("chars count="))
        {
            int max = faster_stof(line, line.find('=') + 1, 8);

            elementPool = static_cast<tCCFontDefHashElement*>(malloc(sizeof(tCCFontDefHashElement) * max));
        }
        if (line.starts_with("kernings count="))
        {
            int max = faster_stof(line, line.find('=') + 1, 8);

            kerningPool = static_cast<tCCKerningHashElement*>(malloc(sizeof(tCCKerningHashElement) * max));
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
            HASH_ADD_INT(self->m_pKerningDictionary, key, element);
            continue;
        }
        if (line.starts_with("char "))
        {
            if (!elementPool)
            {
                log::error("uhh");
                continue;
            }

            tCCFontDefHashElement* element = &elementPool[elementIter];
            elementIter++;
            *element = {};
            
            #define KEYPAIR(var) \
            static size_t GEODE_CONCAT(var, beg) = 0; \
            static size_t GEODE_CONCAT(var, end) = 0;

            KEYPAIR(charID);
            KEYPAIR(x);
            KEYPAIR(y);
            KEYPAIR(width);
            KEYPAIR(height);
            KEYPAIR(xoffset);
            KEYPAIR(yoffset);
            KEYPAIR(xadvance);

            charIDbeg = line.find('=') + strlen("=");
            charIDend = line.find('x', charIDbeg) - 1;

            // xbeg = line.find('=', charIDend) + strlen("=");
            xbeg = charIDend + 3;
            xend = xbeg + 6;

            // ybeg = line.find('=', xend) + strlen("=");
            ybeg = xend + 2;
            yend = ybeg + 6;

            // widthbeg = line.find('=', yend) + strlen("=");
            widthbeg = yend + 6;
            widthend = widthbeg + 6;

            // heightbeg = line.find('=', widthend) + strlen("=");
            heightbeg = widthend + 7;
            heightend = heightbeg + 6;

            // xoffsetbeg = line.find('=', heightend) + strlen("=");
            xoffsetbeg = heightend + 8;
            xoffsetend = xoffsetbeg + 6;

            // yoffsetbeg = line.find('=', xoffsetend) + strlen("=");
            yoffsetbeg = xoffsetend + 8;
            yoffsetend = yoffsetbeg + 6;

            // xadvancebeg = line.find('=', yoffsetend) + strlen("=");
            xadvancebeg = yoffsetend + 9;
            xadvanceend = xadvancebeg + 6;


            element->fontDef.charID = faster_stof(line, charIDbeg, charIDend-charIDbeg);
            element->fontDef.rect.origin.x =    faster_stof(line, xbeg, xend-xbeg);
            element->fontDef.rect.origin.y =    faster_stof(line, ybeg, yend-ybeg);
            element->fontDef.rect.size.width =  faster_stof(line, widthbeg, widthend-widthbeg);
            element->fontDef.rect.size.height = faster_stof(line, heightbeg, heightend-heightbeg);
            element->fontDef.xOffset =          faster_stof(line, xoffsetbeg, xoffsetend-xoffsetbeg);
            element->fontDef.yOffset =          faster_stof(line, yoffsetbeg, yoffsetend-yoffsetbeg);
            element->fontDef.xAdvance =         faster_stof(line, xadvancebeg, xadvanceend-xadvancebeg);

            element->key = element->fontDef.charID;
            HASH_ADD_INT(self->m_pFontDefDictionary, key, element);

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


    auto end = steady_clock::now();

    auto duration = duration_cast<nanoseconds>(end - start);

    log::info("Parsed {} in {}ns", FNTfile, duration.count());

    return charsSet;
}

/*gd::set<unsigned int>* HK__parseConfigFile(CCBMFontConfiguration* self, const char *FNTfile)
{
    auto start = steady_clock::now();

    auto pRet = reinterpret_cast<gd::set<unsigned int>*(__cdecl*)(CCBMFontConfiguration*, const char*)>(__address)(self, FNTfile);

    auto end = steady_clock::now();

    auto duration = duration_cast<nanoseconds>(end - start);

    log::info("Parsed {} in {}ms", FNTfile, duration.count());

    return pRet;
}*/

// chatFont: 1248600ns
// chirongo: 49405200ns
// chirongo: 45881000ns
// chirongo: 46889900ns
// chirongo: 19086600ns
// chirongo: 46914600ns
// chirongo: 45058800ns
// chirongo: 11317200ns
// chirongo: 9751500ns

/*$execute
{
    __address = reinterpret_cast<void*>(GetProcAddress(
        GetModuleHandle("libcocos2d.dll"), "?parseConfigFile@CCBMFontConfiguration@cocos2d@@AEAAPEAV?$set@IU?$less@I@std@@V?$allocator@I@2@@std@@PEBD@Z"
    ));

    Mod::get()->hook(
        __address,
        &HK__parseConfigFile,
        "cocos2d::CCBMFontConfiguration::initWithFNTfile",
        tulip::hook::TulipConvention::Thiscall
    );
}*/