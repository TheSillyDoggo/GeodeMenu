#ifndef GEODE_IS_MACOS

#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* thicker = nullptr;
Module* fill = nullptr;
SliderModule* fillOpacity = nullptr;

ColourModule* solid = nullptr;
ColourModule* hazard = nullptr;
ColourModule* passable = nullptr;
ColourModule* interact = nullptr;
ColourModule* player = nullptr;
ColourModule* playerRot = nullptr;

bool myDrawPoly(CCDrawNode* ins, CCPoint *verts, unsigned int count, const ccColor4F &fillColor, float borderWidth, /*const */ccColor4F &borderColor) {
    if (!thicker)
        thicker = Client::GetModule("show-hitboxes")->options[7];

    if (!fill)
        fill = Client::GetModule("show-hitboxes")->options[8];

    if (!fillOpacity)
        fillOpacity = as<SliderModule*>(Client::GetModule("show-hitboxes")->options[9]);

    if (!solid)
        solid = as<ColourModule*>(Client::GetModule("show-hitboxes")->options[0]);

    if (!hazard)
        hazard = as<ColourModule*>(Client::GetModule("show-hitboxes")->options[1]);

    if (!passable)
        passable = as<ColourModule*>(Client::GetModule("show-hitboxes")->options[2]);

    if (!interact)
        interact = as<ColourModule*>(Client::GetModule("show-hitboxes")->options[3]);

    if (!player)
        player = as<ColourModule*>(Client::GetModule("show-hitboxes")->options[4]);

    if (!playerRot)
        playerRot = as<ColourModule*>(Client::GetModule("show-hitboxes")->options[5]);

    if (ins->getTag() == -9999)
    {
        if (borderColor.r == 0 && borderColor.g == 0.25f && borderColor.b == 1)
            borderColor = ccc4FFromccc3B(solid->colour);

        else if (borderColor.r == 1 && borderColor.g == 0 && borderColor.b == 0)
            borderColor = ccc4FFromccc3B(hazard->colour);

        else if (borderColor.r == 0 && borderColor.g == 1 && borderColor.b == 0)
            borderColor = ccc4FFromccc3B(interact->colour);

        else if (borderColor.r == 0 && borderColor.g == 1 && borderColor.b == 1)
            borderColor = ccc4FFromccc3B(passable->colour);

        else if (borderColor.r == 1 && borderColor.g == 1 && borderColor.b == 0)
            borderColor = ccc4FFromccc3B(playerRot->colour);

        else if (borderColor.r == -1 && borderColor.g == -1 && borderColor.b == -1)
            borderColor = ccc4FFromccc3B(player->colour);

        auto c = borderColor;

        c.a = fillOpacity->value;

        if (borderWidth == 0)
            borderWidth = 1;

        return ins->drawPolygon(verts, count, fill->enabled ? c : fillColor, borderWidth * (thicker->enabled ? 2.2f : 1), borderColor);
    }
    else
        return ins->drawPolygon(verts, count, fillColor, borderWidth, borderColor);
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

class $modify (GJBaseGameLayer)
{
    void drawForPlayer(PlayerObject* po)
    {
        CCPoint squareSize = po->getObjectRect().size;
        CCPoint squarePosition = po->getPosition();

        CCPoint squareVertices[] = {
            ccp(squarePosition.x - squareSize.x / 2, squarePosition.y - squareSize.y / 2), // Bottom-left
            ccp(squarePosition.x + squareSize.x / 2, squarePosition.y - squareSize.y / 2), // Bottom-right
            ccp(squarePosition.x + squareSize.x / 2, squarePosition.y + squareSize.y / 2), // Top-right
            ccp(squarePosition.x - squareSize.x / 2, squarePosition.y + squareSize.y / 2)  // Top-left
        };

        m_debugDrawNode->drawPolygon(squareVertices, 4, ccc4f(0, 0, 0, 0), 0.35f, ccc4f(-1, -1, -1, -1));

        m_debugDrawNode->drawCircle(squarePosition, po->getObjectRect().size.width / 2, ccc4f(0, 0, 0, 0), 0.35f, ccc4f(-1, -1, -1, -1), 64);

        CCPoint squareSize2 = po->getObjectRect(0.25f, 0.25f).size;

        CCPoint squareVertices2[] = {
            ccp(squarePosition.x - squareSize2.x / 2, squarePosition.y - squareSize2.y / 2), // Bottom-left
            ccp(squarePosition.x + squareSize2.x / 2, squarePosition.y - squareSize2.y / 2), // Bottom-right
            ccp(squarePosition.x + squareSize2.x / 2, squarePosition.y + squareSize2.y / 2), // Top-right
            ccp(squarePosition.x - squareSize2.x / 2, squarePosition.y + squareSize2.y / 2)  // Top-left
        };

        m_debugDrawNode->drawPolygon(squareVertices2, 4, ccc4f(0, 0, 0, 0), 0.35f, ccc4f(0, 0.25f, 1, 1));
    }

    virtual void updateDebugDraw()
    {
        GJBaseGameLayer::updateDebugDraw();

        //if (!player)
            //player = as<ColourModule*>(Client::GetModule("show-hitboxes")->options[4]);

        drawForPlayer(m_player1);

        if (m_player2)
            drawForPlayer(m_player2);
    }
};

#endif