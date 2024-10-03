#include "Layout.hpp"

std::vector<int> objectIDS =
{
    // Gradient Trigger
    2903,

    // Area Tint
    3010,
    3015,
    3021,

    // Area Fade
    3009,
    3014,
    3020,

    // Ghost Triggers
    32,
    33,

    // Transitions
    22,
    23,
    24,
    25,
    26,
    27,
    28,
    55,
    56,
    57,
    58,
    59,
    1915,

    // Colour Triggers
    29,
    30,
    104,
    105,
    221,
    717,
    718,
    743,
    744,
    899,
    900,
    915,

    // BG Effect
    1818,
    1819,

    // Hide Player
    1612,
    1613,

    // Spawn Particle
    3608,

    // Edit XXX
    3029,
    3030,
    3031,

    // Setup MG
    2999,

    // XXX Speed
    3606,
    3612,

    // Pulse Trigger
    1006,

    // Alpha Trigger
    1007,

    // Shake Trigger
    1520,

    // Shaders
    2904,
    2905,
    2907,
    2909,
    2910,
    2911,
    2912,
    2913,
    2914,
    2915,
    2916,
    2917,
    2919,
    2920,
    2921,
    2922,
    2923,
    2924,
};

void LayoutPlayLayer::addObject(GameObject* object)
{
    bool dontAdd = false;
    bool addToSpecial = false;

    if (object->m_objectType == GameObjectType::Decoration)
        dontAdd = true;

    if (std::find(objectIDS.begin(), objectIDS.end(), object->m_objectID) != objectIDS.end())
        dontAdd = true;

    if (object->m_objectID == 749 || object->m_objectID == 44)
        dontAdd = false;

    if (object->m_isHide && object->m_objectType != GameObjectType::Modifier && object->m_objectType != GameObjectType::Special)
        addToSpecial = true;

    if (object->m_isNoTouch)
        dontAdd = true;

    if (object->m_groups && m_levelSettings->m_spawnGroup != 0)
    {
        if (std::find(object->m_groups->begin(), object->m_groups->end(), m_levelSettings->m_spawnGroup) != object->m_groups->end())
            dontAdd = false;
    }

    if (object->m_hasGroupParent)
        dontAdd = false;

    if (object->m_hasAreaParent)
        dontAdd = false;

    if (!dontAdd)
        PlayLayer::addObject(object);

    if (addToSpecial)
    {
        m_fields->node->addChild(object);
    }
}

bool LayoutPlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
{
    for (auto hook : showLayoutHooks)
    {
        if (Client::GetModuleEnabled("show-layout"))
            hook->enable();
        else
            hook->disable();
    }

    m_fields->node = NonRemovableNode::create();

    if (!PlayLayer::init(level, useReplay, dontCreateObjects))
        return false;

    if (Client::GetModuleEnabled("show-layout"))
    {
        m_groundLayer->setPositionY(91);
        m_groundLayer2->setPositionY(CCDirector::get()->getWinSize().height);
        m_groundLayer2->setVisible(false);
    }

    m_objectLayer->addChild(m_fields->node);
    
    return true;
}

void LayoutPlayLayer::onQuit()
{
    m_fields->node->allowDeleting = true;

    PlayLayer::onQuit();
}

void LayoutBaseGameLayer::updateColor(cocos2d::ccColor3B& color, float fadeTime, int colorID, bool blending, float opacity, cocos2d::ccHSVValue& copyHSV, int colorIDToCopy, bool copyOpacity, EffectGameObject* callerObject, int unk1, int unk2)
{
    if (colorID <= 999)
        color = ccWHITE;

    static ColourModule* bg = nullptr;
    static ColourModule* g = nullptr;

    if (!bg)
        bg = as<ColourModule*>(Client::GetModule("show-layout")->options[1]);

    if (!g)
        g = as<ColourModule*>(Client::GetModule("show-layout")->options[2]);

    if (colorID == 1000)
        color = bg->colour;

    if (colorID == 1001)
        color = g->colour;

    if (colorID == 1002)
        color = ccWHITE;

    if (colorID == 1013 || colorID == 1014)
    {
        opacity = 0;
    }

    GJBaseGameLayer::updateColor(color, fadeTime, colorID, blending, opacity, copyHSV, colorIDToCopy, copyOpacity, callerObject, unk1, unk2);
}


void LayoutBaseGameLayer::createBackground(int p0)
{
    if (typeinfo_cast<PlayLayer*>(this))
        p0 = 0;

    GJBaseGameLayer::createBackground(p0);
}

void LayoutBaseGameLayer::createGroundLayer(int p0, int p1)
{
    if (typeinfo_cast<PlayLayer*>(this))
    {
        p0 = 0;
        p1 = 1;
    }

    GJBaseGameLayer::createGroundLayer(p0, p1);
}

void LayoutBaseGameLayer::createMiddleground(int p0)
{
    if (typeinfo_cast<PlayLayer*>(this))
        p0 = 0;

    GJBaseGameLayer::createMiddleground(p0);
}