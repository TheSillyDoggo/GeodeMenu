#include <Geode/Geode.hpp>
#include "../../Client/ButtonModule.hpp"
#include <BetterAlertLayer.hpp>
#include <LocalisationManager.hpp>

using namespace geode::prelude;

class UncompleteLevel : public ButtonModule, public FLAlertLayerProtocol
{
    public:
        MODULE_SETUP(UncompleteLevel)
        {
            setID("shortcut/uncomplete-level");
            setCategory("Shortcuts");

            setPriority(sortPriority + 3);
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

                if (auto edit = scene->getChildByType<EditLevelLayer>(0))
                {
                    level = edit->m_level;
                }

                if (auto select = scene->getChildByType<LevelSelectLayer>(0))
                {
                    if (auto page = typeinfo_cast<LevelPage*>(select->m_scrollLayer->getPage(select->m_scrollLayer->m_page)))
                    {
                        level = page->m_level;
                    }
                }
            }

            if (!level)
            {
                BetterAlertLayer::createWithLocalisation("ui/title-error", "uncomplete-level/no-level-found", "ui/ok-button")->show();
                return;
            }

            if (level->m_levelType != GJLevelType::Saved)
            {
                auto t = LocalisationManager::get()->getLocalisedString("ui/title-error");
                auto text = LocalisationManager::get()->getLocalisedString("uncomplete-level/wrong-type-confirmation");
                auto cac = LocalisationManager::get()->getLocalisedString("ui/ok-button");

                std::string type = fmt::format("GJLevelType::unknown ({})", (int)level->m_levelType);

                switch (level->m_levelType)
                {
                    case GJLevelType::Default:
                        type = "GJLevelType::Default";
                        break;
                    case GJLevelType::Main:
                        type = "GJLevelType::Main";
                        break;
                    case GJLevelType::Editor:
                        type = "GJLevelType::Editor";
                        break;
                    case GJLevelType::Saved:
                        type = "GJLevelType::Saved";
                        break;
                    case GJLevelType::SearchResult:
                        type = "GJLevelType::SearchResult";
                        break;
                }

                text = utils::string::replace(text, "%s", type);

                BetterAlertLayer::create(t.c_str(), text, cac.c_str())->show();
                return;
            }

            auto t = LocalisationManager::get()->getLocalisedString("names/shortcut/uncomplete-level");
            auto text = LocalisationManager::get()->getLocalisedString("uncomplete-level/found-confirmation");
            auto cac = LocalisationManager::get()->getLocalisedString("ui/cancel-button");
            auto con = LocalisationManager::get()->getLocalisedString("ui/confirm-button");

            text = utils::string::replace(text, "%s", level->m_levelName);

            auto alert = BetterAlertLayer::create(this, t.c_str(), text, cac.c_str(), con.c_str(), 320, false, 280, 1.0f);
            alert->setUserData(level);
            alert->show();
        }

        virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2)
        {
            GJGameLevel* level = static_cast<GJGameLevel*>(layer->getUserData());

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

                for (auto i = 0; i < level->m_coins; i++)
                {
                    auto key = level->getCoinKey(i + 1);
                    if (GameStatsManager::get()->hasUserCoin(key) && level->m_coinsVerified.value() > 0)
                    {
                        GameStatsManager::get()->setStat("12", GameStatsManager::get()->getStat("12") - 1);
                        GameStatsManager::get()->m_verifiedUserCoins->removeObjectForKey(key);
                    }
                    else if (GameStatsManager::get()->hasPendingUserCoin(key))
                    {
                        GameStatsManager::get()->m_pendingUserCoins->removeObjectForKey(key);
                    }
                }

                GameLevelManager::get()->saveLevel(level);
            }
        }
};

SUBMIT_HACK(UncompleteLevel);
