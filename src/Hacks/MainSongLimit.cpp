#include <Geode/Geode.hpp>
#include <Geode/modify/SongSelectNode.hpp>
#include <Geode/modify/MoreSearchLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (SongSelectNode)
{
    void audioNext(cocos2d::CCObject* p0)
    {
        if (Module::get("default-song-bypass")->enabled)
        {
            m_selectedSongID++;
            getLevelSettings()->m_level->m_audioTrack = m_selectedSongID;

            return SongSelectNode::updateAudioLabel();
        }

        SongSelectNode::audioNext(p0);
    }

    void audioPrevious(cocos2d::CCObject* p0)
    {
        if (Module::get("default-song-bypass")->enabled)
        {
            m_selectedSongID--;
            getLevelSettings()->m_level->m_audioTrack = m_selectedSongID;
            
            return SongSelectNode::updateAudioLabel();
        }

        SongSelectNode::audioPrevious(p0);
    }
};

class $modify (MoreSearchLayer)
{

};