#include "PlayLayer.hpp"

PlayLayerUtils* PlayLayerUtils::getUtils()
{
    return static_cast<PlayLayerUtils*>(get());
}

bool PlayLayerUtils::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects)
{
    if (!PlayLayer::init(level, useReplay, dontCreateObjects))
        return false;

    m_fields->drawNode = CCDrawNode::create();
    m_objectLayer->addChild(m_fields->drawNode);

    return true;
}

void PlayLayerUtils::postUpdate(float p0)
{
    PlayLayer::postUpdate(p0);

    m_fields->drawNode->clear();
}

void PlayLayerUtils::addObject(GameObject* obj)
{
    PlayLayer::addObject(obj);

    if (obj->m_objectType == GameObjectType::UserCoin || obj->m_objectType == GameObjectType::SecretCoin)
    {
        m_fields->coins.push_back(static_cast<EffectGameObject*>(obj));
    }

    switch (obj->m_objectID)
    { //thanks prevter :3
        case 10: // Blue Gravity Portal
        case 11: // Yellow Gravity Portal
        case 2926: // Green Gravity Portal
            m_fields->gravityObjects.push_back(obj);
            break;
        case 12: // Cube Portal
        case 13: // Ship Portal
        case 47: // Ball Portal
        case 111: // UFO Portal
        case 660: // Wave Portal
        case 745: // Robot Portal
        case 1331: // Spider Portal
        case 1933: // SwingCopter Portal
            m_fields->gamemodeObjects.push_back(obj);
            break;
        case 286: // Orange Dual Portal
        case 287: // Blue Dual Portal
            m_fields->dualObjects.push_back(obj);
            break;
        case 99: // Normal Size Portal
        case 101: // Mini Portal
            m_fields->sizeObjects.push_back(obj);
            break;
        case 45: // Orange Mirror Portal
        case 46: // Blue Mirror Portal
            m_fields->mirrorObjects.push_back(obj);
            break;
        case 200: // -1x Speed Change
        case 201: // 1x Speed Change
        case 202: // 2x Speed Change
        case 203: // 3x Speed Change
        case 1334: // 4x Speed Change
            m_fields->speedObjects.push_back(obj);
            break;
        case 31: // startpos
            m_fields->startPositions.push_back(static_cast<StartPosObject*>(obj));
            break;
        default:
            return;
    }
}

void PlayLayerUtils::createObjectsFromSetupFinished()
{
    sortByXPos();

    PlayLayer::createObjectsFromSetupFinished();
}

#define SORT_FIELD(field) \
std::sort(field.begin(), field.end(), [](auto* a, auto* b) { return a->getPositionX() < b->getPositionX(); });

void PlayLayerUtils::sortByXPos()
{
    auto self = m_fields.self();

    SORT_FIELD(self->coins);
    SORT_FIELD(self->startPositions);
    SORT_FIELD(self->speedObjects);
    SORT_FIELD(self->mirrorObjects);
    SORT_FIELD(self->sizeObjects);
    SORT_FIELD(self->dualObjects);
    SORT_FIELD(self->gamemodeObjects);
    SORT_FIELD(self->gravityObjects);
}

void PlayLayerUtils::onModify(auto& self)
{
    (void)self.setHookPriorityPost("PlayLayer::addObject", Priority::First);
    (void)self.setHookPriorityPost("PlayLayer::postUpdate", Priority::First);
}