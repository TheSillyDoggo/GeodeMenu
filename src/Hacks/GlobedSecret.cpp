/*#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Ref<CCLabelBMFont> lbl1 = nullptr;
Ref<CCLabelBMFont> lbl2 = nullptr;

void mySetID(CCNode* ins, const std::string &id) {
    ins->setID(id);

    if (id == "dankmeme.globed2/remote-player-16778880")
    {
        log::info("i am real");

        Notification::create("TheSillyDoggo (creator of qolmod) is in your Globed server\nsay hi :3 :3 :3", NotificationIcon::None, 0.69f)->show(); //atleast make it show for very long

        lbl1 = (getChildOfType<CCLabelBMFont>(as<CCNode*>(ins->getChildren()->objectAtIndex(0)), 0));
        if (lbl1->getID() == "lbl1")
            lbl1 = nullptr;
        
        lbl2 = (getChildOfType<CCLabelBMFont>(as<CCNode*>(ins->getChildren()->objectAtIndex(1)), 0));
        if (lbl2->getID() == "lbl2")
            lbl2 = nullptr;
        
        if (lbl1)
            lbl1->setID("lbl1");

        if (lbl2)
            lbl2->setID("lbl2");
    }
}

class $modify (CCScheduler)
{
    void update(float dt)
    {
        CCScheduler::update(dt);

        if (lbl1 && lbl1->getParent())
            lbl1->setColor(ColourUtility::getPastelColour());

        if (lbl2 && lbl2->getParent())
            lbl2->setColor(ColourUtility::getPastelColour());
    }
};

$execute {
    Mod::get()->hook(
        reinterpret_cast<void*>(
            geode::addresser::getNonVirtual(&CCNode::setID)
        ),
        &mySetID,
        "cocos2d::CCNode::setID (Geode)",
        tulip::hook::TulipConvention::Thiscall
    );
}*/