#include "PaintNode.hpp"
#include "PaintControl.hpp"
#include "PaintControl.hpp"

using namespace geode::prelude; 
using namespace qolmod;

PaintNode* PaintNode::get()
{
    static Ref<PaintNode> instance = nullptr;

    if (!instance)
    {
        instance = new PaintNode();
        instance->init();
    }

    return instance;
}

bool PaintNode::init()
{
    auto win = CCDirector::get()->getWinSize();

    rTex = CCRenderTexture::create(win.width, win.height, kCCTexture2DPixelFormat_RGBA8888);
    rTex->getSprite()->setAnchorPoint(ccp(0, 1));

    this->addChild(rTex);
    return true;
}

void PaintNode::drawPixel(int x, int y, ccColor4F col)
{
    CCPoint points[] = {
        ccp(x, y),
        ccp(x, y + 1),
        ccp(x + 1, y + 1),
        ccp(x + 1, y),
    };

    float mult = 1.0f / CCDirector::get()->getWinSize().width * rTex->m_nWidth;

    rTex->begin();

    glEnable(GL_SCISSOR_TEST);

    glScissor(x * mult, y * mult, mult, mult);
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    ccDrawSolidPoly(points, 4, col);

    rTex->end();
}

void PaintNode::paintPos(CCPoint position, ccColor3B colour)
{
    std::pair<int, int> pos = { (int)position.x, (int)position.y };

    drawPixel(pos.first, pos.second, ccc4FFromccc3B(colour));
}

static std::unordered_map<int, std::vector<std::pair<int,int>>> brushCache;

// stolen
static const std::vector<std::pair<int,int>>& getBrush(int size)
{
    auto it = brushCache.find(size);
    if (it != brushCache.end())
        return it->second;

    auto& points = brushCache[size];
    int r = size / 2;

    for (int y = -r; y <= r; y++)
    {
        int dx = (int)std::floor(std::sqrt(r * r - y * y));
        for (int x = -dx; x <= dx; x++)
        {
            points.emplace_back(x, y);
        }
    }

    return points;
}

void PaintNode::paintBrush(cocos2d::CCPoint position, cocos2d::ccColor3B colour, int size, bool erase)
{
    std::vector<std::pair<int,int>> points = getBrush(size);

    for (auto point : points)
    {
        if (erase)
            erasePos(position + ccp(point.first, point.second));
        else
            paintPos(position + ccp(point.first, point.second), colour);
    }
}

void PaintNode::erasePos(CCPoint position)
{
    std::pair<int, int> pos = { (int)position.x, (int)position.y };

    drawPixel(pos.first, pos.second, ccc4f(0, 0, 0, 0));
}

CCDrawNode* PaintNode::getPreview(int size)
{
    if (!cursorPreviews.contains(size))
    {
        auto n = CCDrawNode::create();
        n->setBlendFunc(rTex->getSprite()->getBlendFunc());

        auto brush = getBrush(size);

        for (auto point : brush)
        {
            int x = point.first;
            int y = point.second;

            CCPoint points[] = {
                ccp(x, y),
                ccp(x, y + 1),
                ccp(x + 1, y + 1),
                ccp(x + 1, y),
            };

            n->drawPolygon(points, 4, ccc4f(1, 1, 1, 0.3f), 0, ccc4f(1, 1, 1, 0.5f));
        }

        /*for (auto point : brush)
        {
            // log::error("point: {}, {}", point.first, point.second);
            points.push_back(ccp(point.first, point.second));
        }*/
        
        // n->drawPolygon(points.data(), points.size(), ccc4f(1, 0, 0, 1), 0, ccc4f(1, 1, 1, 0.5f));

        cursorPreviews[size] = n;
    }

    return cursorPreviews[size];
}

void PaintNode::visit()
{
    auto size = PaintControl::get()->getSelectedSize();

    if (CCKeyboardDispatcher::get()->getShiftKeyPressed())
        paintBrush(getMousePos(), PaintControl::get()->getColour(), size);

    if (CCKeyboardDispatcher::get()->getAltKeyPressed())
        paintBrush(getMousePos(), ccWHITE, size, true);

    CCNode::visit();

    getPreview(size)->setPosition(ccp((int)getMousePos().x, (int)getMousePos().y));
    getPreview(size)->visit();
}