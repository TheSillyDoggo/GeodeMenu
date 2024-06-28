#include <Geode/Geode.hpp>
#include <Geode/modify/FMODAudioEngine.hpp>
#include <Geode/modify/MusicDownloadManager.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (FMODAudioEngine)
{
    void playEffect(gd::string p0, float p1, float p2, float p3)
    {
        if (!Client::GetModuleEnabled("fire-in-the-hole"))
            FMODAudioEngine::playEffect(p0, p1, p2, p3);
        else        
            FMODAudioEngine::playEffect("sfx/s4451.ogg", p1, p2, p3);
    }
};

class $modify (MusicDownloadManager)
{
    gd::string pathForSFX(int p0)
    {
        if (!Client::GetModuleEnabled("fire-in-the-hole"))
            return MusicDownloadManager::pathForSFX(p0);
        else
            return "sfx/s4451.ogg";
    }
};