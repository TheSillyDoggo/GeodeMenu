#include "LanguageNode.hpp"
#include "../../Utils/AdvancedLabel/AdvLabelBMFont.hpp"
#include "../../Localisation/LocalisationManager.hpp"
#include "../../Localisation/UI/LanguageNode.hpp"
#include "../BetterButtonSprite.hpp"

bool LanguageNodeUI::init()
{
    if (!CategoryNode::init())
        return false;

    auto gradient = CCLayerGradient::create(ccc4(139, 89, 173, 255), ccc4(45, 110, 222, 255), ccp(1, -0.5f));
    gradient->ignoreAnchorPointForPosition(false);
    gradient->setAnchorPoint(ccp(0, 1));
    gradient->setContentSize(ccp(getContentWidth() - 2.5f * 2, 20));

    auto stencil = CCScale9Sprite::create("square02b_small.png");
    stencil->setContentSize(gradient->getContentSize() / 0.5f);
    stencil->setPosition(gradient->getContentSize() * ccp(0.5f, -0.5f));
    stencil->setScale(0.5f);

    auto clip = CCClippingNode::create(stencil);
    clip->setAlphaThreshold(0.00f);
    clip->addChild(gradient);

    auto missingLabel = AdvLabelBMFont::createWithLocalisation("ui/language-missing-unfinished", "bigFont.fnt");
    missingLabel->setAnchorPoint(ccp(0, 0.5f));
    missingLabel->limitLabelWidth(170, 0.4f, 0);

    auto missingSpr = BetterButtonSprite::createWithLocalisation(ccp(100, 30), "ui/language-contribute-button", "bigFont.fnt", "geode.loader/GE_button_01.png");
    missingSpr->setScale(0.55f);
    auto missingBtn = CCMenuItemSpriteExtra::create(missingSpr, this, menu_selector(LanguageNodeUI::onContribute));
    missingBtn->setPositionX(missingLabel->getScaledContentWidth() + missingBtn->getContentWidth() / 2 + 6);

    auto missingMenu = CCMenu::create();
    missingMenu->setContentSize(ccp(missingLabel->getScaledContentWidth() + missingBtn->getContentWidth() + 6, 20));
    missingMenu->ignoreAnchorPointForPosition(false);
    missingMenu->setAnchorPoint(ccp(0.5f, 0));
    missingMenu->addChild(missingLabel);
    missingMenu->addChild(missingBtn);

    auto langs = LocalisationManager::get()->getAllLanguageFiles();
    float height = std::ceil(langs.size() / 2.0f) * (60 + 2.5f);

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

    this->addChildAtPosition(clip, Anchor::TopLeft, ccp(2.5f, 0));
    this->addChildAtPosition(missingMenu, Anchor::Top, ccp(0, -10));
    return true;
}

void LanguageNodeUI::updateUI()
{
    scroll->setContentHeight(getContentHeight() - 20 - 2.5f);

    scroll->moveToTop();
    scroll->setTouchEnabled(true);
    scrollbar->setVisible(false);
    scrollbar->setDisabled(true);
}

void LanguageNodeUI::onContribute(CCObject* sender)
{
    geode::utils::web::openLinkInBrowser("github.com/TheSillyDoggo/QOLMod-Translations/");
}