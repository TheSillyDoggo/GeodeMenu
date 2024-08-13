#ifndef GEODE_IS_MACOS

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/UILayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (SmartPlayLayer, PlayLayer)
{
    struct Fields {
        std::vector<GameObject*> speedObjects;
        std::vector<GameObject*> mirrorObjects;
        std::vector<GameObject*> sizeObjects;
        std::vector<GameObject*> dualObjects;
        std::vector<GameObject*> gamemodeObjects;
        std::vector<GameObject*> gravityObjects;
        std::vector<StartPosObject*> startposObjects;
    };

    void addObject(GameObject* object)
    {
        PlayLayer::addObject(object);

        switch (object->m_objectID) { //thanks prevter :3
            case 10: // Blue Gravity Portal
            case 11: // Yellow Gravity Portal
            case 2926: // Green Gravity Portal
                m_fields->gravityObjects.push_back(object);
                break;
            case 12: // Cube Portal
            case 13: // Ship Portal
            case 47: // Ball Portal
            case 111: // UFO Portal
            case 660: // Wave Portal
            case 745: // Robot Portal
            case 1331: // Spider Portal
            case 1933: // SwingCopter Portal
                m_fields->gamemodeObjects.push_back(object);
                break;
            case 286: // Orange Dual Portal
            case 287: // Blue Dual Portal
                m_fields->dualObjects.push_back(object);
                break;
            case 99: // Normal Size Portal
            case 101: // Mini Portal
                m_fields->sizeObjects.push_back(object);
                break;
            case 45: // Orange Mirror Portal
            case 46: // Blue Mirror Portal
                m_fields->mirrorObjects.push_back(object);
                break;
            case 200: // -1x Speed Change
            case 201: // 1x Speed Change
            case 202: // 2x Speed Change
            case 203: // 3x Speed Change
            case 1334: // 4x Speed Change
                m_fields->speedObjects.push_back(object);
                break;
            case 31:
                m_fields->startposObjects.push_back(as<StartPosObject*>(object));
                break;
            default:
                return;
        }
    }

    void createObjectsFromSetupFinished()
    {
        PlayLayer::createObjectsFromSetupFinished();

        std::sort(m_fields->speedObjects.begin(), m_fields->speedObjects.end(), [](auto* a, auto* b) { return a->getPositionX() < b->getPositionX(); });
        std::sort(m_fields->mirrorObjects.begin(), m_fields->mirrorObjects.end(), [](auto* a, auto* b) { return a->getPositionX() < b->getPositionX(); });
        std::sort(m_fields->sizeObjects.begin(), m_fields->sizeObjects.end(), [](auto* a, auto* b) { return a->getPositionX() < b->getPositionX(); });
        std::sort(m_fields->dualObjects.begin(), m_fields->dualObjects.end(), [](auto* a, auto* b) { return a->getPositionX() < b->getPositionX(); });
        std::sort(m_fields->gamemodeObjects.begin(), m_fields->gamemodeObjects.end(), [](auto* a, auto* b) { return a->getPositionX() < b->getPositionX(); });
        std::sort(m_fields->gravityObjects.begin(), m_fields->gravityObjects.end(), [](auto* a, auto* b) { return a->getPositionX() < b->getPositionX(); });

        for (auto sp : m_fields->startposObjects)
        {
            if (auto speed = latestObjectForVector(m_fields->speedObjects, sp))
            {
                sp->m_startSettings->m_startSpeed = speedForObjID(speed->m_objectID);
            }

            if (auto mirror = latestObjectForVector(m_fields->mirrorObjects, sp))
            {
                sp->m_startSettings->m_mirrorMode = mirror->m_objectID == 45;
            }

            if (auto mini = latestObjectForVector(m_fields->sizeObjects, sp))
            {
                sp->m_startSettings->m_startMini = mini->m_objectID == 101;
            }

            if (auto gamemode = latestObjectForVector(m_fields->gamemodeObjects, sp))
            {
                sp->m_startSettings->m_startMode = gamemodeForObjID(gamemode->m_objectID);
            }

            if (auto dual = latestObjectForVector(m_fields->dualObjects, sp))
            {
                sp->m_startSettings->m_startDual = dual->m_objectID == 286;
            }

            if (auto gravity = latestObjectForVector(m_fields->gravityObjects, sp))
            {
                //sp->m_startSettings->m_isFlipped = getFlippedGravity(m_fields->gravityObjects, sp);
            }
        }
    }

    GameObject* latestObjectForVector(std::vector<GameObject*> vector, GameObject* startpos)
    {
        GameObject* obj = nullptr;

        for (auto o : vector)
        {
            if (o->m_positionX > startpos->m_positionX)
                return obj;

            obj = o;
        }

        return obj;
    }

    bool getFlippedGravity(std::vector<GameObject*> vector, GameObject* startpos)
    {
        bool flipped;

        for (auto o : vector)
        {
            if (o->m_positionX > startpos->m_positionX)
                return flipped;

            if (o->m_objectID == 10)
                flipped = false;

            if (o->m_objectID == 11)
                flipped = true;

            if (o->m_objectID == 2926)
                flipped = !flipped;
        }

        return flipped;
    }

    Speed speedForObjID(int objID)
    {
        switch (objID)
        {
            case 200:
                return Speed::Slow;
            default:
                return Speed::Normal;
            case 202:
                return Speed::Fast;
            case 203:
                return Speed::Faster;
            case 1334:
                return Speed::Fastest;
        }
    }

    int gamemodeForObjID(int objID)
    {
        switch (objID)
        {
            default: return 0;
            case 13: return 1;
            case 47: return 2;
            case 111: return 3;
            case 660: return 4;
            case 745: return 5;
            case 1331: return 6;
            case 1933: return 7;
        }
    }

    QOLMOD_MOD_HOOK("smart-startpos", "PlayLayer::createObjectsFromSetupFinished")
};

#endif