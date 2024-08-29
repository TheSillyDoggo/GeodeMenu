#include <Geode/Geode.hpp>
#include <Geode/modify/CCSprite.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/modify/AppDelegate.hpp>
#include <Geode/modify/AchievementNotifier.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

std::string getNodeName(CCObject* node) {
#ifdef GEODE_IS_WINDOWS
    return typeid(*node).name() + 6;
#else 
    {
        std::string ret;

        int status = 0;
        auto demangle = abi::__cxa_demangle(typeid(*node).name(), 0, 0, &status);
        if (status == 0) {
            ret = demangle;
        }
        free(demangle);

        return ret;
    }
#endif
}

ccColor3B oldColour = ccc3(0, 102, 255);

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

        if (!Client::GetModuleEnabled("trans-bg"))
            return;

        if (scene->getChildrenCount() > 0)
        {
            if (auto l = as<CCLayer*>(scene->getChildren()->objectAtIndex(0)); l->getChildrenCount() > 0)
            {
                if (getChildOfType<LevelEditorLayer>(scene, 0))
                    return;

                if (getChildOfType<LoadingLayer>(scene, 0))
                    return;

                l->sortAllChildren();

                if (auto b = typeinfo_cast<CCSprite*>(l->getChildren()->objectAtIndex(0)))
                {
                    if (getNodeName(b).starts_with("cocos2d::CCSprite"))
                    {
                        if (b->getColor().r == 0 && b->getColor().g == 102 && b->getColor().b == 255)
                        {
                            oldColour = b->getColor();
                            b->setColor({255, 255, 255});
                        }
                    }
                }
            }
        }
    }
};

$execute
{
    Loader::get()->queueInMainThread([] {
        Client::GetModule("trans-bg")->onToggle = [](bool enabled){
            if (auto scene = CCScene::get())
            {
                if (scene->getChildrenCount() > 0)
                {
                    if (auto l = as<CCLayer*>(scene->getChildren()->objectAtIndex(0)); l->getChildrenCount() > 0)
                    {
                        if (getChildOfType<LevelEditorLayer>(scene, 0))
                            return;

                        if (getChildOfType<LoadingLayer>(scene, 0))
                            return;

                        l->sortAllChildren();

                        if (auto b = typeinfo_cast<CCSprite*>(l->getChildren()->objectAtIndex(0)))
                        {
                            if (getNodeName(b).starts_with("cocos2d::CCSprite"))
                            {
                                if ((b->getColor().r == 0 && b->getColor().g == 102 && b->getColor().b == 255) || (b->getColor() == ccc3(255, 255, 255)))
                                {
                                    if (enabled)
                                        b->setColor({255, 255, 255});
                                    else
                                        b->setColor(oldColour);
                                }
                            }
                        }
                    }
                }
            }
        };
    });
}