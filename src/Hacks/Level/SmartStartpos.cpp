#include "../../Client/Module.hpp"
#include <Geode/modify/PlayLayer.hpp>
#include "../Utils/PlayLayer.hpp"

using namespace geode::prelude;

class SmartStartpos : public Module
{
    public:
        MODULE_SETUP(SmartStartpos)
        {
            setName("Smart Startpos");
            setID("smart-startpos");
            setCategory("Level");
            setDescription("Automatically adjusts the settings of a startpos to correctly set the gamemode, speed, gravity, etc");
            setDefaultEnabled(true);
        }
};

SUBMIT_HACK(SmartStartpos);

class $modify (PlayLayer)
{
    void createObjectsFromSetupFinished()
    {
        PlayLayer::createObjectsFromSetupFinished();

        if (!SmartStartpos::get()->getRealEnabled())
            return;

        auto self = PlayLayerUtils::getUtils()->m_fields.self();

        for (auto sp : self->startPositions)
        {
            if (auto speed = latestObjectForVector(self->speedObjects, sp))
            {
                sp->m_startSettings->m_startSpeed = speedForObjID(speed->m_objectID);
            }

            if (auto mirror = latestObjectForVector(self->mirrorObjects, sp))
            {
                sp->m_startSettings->m_mirrorMode = mirror->m_objectID == 45;
            }

            if (auto mini = latestObjectForVector(self->sizeObjects, sp))
            {
                sp->m_startSettings->m_startMini = mini->m_objectID == 101;
            }

            if (auto gamemode = latestObjectForVector(self->gamemodeObjects, sp))
            {
                sp->m_startSettings->m_startMode = gamemodeForObjID(gamemode->m_objectID);
            }

            if (auto dual = latestObjectForVector(self->dualObjects, sp))
            {
                sp->m_startSettings->m_startDual = dual->m_objectID == 286;
            }

            if (auto gravity = latestObjectForVector(self->gravityObjects, sp))
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
};