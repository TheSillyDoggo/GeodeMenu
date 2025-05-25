#include <Geode/Geode.hpp>
#include <Geode/modify/CCTextInputNode.hpp>
#include <Geode/modify/CCSprite.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (CCTextInputNode)
{
    void updateLabel(gd::string p0)
    {
        if (this->getID().starts_with("IGNOREBYPASSES"_spr))
        {
            CCTextInputNode::updateLabel(p0);

            return;
        }

        if (Module::get("char-limit")->enabled)
            CCTextInputNode::setMaxLabelLength(6969);

        if (Module::get("char-filter")->enabled)
            CCTextInputNode::setAllowedChars("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ ");
        
        CCTextInputNode::updateLabel(p0);
    }
};