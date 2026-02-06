#include <Geode/Geode.hpp>
#include "../../Client/ButtonModule.hpp"
#include <BetterAlertLayer.hpp>

using namespace geode::prelude;

class UncompleteLevel : public ButtonModule, public FLAlertLayerProtocol
{
    public:
        MODULE_SETUP(UncompleteLevel)
        {
            setID("shortcut/uncomplete-level");
            setCategory("Universal");

            setPriority(sortPriority + 2);
        }

        virtual void onClick()
        {
            GJGameLevel* level = nullptr;

            if (auto gjbgl = GJBaseGameLayer::get())
                level = gjbgl->m_level;

            if (auto scene = CCScene::get())
            {
                if (auto info = scene->getChildByType<LevelInfoLayer>(0))
                {
                    level = info->m_level;
                }
            }

            if (!level)
            {
                BetterAlertLayer::createWithLocalisation("ui/title-error", "uncomplete-level/no-level-found", "ui/ok-button")->show();
                return;
            }

            auto alert = BetterAlertLayer::createWithLocalisation(this, "names/shortcut/uncomplete-level", "uncomplete-level/found-confirmation", "ui/cancel-button", "ui/confirm-button", 320, false, 280, 1.0f);
            alert->setUserData(level);
            alert->show();
        }

        virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2)
        {
            GJGameLevel* level = reinterpret_cast<GJGameLevel*>(layer->getUserData());

            if (btn2)
            {
                level->m_practicePercent = 0;
                level->m_normalPercent = 0;
                level->m_newNormalPercent2 = 0;
                level->m_orbCompletion = 0;
                level->m_platformerSeed = 0;
                level->m_bestPoints = 0;
                level->m_bestTime = 0;

                auto key = GameLevelManager::get()->getLevelKey(level->m_levelID.value());
                GameStatsManager::get()->m_completedLevels->removeObjectForKey(key);
                GameStatsManager::get()->m_completedLevels->removeObjectForKey(fmt::format("demon_{}", level->m_levelID.value()));
                GameStatsManager::get()->m_completedLevels->removeObjectForKey(std::string(GameStatsManager::get()->getStarLevelKey(level)));

                if (level->isPlatformer())
                {
                    GameStatsManager::get()->setStat("28", GameStatsManager::get()->getStat("28") - level->m_stars);
                }
                else
                {
                    GameStatsManager::get()->setStat("6", GameStatsManager::get()->getStat("6") - level->m_stars);
                }

                if (level->m_demon > 0)
                {
                    GameStatsManager::get()->setStat("5", GameStatsManager::get()->getStat("5") - 1);
                }


                GameLevelManager::get()->saveLevel(level);
            }
        }
};

SUBMIT_HACK(UncompleteLevel);