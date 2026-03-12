#include "../../Client/Module.hpp"
#include "../Utils/PlayLayer.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

class AllModesPlatformer : public Module
{
    public:
        MODULE_SETUP(AllModesPlatformer)
        {
            setID("all-plat");
            setCategory("Level");
        }
};

SUBMIT_HACK(AllModesPlatformer);

class $modify (GJBaseGameLayer)
{
    void collisionCheckObjects(PlayerObject* p0, gd::vector<GameObject*>* p1, int p2, float p3)
    {
        GJBaseGameLayer::collisionCheckObjects(p0, p1, p2, p3);

        if (!m_isPlatformer || !AllModesPlatformer::get()->getRealEnabled())
            return;

        if (p0 && p1)
        {
            for (auto obj : *p1)
            {
                if (!obj)
                    continue;

                if (obj->m_isDecoration || obj->m_isDecoration2)
                    continue;

                if (obj->m_unk3ee)
                    continue;

                if (auto eff = typeinfo_cast<EffectGameObject*>(obj))
                {
                    if (obj->m_objectType == GameObjectType::WavePortal || obj->m_objectType == GameObjectType::SwingPortal)
                    {
                        bool intersects = false;

                        if (obj->m_orientedBox)
                            intersects = obj->m_orientedBox->overlaps(p0->m_orientedBox);
                        else
                            intersects = p0->getObjectRect().intersectsRect(obj->getObjectRect());

                        if (intersects)
                        {
                            if(this->canBeActivatedByPlayer(p0, eff))
                            {
                                this->playerWillSwitchMode(p0, obj);
                                this->switchToFlyMode(p0, obj, false, (int)(obj->m_objectType));
                                obj->playShineEffect();
                            }
                        }
                    }
                }
            }
        }
    }
};