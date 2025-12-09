#include "../../Client/Module.hpp"
#include <Geode/modify/AppDelegate.hpp>
#include <Geode/modify/AchievementNotifier.hpp>

using namespace geode::prelude;

class TransparentBG : public Module
{
    public:
        MODULE_SETUP(TransparentBG)
        {
            setName("Transparent BG");
            setID("trans-bg");
            setCategory("Universal");
            setDescription("Sets the colour for all gradient backgrounds to white");
        }
};

SUBMIT_HACK(TransparentBG);

#ifdef __APPLE__
class $modify (AchievementNotifier)
#else
class $modify (AppDelegate)
#endif
{
    virtual void willSwitchToScene(CCScene* scene)
    {
        #ifdef __APPLE__
        AchievementNotifier::willSwitchToScene(scene);
        #else
        AppDelegate::willSwitchToScene(scene);
        #endif

        if (!TransparentBG::get()->getRealEnabled())
            return;

        if (scene->getChildrenCount() > 0)
        {
            if (auto l = scene->getChildByType<CCLayer>(0))
            {
                if (scene->getChildByType<LevelEditorLayer>(0))
                    return;

                if (scene->getChildByType<LoadingLayer>(0))
                    return;

                l->sortAllChildren();

                if (auto b = typeinfo_cast<CCSprite*>(l->getChildren()->objectAtIndex(0)))
                {
                    if (b->getColor().r == 0 && b->getColor().g == 102 && b->getColor().b == 255)
                    {
                        b->setColor({255, 255, 255});
                    }
                }
            }
        }
    }
};