#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* accurateMod = nullptr;
Module* inaccurateMod = nullptr;
InputModule* accurateModPlaces = nullptr;

class $modify (PlayLayer)
{
    CCLabelBMFont* title;

    float getPerc()
    {
        return inaccurateMod->enabled ? ((m_player1->getPositionX() / (m_levelLength == 0 ? 1 : m_levelLength))) * 100 : this->getCurrentPercent();
    }

    void updateProgressbar()
    {
        PlayLayer::updateProgressbar();

        if (!m_fields->title)
        {
            m_fields->title = getChildOfType<CCLabelBMFont>(this, 0);

            if (!(std::string(m_fields->title->getString())).ends_with("%"))
                m_fields->title = nullptr;
        }

        if (!m_fields->title)
            return;

        if (!accurateMod)
            accurateMod = Client::GetModule("accurate-percentage");

        if (!inaccurateMod)
            inaccurateMod = Client::GetModule("classic-percentage");

        if (!accurateModPlaces)
            accurateModPlaces = as<InputModule*>(accurateMod->options[0]);

        if (accurateMod->enabled || inaccurateMod->enabled)
        {
            int places = 2;

            places = accurateModPlaces->getIntValue();

            if (!accurateMod->enabled)
                places = GameManager::sharedState()->getGameVariable("0126") ? 2 : 0;

            m_fields->title->setString((utils::numToString<float>(getPerc(), places) + std::string("%")).c_str());
        }
    }
};