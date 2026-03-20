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

    setDebugEnabled(GJAccountManager::get()->m_accountID == 16778880);

    world1 = CCNode::create();
    world2 = CCNode::create();

    world1->retain();
    world2->retain();

    world1->addChild(world2);

    debugLabel = CCLabelBMFont::create("", "chatFont.fnt");
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

void BaseDrawNode::transform(void)
{
    if (worldSpace)
    {
        auto win = CCDirector::get()->getWinSize();
        auto gjbgl = GJBaseGameLayer::get();

        if (!gjbgl)
            return;

        auto copy = gjbgl->m_debugDrawNode->getParent();
        world1->setPosition(win / 2);
        world1->setRotation(gjbgl->m_gameState.m_cameraAngle);

        world2->setPosition(-gjbgl->m_gameState.m_cameraPosition * gjbgl->m_gameState.m_cameraZoom - world1->getPosition());
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
    void processCommands(float dt, bool isHalfTick, bool isLastTick)
    {
        GJBaseGameLayer::processCommands(dt, isHalfTick, isLastTick);
    #else
    void processCommands(float dt)
    {
        GJBaseGameLayer::processCommands(dt);
    #endif
    
        __didTickThisFrame = true;

        for (auto node : baseDrawNodes)
        {
            node->onTickStart();
        }
    }

    virtual void update(float dt)
    {
        auto tick = m_gameState.m_currentProgress;

        GJBaseGameLayer::update(dt);

        if (!m_started || __didTickThisFrame)
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