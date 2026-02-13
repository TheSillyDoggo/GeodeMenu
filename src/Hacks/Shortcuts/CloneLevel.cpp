#include <Geode/Geode.hpp>
#include "../../Client/ButtonModule.hpp"
#include <BetterAlertLayer.hpp>
#include <LocalisationManager.hpp>
#include <Utils.hpp>

using namespace geode::prelude;

class CloneLevel : public ButtonModule
{
    public:
        MODULE_SETUP(CloneLevel)
        {
            setID("shortcut/clone-level");
            setCategory("Shortcuts");

            setPriority(sortPriority + 3);
        }

        virtual void onClick()
        {
            GJGameLevel* level = qolmod::utils::getCurrentLevel();

            if (!level)
            {
                BetterAlertLayer::createWithLocalisation("ui/title-error", "uncomplete-level/no-level-found", "ui/ok-button")->show();
                return;
            }

            auto newLevel = GameLevelManager::get()->createNewLevel();
            newLevel->copyLevelInfo(level);
            newLevel->setOriginalLevel(level->m_levelID.value());

            if (!GJBaseGameLayer::get())
            {
                CCDirector::get()->replaceScene(CCTransitionFade::create(0.5f, EditLevelLayer::scene(newLevel)));
            }
            else
            {
                BetterAlertLayer::createWithLocalisation("names/shortcut/clone-level", "clone-level/success", "ui/ok-button")->show();
            }
        }
};

SUBMIT_HACK(CloneLevel);
