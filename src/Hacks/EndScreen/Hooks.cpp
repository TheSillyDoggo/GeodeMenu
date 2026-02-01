#include "Hooks.hpp"
#include "EndscreenStatLabel.hpp"
#include "../Level/Noclip/Noclip.hpp"

void CustomEndLevelLayer::customSetup()
{
    EndLevelLayer::customSetup();
    return;

    if (Loader::get()->getLoadedMod("geode.node-ids"))
    {

    }
    else
    {
        for (auto child : CCArrayExt<CCNode*>(m_mainLayer->getChildren()))
        {
            if (typeinfo_cast<CCLabelBMFont*>(child))
            {
                child->setVisible(false);
            }
        }

        auto menu = CCMenu::create();
        menu->setPosition(CCDirector::get()->getWinSize() / 2 + ccp(0, 35));

        bool right = false;
        int y = 0;

        EndscreenStatLabel* lbl;

        for (size_t i = 0; i < 6; i++)
        {
            if (i == (int)EndscreenStatType::Points)
            {
                if (!GJBaseGameLayer::get()->m_level->isPlatformer())
                    continue;
            }

            if (i == (int)EndscreenStatType::NoclipDeaths || i == (int)EndscreenStatType::NoclipAccuracy)
            {
                if (!(static_cast<NoclipBaseGameLayer*>(GJBaseGameLayer::get())->m_fields->hasDiedThisAttempt || Noclip::get()->getRealEnabled()))
                    continue;
            }

            lbl = EndscreenStatLabel::create((EndscreenStatType)i);
            lbl->setPosition(ccp(right ? 80 : -80, -y * 24));

            menu->addChild(lbl);

            right = !right;
            if (!right)
                y++;
        }

        if (right && lbl)
        {
            lbl->setPositionX(0);
        }

        if (menu->getChildrenCount() <= 3)
        {
            int i = 0;
            for (auto child : CCArrayExt<CCNode*>(menu->getChildren()))
            {
                child->setPosition(ccp(0, -i * 24));
                i++;
            }
            
        }
        
        m_mainLayer->addChild(menu);
    }
}