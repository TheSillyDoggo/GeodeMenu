#include <Geode/Geode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/CCDrawNode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/HardStreak.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

Module* noWave = nullptr;
Module* solidWave = nullptr;

class $modify (CCDrawNode)
{
    bool drawPolygon(CCPoint *verts, unsigned int count, const ccColor4F &fillColor, float borderWidth, const ccColor4F &borderColor)
    {
        if (typeinfo_cast<HardStreak*>(this))
        {
            if (!noWave)
                noWave = Client::GetModule("no-wave");

            if (!solidWave)
                solidWave = Client::GetModule("solid-wave");

            if (noWave->enabled)
                return true;

            if (solidWave->enabled)
            {
                if (fillColor.r >= 1.0f && fillColor.g >= 1.0f && fillColor.b >= 1.0f && this->getColor() != ccc3(255, 255, 255))
                    return true;

                if (this->getTag() != 1)
                {
                    this->setTag(1);
                    this->setBlendFunc(CCSprite::create()->getBlendFunc());
                }

                this->setZOrder(-1);
            }
        }

        return CCDrawNode::drawPolygon(verts, count, fillColor, borderWidth, borderColor);
    }
};