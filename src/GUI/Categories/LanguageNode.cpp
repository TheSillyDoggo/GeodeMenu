#include "LanguageNode.hpp"
#include "../../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "../../Localisation/LocalisationManager.hpp"
#include "../../Localisation/UI/LanguageNode.hpp"
#include "../BetterButtonSprite.hpp"

bool LanguageNodeUI::init()
{
    if (!CategoryNode::init())
        return false;

    auto langs = LocalisationManager::get()->getAllLanguageFiles();
    float height = (std::ceil((langs.size() / 2) / 2) * 2) * 2 * (60 + 2.5f);

    int x = 0;
    int y = 0;
    scroll->m_contentLayer->setContentHeight(height);

    std::vector<LanguageNode*> nodes = {};

    for (auto lang : langs)
    {
        auto node = LanguageNode::create(lang);
        node->setContentSize(ccp((getContentWidth() - 2.5f * 3) / 2, 60));
        node->updateLayout();
        nodes.push_back(node);

        scroll->m_contentLayer->addChild(node);
    }

    std::sort(nodes.begin(), nodes.end(), [](LanguageNode* a, LanguageNode* b)
    {
        return static_cast<CCFloat*>(a->getUserObject())->getValue() > static_cast<CCFloat*>(b->getUserObject())->getValue();
    });

    for (auto node : nodes)
    {
        node->setAnchorPoint(ccp(x, 1));
        node->setPosition(ccp(x == 0 ? 2.5f : getContentWidth() - 2.5f, (height) - (y * (node->getContentHeight() + 2.5f))));

        x++;

        if (x == 2)
        {
            x = 0;
            y++;
        }
    }

    return true;
}

void LanguageNodeUI::updateUI()
{
    scroll->moveToTop();
    scroll->setTouchEnabled(true);
    scrollbar->setVisible(false);
    scrollbar->setDisabled(true);
}