#include "LanguageNode.hpp"
#include "../LocalisationManager.hpp"
#include "../../GUI/BetterButtonSprite.hpp"
#include "LanguageContributorsUI.hpp"

LanguageNode* LanguageNode::create(std::string lang)
{
    auto pRet = new LanguageNode();

    if (pRet && pRet->init(lang))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool LanguageNode::init(std::string lang)
{
    if (!CCNode::init())
        return false;

    this->lang = lang;
    bool inUse = LocalisationManager::get()->getCurrentLoadedFile() == (Mod::get()->getResourcesDir() / lang);
    language = LocalisationManager::get()->languageForPath(Mod::get()->getResourcesDir() / lang);
    
    this->setID(lang);

    bg = CCScale9Sprite::create("square02b_small.png");
    bg->setScale(0.5f);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setColor(ccc3(0, 0, 0));
    bg->setOpacity(100);

    float percent = getPercentageComplete();
    this->setUserObject(CCFloat::create(lang == "en-AU.json" ? 80085 : percent));

    percentLabel = CCLabelBMFont::create(fmt::format("{:.2f}%", percent * 100).c_str(), "bigFont.fnt");
    percentLabel->setScale(0.4f);
    percentLabel->setOpacity(175);

    auto percentBtn = CCMenuItemSpriteExtra::create(percentLabel, this, menu_selector(LanguageNode::onMissingTranslations));
    percentBtn->setPosition(percentBtn->getContentSize() * ccp(-0.5f, 0.5f));
    percentBtn->setEnabled(percent != 1.0f);

    auto percentMenu = CCMenu::create();
    percentMenu->addChild(percentBtn);

    useMenu = CCMenu::create();
    useMenu->setAnchorPoint(ccp(0, 0));
    useMenu->setScale(0.75f);

    auto useBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName(inUse ? "GJ_selectSongOnBtn_001.png" : "GJ_selectSongBtn_001.png"), this, menu_selector(LanguageNode::onSelectLanguage));
    useBtn->setEnabled(!inUse);

    useMenu->addChild(useBtn);

    auto creditsMenu = CCMenu::create();
    creditsMenu->setVisible(language->getContributors().size() > 0);

    auto creditsSpr = BetterButtonSprite::createWithLocalisation(ccp(120, 28), "ui/language-contributors-button", "bigFont.fnt", "GJ_button_04.png");
    creditsSpr->setScale(0.65f);

    auto creditsBtn = CCMenuItemSpriteExtra::create(creditsSpr, this, menu_selector(LanguageNode::onViewCredits));
    creditsBtn->m_scaleMultiplier = 1.1f;

    creditsMenu->addChild(creditsBtn);

    enName = CCLabelBMFont::create(language->getEnglishName().c_str(), "bigFont.fnt");
    enName->setScale(0.5f);
    enName->setAnchorPoint(ccp(0, 1));

    this->addChild(bg);
    this->addChildAtPosition(enName, Anchor::TopLeft, ccp(2.5f, -2.5f));
    this->addChildAtPosition(percentMenu, Anchor::BottomRight, ccp(-2, 2));
    this->addChildAtPosition(useMenu, Anchor::Right);
    this->addChildAtPosition(creditsMenu, Anchor::BottomLeft, ccp(2.5f, 2.5f) + creditsBtn->getContentSize() / 2);
    return true;
}

void LanguageNode::onSelectLanguage(CCObject* sender)
{
    static_cast<CCMenuItemSpriteExtra*>(sender)->setEnabled(false);

    LocalisationManager::get()->switchLocalisationWithUI(lang);
}

void LanguageNode::onViewCredits(CCObject* sender)
{
    LanguageContributorsUI::create(lang)->show();
}

void LanguageNode::onMissingTranslations(CCObject* sender)
{
    auto en = LocalisationManager::get()->languageForPath(Mod::get()->getResourcesDir() / "en-AU.json");
    auto keys = en->getStrings();
    auto keys2 = language->getStrings();

    std::unordered_map<std::string, std::string> missingKeys = {};

    for (auto key : keys)
    {
        if (!keys2.contains(key.first))
            missingKeys.emplace(key.first, key.second);
    }

    if (missingKeys.empty())
    {
        static_cast<CCMenuItemSpriteExtra*>(sender)->setEnabled(false);
        return;
    }

    std::string ss;
    std::string debugStr;

    for (auto key : missingKeys)
    {
        debugStr.append(fmt::format("\"{}\": \"{}\"\n", key.first, utils::string::replace(key.second, "\n", "\\n")));
        ss.append(key.first);
        ss.append("\n");
    }
    
    auto alert = geode::MDPopup::create(true, "Missing Keys", ss, "OK", "Copy Strings", [this, debugStr](bool btn2)
    {
        if (btn2)
        {
            clipboard::write(debugStr);
        }
    });
    alert->show();
}

float LanguageNode::getPercentageComplete()
{
    if (lang == "en-AU.json")
        return 1;

    auto en = LocalisationManager::get()->languageForPath(Mod::get()->getResourcesDir() / "en-AU.json");

    return (float)language->getStringCount() / (float)en->getStringCount();
}

void LanguageNode::visit()
{
    if (!nativeName)
    {
        auto name = language->getNativeName();

        if (name == "Error")
            name = this->lang;

        nativeName = CCLabelTTF::create(name.c_str(), "Arial.ttf", 16);
        nativeName->setScale(0.5f);
        nativeName->setAnchorPoint(ccp(0, 1));

        this->addChildAtPosition(nativeName, Anchor::TopLeft, ccp(4, (-2.5f * 2) - enName->getScaledContentHeight()));
    }

    bg->setContentSize(getContentSize() / 0.5f);

    float off = (getContentHeight() - percentLabel->getScaledContentHeight());
    useMenu->setPosition(ccp(getContentWidth() - off / 2, getContentHeight() / 2 + (percentLabel->getScaledContentHeight() / 2)));

    CCNode::visit();
}