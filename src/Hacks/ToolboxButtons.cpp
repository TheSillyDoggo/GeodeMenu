#include <Geode/Geode.hpp>
#include <Geode/modify/EditorOptionsLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (EditorOptionsLayer)
{
    void onButtonRows(cocos2d::CCObject* sender)
    {
        m_buttonRows += sender->getTag() ? 1 : -1;
        m_buttonRows = clamp<int>(m_buttonRows, 1, m_buttonRows);
        m_buttonRowsLabel->setString(fmt::format("{}", m_buttonRows).c_str());
    }

    void onButtonsPerRow(cocos2d::CCObject* sender)
    {
        m_buttonsPerRow += sender->getTag() ? 1 : -1;
        m_buttonsPerRow = clamp<int>(m_buttonsPerRow, 1, m_buttonsPerRow);
        m_buttonsPerRowLabel->setString(fmt::format("{}", m_buttonsPerRow).c_str());
    }

    QOLMOD_MOD_ALL_HOOKS("toolbox-buttons")
};