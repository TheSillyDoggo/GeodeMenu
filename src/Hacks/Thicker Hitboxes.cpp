#ifndef GEODE_IS_MACOS

#include <Geode/Geode.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* thicker = nullptr;
Module* fill = nullptr;

bool myDrawPoly(CCDrawNode* ins, CCPoint *verts, unsigned int count, const ccColor4F &fillColor, float borderWidth, const ccColor4F &borderColor) {
    if (!thicker)
        thicker = Client::GetModule("thick-hitbox");

    if (!fill)
        fill = Client::GetModule("fill-hitbox");

    return ins->drawPolygon(verts, count, fill->enabled ? borderColor : fillColor, borderWidth * ((thicker->enabled && (ins->getTag() == -9999)) ? 2.2f /*2.2?! HOLY SHIT IS THAT A MOTHERFUCKING GEOMETRY DASH REFERENCE*/ : 1), borderColor);
}

$execute {
    Mod::get()->hook(
        reinterpret_cast<void*>(
            geode::addresser::getNonVirtual(&CCDrawNode::drawPolygon)
        ),
        &myDrawPoly,
        "cocos2d::CCDrawNode::drawPolygon",
        tulip::hook::TulipConvention::Thiscall
    );
}

#endif