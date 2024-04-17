#include <Geode/Geode.hpp>
#include <Geode/modify/CCSprite.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/modify/AppDelegate.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

/*class $modify (CCSprite)
{
    bool grad;

    virtual bool initWithFile(const char *pszFilename)
    {
        if (!CCSprite::initWithFile(pszFilename))
            return false;

        if (std::string(pszFilename).starts_with("GJ_gradientBG.png"))
        {
            m_fields->grad = true;
        }

        return true;
    }

    virtual void setColor(const ccColor3B& color3)
    {
        if (m_fields->grad && Client::GetModuleEnabled("trans-bg"))
            CCSprite::setColor({255, 255, 255});
        else
            CCSprite::setColor(color3);
    }
};*/

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

class $modify (AppDelegate)
{
    virtual void willSwitchToScene(CCScene* scene)
    {
        AppDelegate::willSwitchToScene(scene);

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
                            b->setColor({255, 255, 255});
                        }
                    }
                }
            }
        }
    }
};