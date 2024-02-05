#include <Geode/Geode.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

bool myDrawCircle(CCDrawNode* ins, CCPoint *verts, unsigned int count, const ccColor4F &fillColor, float borderWidth, const ccColor4F &borderColor) {
    if (Client::GetModuleEnabled("no-wave"))
        return true;

    if (Client::GetModuleEnabled("solid-wave"))
    {
        if (fillColor.r >= 1.0f && fillColor.g >= 1.0f && fillColor.b >= 1.0f)
            return true;

        ins->setBlendFunc(CCSprite::create()->getBlendFunc());

        ins->setZOrder(-1);
    }

    return ins->drawPolygon(verts, count, fillColor, borderWidth, borderColor);
}

$execute {
    Mod::get()->hook(
        reinterpret_cast<void*>(
            geode::addresser::getNonVirtual(&HardStreak::drawPolygon)
        ),
        &myDrawCircle,
        "cocos2d::CCDrawNode::drawPolygon",
        tulip::hook::TulipConvention::Thiscall
    );
}