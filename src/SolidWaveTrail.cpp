//#include <Geode/Geode.hpp>
//#include <Geode/modify/CCDrawNode.hpp>
//#include <Geode/modify/PlayerObject.hpp>
//#include "Client/Client.h"

//using namespace geode::prelude;

/*class SolidWaveTrail
{
    public:
        static inline bool hstreak = false;
};

class $modify (CCDrawNode)
{
    void drawPolygon(CCPoint * verts, unsigned int count, const ccColor4F & fillColor, float borderWidth, const ccColor4F & borderColor)
    {
        if (PlayLayer::get())
        {
            CCDrawNode::drawPolygon(verts, count, fillColor, borderWidth * 10, fillColor);
        }
        else
        {
            CCDrawNode::drawPolygon(verts, count, fillColor, borderWidth, borderColor);
        }
    }
};

class $modify(PlayerObject)
{
    virtual void update(float dt)
    {
        SolidWaveTrail::hstreak = true;
        PlayerObject::update(dt);

        if (this->m_waveTrail)
        {
            this->m_waveTrail->setOpacity(255);
            this->m_waveTrail->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
        }
    }
};*/