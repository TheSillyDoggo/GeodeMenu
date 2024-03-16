/*#include <Geode/Geode.hpp>
#include <Geode/modify/SongSelectNode.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

#define MEMBERBYOFFSET(type, class, offset) *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(class) + offset)
#define MBO MEMBERBYOFFSET

class $modify (SongSelectNode)
{
    void audioNext(cocos2d::CCObject* p0)
    {
        int v = 0x0;

        for (size_t i = 0; i < sizeof(this) * 16; i++)
        {
            log::info("v {}: {}", v, MBO(int, this, v));

            v += 0x1;
        }

        SongSelectNode::updateAudioLabel();
    }

    void audioPrevious(cocos2d::CCObject* p0)
    {
        SongSelectNode::updateAudioLabel();
    }
};*/