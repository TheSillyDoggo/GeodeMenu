/*
#include <Geode/Geode.hpp>
#include <Geode/modify/CCBMFontConfiguration.hpp>

using namespace geode::prelude;
using namespace std::chrono;

void* __address = 0x0;
void* __address2 = 0x0;

gd::set<unsigned int>* HK__parseConfigFile(CCBMFontConfiguration* self, const char *FNTfile)
{
    auto start = steady_clock::now();

    auto pRet = reinterpret_cast<gd::set<unsigned int>*(__cdecl*)(CCBMFontConfiguration*, const char*)>(__address2)(self, FNTfile);

    auto end = steady_clock::now();

    auto duration = duration_cast<milliseconds>(end - start);

    log::info("Parsed {} in {}ms", FNTfile, duration.count());

    return pRet;
}

void HK__parseCharacterDefinition(CCBMFontConfiguration* self, std::string line, ccBMFontDef* charDef)
{
    //char id=0    x=53    y=61    width=1     height=1     xoffset=0     yoffset=0     xadvance=0     page=0  chnl=15
    int beg = line.find("=") + strlen("=");
    int end = line.find(' ', beg + 1);
    
    charDef->charID = std::stoi(line.substr(beg, end-beg));
}

$execute
{
    __address = reinterpret_cast<void*>(GetProcAddress(
        GetModuleHandle("libcocos2d.dll"), "?parseCharacterDefinition@CCBMFontConfiguration@cocos2d@@AEAAXV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEAU_BMFontDef@2@@Z"
    ));

    Mod::get()->hook(
        __address,
        &HK__parseCharacterDefinition,
        "cocos2d::CCBMFontConfiguration::initWithFNTfile",
        tulip::hook::TulipConvention::Thiscall
    );
}

$execute
{
    __address2 = reinterpret_cast<void*>(GetProcAddress(
        GetModuleHandle("libcocos2d.dll"), "?parseConfigFile@CCBMFontConfiguration@cocos2d@@AEAAPEAV?$set@IU?$less@I@std@@V?$allocator@I@2@@std@@PEBD@Z"
    ));

    Mod::get()->hook(
        __address2,
        &HK__parseConfigFile,
        "cocos2d::CCBMFontConfiguration::initWithFNTfile",
        tulip::hook::TulipConvention::Thiscall
    );
}
*/