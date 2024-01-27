#include <Geode/Geode.hpp>
#include <Geode/modify/CCTextInputNode.hpp>
#include <Geode/modify/CCSprite.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (CCTextInputNode)
{
    TodoReturn updateLabel(gd::string p0)
    {
        log::info("updating label");
        
        if (Client::GetModuleEnabled("char-limit"))
            CCTextInputNode::setMaxLabelLength(99999);

        if (Client::GetModuleEnabled("char-filter"))
            CCTextInputNode::setAllowedChars("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ ");
        
        CCTextInputNode::updateLabel(p0);
    }
};