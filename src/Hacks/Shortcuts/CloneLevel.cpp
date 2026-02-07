#include <Geode/Geode.hpp>
#include "../../Client/ButtonModule.hpp"
#include <BetterAlertLayer.hpp>
#include <LocalisationManager.hpp>

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
