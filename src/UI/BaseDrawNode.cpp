#include "BaseDrawNode.hpp"
#include <Utils.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;
using namespace qolmod;

std::vector<BaseDrawNode*> baseDrawNodes = {};

BaseDrawNode::BaseDrawNode()
{
    baseDrawNodes.push_back(this);
}

BaseDrawNode::~BaseDrawNode()
{
    world1->release();
    world2->release();
    debugLabel->release();
    baseDrawNodes.erase(std::find(
        baseDrawNodes.begin(),
        baseDrawNodes.end(),
        this
    ));
}

bool BaseDrawNode::init()
{
    if (!CCDrawNode::init())
        return false;

    setBlendFunc(ccBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA }));
    ensureCapacity(4096);

    gjbgl = GJBaseGameLayer::get();

    setDebugEnabled(GJAccountManager::get()->m_accountID == 16778880 && CCKeyboardDispatcher::get()->getAltKeyPressed());

    world1 = CCNode::create();
    world2 = CCNode::create();

    world1->retain();
    world2->retain();

    world1->addChild(world2);

    debugLabel = CCLabelBMFont::create("", "bigFont.fnt");
    debugLabel->setAnchorPoint(ccp(0, 1));
    debugLabel->setPosition(ccp(
        10,
        CCDirector::get()->getWinSize().height - 10 - (65 * (baseDrawNodes.size() - 1))
    ));
    debugLabel->setScale(0.5f);
    debugLabel->setOpacity(150);
    debugLabel->retain();
    return true;
}

void BaseDrawNode::draw()
{
    CCDrawNode::draw();

    updateDebug();
}

void BaseDrawNode::updateDebug()
{
    if (debugEnabled)
    {
        kmGLLoadIdentity();
        debugLabel->setString(fmt::format(
            "== {} ==\nVisible: {}\nPoints: {}\nCapacity: {}\nMemory: {}", 
            cocos::getObjectName(this),
            m_bVisible,
            m_nBufferCount,
            m_uBufferCapacity,
            qolmod::utils::sizeToPretty(m_uBufferCapacity * sizeof(ccV2F_C4B_T2F))
        ).c_str());
        debugLabel->visit();
    }
}

void BaseDrawNode::ensureCapacity(unsigned int count)
{
    if(m_nBufferCount + count > m_uBufferCapacity)
    {
		m_uBufferCapacity += std::max<unsigned int>(m_uBufferCapacity, count);
		m_pBuffer = (ccV2F_C4B_T2F*)realloc(m_pBuffer, m_uBufferCapacity*sizeof(ccV2F_C4B_T2F));
	}
}

void BaseDrawNode::transform(void)
{
    if (worldSpace)
    {
        auto win = CCDirector::get()->getWinSize();

        auto copy = gjbgl->m_debugDrawNode->getParent();

        if (!copy)
            return;
        
        world1->setPosition(win / 2);
        world1->setRotation(gjbgl->m_gameState.m_cameraAngle);

        // ghidra says theres another check here but im too stupid to figure it out
        // see GJBaseGameLayer::updateVisibility
        if (gjbgl->m_gameState.m_cameraAngle == 0)
            world2->setPosition(copy->getPosition() - world1->getPosition());
        else
            world2->setPosition(copy->getPosition());
        
        world2->setScaleX(copy->getScaleX());
        world2->setScaleY(copy->getScaleY());
    }

    world1->transform();
    world2->transform();
}

// reimplemented just so i can replace transform()
void BaseDrawNode::visit()
{
    if (!m_bVisible)
    {
        kmGLPushMatrix();
        updateDebug();
        kmGLPopMatrix();
        return;
    }
    
    kmGLPushMatrix();

    this->transform();
    this->draw();

    m_uOrderOfArrival = 0;

    kmGLPopMatrix();
}

void BaseDrawNode::redraw() {}
void BaseDrawNode::onTickStart() {}
void BaseDrawNode::onTickEnd() {}

void BaseDrawNode::setDebugEnabled(bool enabled)
{
    this->debugEnabled = enabled;
}

bool BaseDrawNode::isDebugEnabled()
{
    return debugEnabled;
}

void BaseDrawNode::setWorldSpace(bool world)
{
    this->worldSpace = world;
}

bool BaseDrawNode::useWorldSpace()
{
    return worldSpace;
}

void BaseDrawNode::drawSegment(const CCPoint& startPoint, const CCPoint& endPoint, float radius, const ccColor4F& color)
{
    CCPoint direction = ccpNormalize(ccpSub(endPoint, startPoint));
    CCPoint perpendicular = ccp(-direction.y, direction.x);

    CCPoint vertices[4];
    vertices[0] = ccpAdd(startPoint, ccpMult(perpendicular, radius));
    vertices[1] = ccpSub(startPoint, ccpMult(perpendicular, radius));
    vertices[2] = ccpSub(endPoint, ccpMult(perpendicular, radius));
    vertices[3] = ccpAdd(endPoint, ccpMult(perpendicular, radius));

    drawPolygon(vertices, 4, color, 0, color);
}

void BaseDrawNode::enableCull(cocos2d::CCRect area, bool outer)
{
    this->cullingEnabled = true;
    this->outerCull = outer;
    this->cullArea = area;
}

void BaseDrawNode::disableCull()
{
    this->cullingEnabled = false;
}

bool BaseDrawNode::drawPolygon(CCPoint *verts, unsigned int count, const ccColor4F &fillColor, float borderWidth, const ccColor4F &borderColor, cocos2d::BorderAlignment alignment)
{
    /*if (cullingEnabled)
    {
        if (outerCull)
        {
            for (size_t i = 0; i < count; i++)
            {
                auto& vert = verts[i];

                if (vert.y > cullArea.getMinY())
                    vert.y = cullArea.getMinY();
            }
        }
        else
        {
            for (size_t i = 0; i < count; i++)
            {
                auto& vert = verts[i];

                if (vert.x < cullArea.getMinX())
                    vert.x = cullArea.getMinX();

                if (vert.x > cullArea.getMaxX())
                    vert.x = cullArea.getMaxX();

                if (vert.y < cullArea.getMinY())
                    vert.y = cullArea.getMinY();

                if (vert.y > cullArea.getMaxY())
                    vert.y = cullArea.getMaxY();
            }
        }
    }*/

    #if GEODE_COMP_GD_VERSION >= 22081
    return CCDrawNode::drawPolygon(verts, count, fillColor, borderWidth, borderColor, alignment);
    #else
    return CCDrawNode::drawPolygon(verts, count, fillColor, borderWidth, borderColor);
    #endif
}

// Hooks

bool __isEndTickCameraUpdate = false;
bool __didTickThisFrame = false;

void redrawAll()
{
    for (auto node : baseDrawNodes)
    {
        node->redraw();
    }
}

class $modify (GJBaseGameLayer)
{
    void checkRepellPlayer()
    {
        GJBaseGameLayer::checkRepellPlayer();

        __isEndTickCameraUpdate = true;
    }

    void updateCamera(float dt)
    {
        GJBaseGameLayer::updateCamera(dt);

        if (__isEndTickCameraUpdate)
        {
            __isEndTickCameraUpdate = false;
            for (auto node : baseDrawNodes)
            {
                node->onTickEnd();
            }
        }
    }

    #if GEODE_COMP_GD_VERSION >= 22081
    void processQueuedButtons(float dt, bool clearInputQueue)
    #else
    void processQueuedButtons()
    #endif
    {
        #if GEODE_COMP_GD_VERSION >= 22081
        GJBaseGameLayer::processQueuedButtons(dt, clearInputQueue);
        #else
        GJBaseGameLayer::processQueuedButtons();
        #endif

        if (PlayLayer::get() ? !PlayLayer::get()->m_isPaused : true)
        {
            __didTickThisFrame = true;

            for (auto node : baseDrawNodes)
            {
                node->onTickStart();
            }
        }
    }

    virtual void update(float dt)
    {
        GJBaseGameLayer::update(dt);

        if (!m_started || __didTickThisFrame || m_player1->m_isDead)
        {
            __didTickThisFrame = false;
            redrawAll();
        }
    }
};

class $modify (LevelEditorLayer)
{
    virtual void updateVisibility(float dt)
    {
        LevelEditorLayer::updateVisibility(dt);

        redrawAll();
    }
};