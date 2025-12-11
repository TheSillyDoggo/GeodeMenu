#include "ShowHitboxes.hpp"
#include "HitboxColours.hpp"
#include "../HitboxUtils.hpp"
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/CCDrawNode.hpp>

SUBMIT_HACK(ShowHitboxes);
SUBMIT_HACK(ShowHitboxesOnDeath);

SUBMIT_OPTION(ShowHitboxes, HitboxSolid);
SUBMIT_OPTION(ShowHitboxes, HitboxHazard);
SUBMIT_OPTION(ShowHitboxes, HitboxPassable);
SUBMIT_OPTION(ShowHitboxes, HitboxInteractable);
SUBMIT_OPTION(ShowHitboxes, HitboxPlayer);
SUBMIT_OPTION(ShowHitboxes, HitboxPlayerRot);

SUBMIT_OPTION(ShowHitboxes, HitboxThickOutline);
SUBMIT_OPTION(ShowHitboxes, HitboxFill);
SUBMIT_OPTION(ShowHitboxes, HitboxFillOpacity);

class $modify (PlayLayer)
{
    virtual void postUpdate(float dt)
    {
        PlayLayer::postUpdate(dt);

        m_debugDrawNode->setVisible(HitboxUtils::shouldHitboxesBeVisible() || (ShowHitboxesOnDeath::get()->getRealEnabled() ? m_player1->m_isDead : false) || ShowHitboxes::get()->getRealEnabled());

        if (m_debugDrawNode->isVisible() != HitboxUtils::shouldHitboxesBeVisible())
            updateDebugDraw();
    }
};

class $modify (HitboxBaseGameLayer, GJBaseGameLayer)
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

        // m_debugDrawNode->drawCircle(squarePosition, po->getObjectRect().size.width / 2, ccc4f(0, 0, 0, 0), 0.35f, ccc4f(-1, -1, -1, -1), 64);

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

        drawForPlayer(m_player1);

        if (m_player2 && m_player2->isRunning())
            drawForPlayer(m_player2);
    }
};

class $modify (CCDrawNode)
{
    bool drawPolygon(CCPoint *verts, unsigned int count, const ccColor4F &fillColor, float borderWidth, const ccColor4F &borderColor)
    {
        if (PlayLayer::get() && PlayLayer::get()->m_debugDrawNode == this)
        {
            auto border = borderColor;

            if (borderColor.r == 0 && borderColor.g == 0.25f && borderColor.b == 1)
                border = ccc4FFromccc3B(HitboxSolid::get()->getColour());

            else if (borderColor.r == 1 && borderColor.g == 0 && borderColor.b == 0)
                border = ccc4FFromccc3B(HitboxHazard::get()->getColour());

            else if (borderColor.r == 0 && borderColor.g == 1 && borderColor.b == 0)
                border = ccc4FFromccc3B(HitboxInteractable::get()->getColour());

            else if (borderColor.r == 0 && borderColor.g == 1 && borderColor.b == 1)
                border = ccc4FFromccc3B(HitboxPassable::get()->getColour());

            else if (borderColor.r == 1 && borderColor.g == 1 && borderColor.b == 0)
                border = ccc4FFromccc3B(HitboxPlayerRot::get()->getColour());

            else if (borderColor.r == -1 && borderColor.g == -1 && borderColor.b == -1)
                border = ccc4FFromccc3B(HitboxPlayer::get()->getColour());

            auto fill = border;
            fill.a = HitboxFillOpacity::get()->getValue();

            if (borderWidth == 0)
                borderWidth = 1;

            return CCDrawNode::drawPolygon(verts, count, HitboxFill::get()->getRealEnabled() ? fill : fillColor, borderWidth * (HitboxThickOutline::get()->getRealEnabled() ? 2 : 1), border);
        }

        return CCDrawNode::drawPolygon(verts, count, fillColor, borderWidth, borderColor);
    }
};