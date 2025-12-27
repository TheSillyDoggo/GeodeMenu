#include "LanguageNode.hpp"
#include "../LocalisationManager.hpp"

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

    json = LocalisationManager::get()->getCachedFile(Mod::get()->getResourcesDir() / lang);

    bg = CCScale9Sprite::create("square02b_small.png");
    bg->setScale(0.5f);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setColor(ccc3(0, 0, 0));
    bg->setOpacity(100);

    percentLabel = CCLabelBMFont::create(fmt::format("{:.2f}%", getPercentageComplete() * 100).c_str(), "bigFont.fnt");
    percentLabel->setScale(0.4f);
    percentLabel->setAnchorPoint(ccp(1, 0));
    percentLabel->setOpacity(175);

    useMenu = CCMenu::create();
    useMenu->setAnchorPoint(ccp(0, 0));
    useMenu->setScale(0.75f);

    auto useBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName(inUse ? "GJ_selectSongOnBtn_001.png" : "GJ_selectSongBtn_001.png"), this, menu_selector(LanguageNode::onSelectLanguage));
    useBtn->setEnabled(!inUse);

    useMenu->addChild(useBtn);

    enName = CCLabelBMFont::create(json["display_name_english"].asString().unwrapOr("Error").c_str(), "bigFont.fnt");
    enName->setScale(0.5f);
    enName->setAnchorPoint(ccp(0, 1));

    nativeName = CCLabelTTF::create(json["display_name_native"].asString().unwrapOr("Error").c_str(), "Arial.ttf", 16);
    nativeName->setScale(0.5f);
    nativeName->setAnchorPoint(ccp(0, 1));

    bool showNativeName = true;
    nativeName->setVisible(showNativeName);

    this->addChild(bg);
    this->addChildAtPosition(enName, Anchor::TopLeft, ccp(2.5f, -2.5f));
    this->addChildAtPosition(nativeName, Anchor::TopLeft, ccp(4, (-2.5f * 2) - enName->getScaledContentHeight()));
    this->addChildAtPosition(percentLabel, Anchor::BottomRight, ccp(-2, 2));
    this->addChildAtPosition(useMenu, Anchor::Right);
    return true;
}

void LanguageNode::onSelectLanguage(CCObject* sender)
{
    static_cast<CCMenuItemSpriteExtra*>(sender)->setEnabled(false);

    LocalisationManager::get()->switchLocalisationWithUI(lang);
}

void LanguageNode::onViewCredits(CCObject* sender)
{

}

float LanguageNode::getPercentageComplete()
{
    if (lang == "en-AU.json")
        return 1;

    auto en = LocalisationManager::get()->getCachedFile(Mod::get()->getResourcesDir() / "en-AU.json");

    return (float)getStringCount(json) / (float)getStringCount(en);
}

int LanguageNode::getStringCount(matjson::Value value)
{
    int count = 0;

    if (value.contains("strings") && value["strings"].isObject())
    {
        for (auto type : value["strings"])
        {
            if (type.isString())
                count++;

            if (type.isObject())
            {
                for (auto str1 : type)
                {
                    if (str1.isString())
                        count++;
                }
            }
        }
    }

    return count;
}

void LanguageNode::visit()
{
    bg->setContentSize(getContentSize() / 0.5f);

    float off = (getContentHeight() - percentLabel->getScaledContentHeight());
    useMenu->setPosition(ccp(getContentWidth() - off / 2, getContentHeight() / 2 + (percentLabel->getScaledContentHeight() / 2)));

    CCNode::visit();
}