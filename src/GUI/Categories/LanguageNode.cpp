#include "LanguageNode.hpp"
#include "../../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "../../Localisation/LocalisationManager.hpp"
#include "../../Localisation/UI/LanguageNode.hpp"
#include "../BetterButtonSprite.hpp"

bool LanguageNodeUI::init()
{
    if (!CategoryNode::init())
        return false;

    std::vector<std::string> langs = { "en-AU.json", "ja-JP.json", "de-DE.json", "ru-RU.json", "vi-VI.json", "id-ID.json" };

    int x = 0;
    int y = 0;

    for (auto lang : langs)
    {
        auto node = LanguageNode::create(lang);
        node->setContentSize(ccp((getContentWidth() - 2.5f * 3) / 2, 60));
        node->setAnchorPoint(ccp(x, 1));
        node->setPosition(ccp(x == 0 ? 2.5f : getContentWidth() - 2.5f, getContentHeight() - y * (node->getContentHeight() + 2.5f)) + ccp(0, -25));
        node->updateLayout();

        this->addChild(node);

        x++;

        if (x == 2)
        {
            x = 0;
            y++;
        }
    }

    return true;
}