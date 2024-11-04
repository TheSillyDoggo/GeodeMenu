#include <Geode/Geode.hpp>
#include <Geode/modify/GauntletLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GauntletLayer)
{
    virtual void loadLevelsFinished(cocos2d::CCArray* p0, char const* p1, int p2)
    {
        GauntletLayer::loadLevelsFinished(p0, p1, p2);

        if (auto menu = getChildByIDRecursive("levels-menu"))
        {
            for (auto child : CCArrayExt<CCNode*>(menu->getChildren()))
            {
                if (auto btn = typeinfo_cast<CCMenuItem*>(child))
                    btn->setEnabled(true);
            }
        }
    }

    bool init(GauntletType p0)
    {
        if (!GauntletLayer::init(p0))
            return false;

        if (auto menu = getChildByIDRecursive("levels-menu"))
        {
            for (auto child : CCArrayExt<CCNode*>(menu->getChildren()))
            {
                if (auto btn = typeinfo_cast<CCMenuItem*>(child))
                    btn->setEnabled(true);
            }
        }

        return true;
    }

    QOLMOD_MOD_ALL_HOOKS("unlock-gauntlets")
};