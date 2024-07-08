#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

#ifdef QOLMOD_AUTOSONG

class $modify (LevelInfoLayer)
{
    bool init(GJGameLevel* p0, bool p1)
    {
        if (!LevelInfoLayer::init(p0, p1))
            return false;

        Loader::get()->queueInMainThread([this] {
            if(m_songWidget && m_songWidget->m_downloadBtn && m_songWidget->m_downloadBtn->isVisible())
                m_songWidget->onDownload(nullptr);
        });

        return true;
    }

    QOLMOD_MOD_HOOK("auto-song", "LevelInfoLayer::init")
};

#endif