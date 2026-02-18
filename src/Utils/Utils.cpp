#include "Utils.hpp"
#include <codecvt>

using namespace geode::prelude;

GJGameLevel* qolmod::utils::getCurrentLevel(bool requireLevelData)
{
    if (auto gjbgl = GJBaseGameLayer::get())
        return gjbgl->m_level;

    if (auto scene = CCScene::get())
    {
        if (auto info = scene->getChildByType<LevelInfoLayer>(0))
        {
            return info->m_level;
        }

        if (auto edit = scene->getChildByType<EditLevelLayer>(0))
        {
            return edit->m_level;
        }

        if (!requireLevelData)
        {
            if (auto select = scene->getChildByType<LevelSelectLayer>(0))
            {
                if (auto page = typeinfo_cast<LevelPage*>(select->m_scrollLayer->getPage(select->m_scrollLayer->m_page)))
                {
                    return page->m_level;
                }
            }
        }
    }

    return nullptr;
}

std::wstring qolmod::utils::toWideString(std::string str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring ws = converter.from_bytes(str);

    return ws;
}

std::string qolmod::utils::toUTF8String(std::wstring ws)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string str = converter.to_bytes(ws);

    return str;
}