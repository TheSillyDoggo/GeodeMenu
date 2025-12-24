#include "../../Client/Module.hpp"
#include <Geode/modify/PlayLayer.hpp>
#include "../Utils/PlayLayer.hpp"

using namespace geode::prelude;

class PracticeComplete : public Module
{
    public:
        MODULE_SETUP(PracticeComplete)
        {
            setName("1 Attempt Practice");
            setID("practice-complete");
            setCategory("Level");
            setDescription("Allows you to complete a level in practice mode if you beat it in <cr>one</c> attempt");
            setDefaultEnabled(true);
        }
};

SUBMIT_HACK(PracticeComplete);

class $modify (PlayLayer)
{
    struct Fields
    {
        bool hasRespawnedWithCheckpoint;
    };

    void resetLevel()
    {
        m_fields->hasRespawnedWithCheckpoint = false;

        PlayLayer::resetLevel();        
    }

    void loadFromCheckpoint(CheckpointObject* cp)
    {
        m_fields->hasRespawnedWithCheckpoint = true;

        PlayLayer::loadFromCheckpoint(cp);
    }

    void levelComplete()
    {
        if (PracticeComplete::get()->getRealEnabled())
        {
            if (m_isPracticeMode)
            {
                m_isPracticeMode = m_fields->hasRespawnedWithCheckpoint;

                if (!m_isPracticeMode)
                {
                    for (auto coin : PlayLayerUtils::getUtils()->m_fields->coins)
                    {
                        if (coin->hasBeenActivated())
                        {
                            pickupItem(static_cast<EffectGameObject*>(coin));
                        }
                    }
                }
            }
        }

        PlayLayer::levelComplete();
    }
};