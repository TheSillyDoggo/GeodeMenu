#include "../../Client/Module.hpp"
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class AutoSongDownload : public Module
{
    public:
        MODULE_SETUP(AutoSongDownload)
        {
            setID("auto-song");
            setCategory("Universal");
        }
};

SUBMIT_HACK(AutoSongDownload);

class $modify (LevelInfoLayer)
{
    bool init(GJGameLevel* p0, bool p1)
    {
        if (!LevelInfoLayer::init(p0, p1))
            return false;

        if (AutoSongDownload::get()->getRealEnabled())
        {
            Loader::get()->queueInMainThread([this] {
                if(m_songWidget && m_songWidget->m_downloadBtn && m_songWidget->m_downloadBtn->isVisible())
                    m_songWidget->onDownload(nullptr);
            });
        }

        return true;
    }
};