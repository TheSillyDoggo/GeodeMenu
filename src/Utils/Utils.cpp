#include "Utils.hpp"

using namespace geode::prelude;

GJGameLevel* qolmod::utils::getCurrentLevel()
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

        if (auto select = scene->getChildByType<LevelSelectLayer>(0))
        {
            if (auto page = typeinfo_cast<LevelPage*>(select->m_scrollLayer->getPage(select->m_scrollLayer->m_page)))
            {
                return page->m_level;
            }
        }
    }

    return nullptr;
}