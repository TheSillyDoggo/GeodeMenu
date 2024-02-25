#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (LevelInfoLayer)
{
    bool init(GJGameLevel* p0, bool p1)
    {
        if (!LevelInfoLayer::init(p0, p1))
            return false;

        if (Client::GetModuleEnabled("auto-song"))
        {
            Loader::get()->queueInMainThread([this] {
                if(m_songWidget && m_songWidget->m_downloadBtn->isVisible())
                    m_songWidget->onDownload(nullptr);
                }
            );
        }

        return true;
    }
};