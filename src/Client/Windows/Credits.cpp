#include "Credits.hpp"

using namespace geode::prelude;

void Credits::onCredit(CCObject* sender) {
    auto tag = static_cast<CCNode*>(sender)->getID();

    if (tag.starts_with("gd"))
    {
        log::info("open gd profile: {}", std::stoi(tag.c_str() + 2));
        ProfilePage::create(std::stoi(tag.c_str() + 2), false)->show();
    }
    else
    {
        CCApplication::sharedApplication()->openURL(tag.c_str());
    }
}

void Credits::createPanel(CCMenu* menu, CCPoint pos, char const * title, char const * name, CCNode* img, float scale, std::string v) {
    auto t = CCLabelBMFont::create(title, "bigFont.fnt");
    t->setPosition(pos + ccp(0, 28));
    t->limitLabelWidth(110, 0.5f, 0.1f);
    menu->addChild(t);

    auto n = CCLabelBMFont::create(name, "goldFont.fnt");
    menu->addChild(n);
    n->limitLabelWidth(110, 0.5f, 0.1f);
    n->setPosition(pos + ccp(0, -28));

    auto btn = CCMenuItemSpriteExtra::create(img, menu, menu_selector(Credits::onCredit));
    btn->setID(v);
    btn->setPosition(pos);
    btn->setContentSize(ccp(34, 34));
    menu->addChild(btn);

    img->setPosition(btn->getContentSize() / 2);
    img->setScale(scale);
}

void Credits::cocosCreate(CCMenu* menu) {
    auto back = CCScale9Sprite::create("square02_small.png");
    back->setContentSize(menu->getContentSize() / 0.5f);
    back->setPosition(ccp(0, 0));
    back->setAnchorPoint(ccp(0, 0));
    back->setScale(0.5f);
    back->setOpacity(100);
    menu->addChild(back);

    auto promImg = SimplePlayer::create(233);
    promImg->setColor({255, 75, 0});
    promImg->setSecondColor({255, 185, 0});
    promImg->setGlowOutline({255, 185, 0});
    promImg->updateColors();

    createPanel(menu, ccp(55, menu->getContentSize().height - 45), "Name", "PrometheusSV", promImg, 1.0f, "gd7107344");

    createPanel(menu, ccp(55 + (230 / 2) * 1, menu->getContentSize().height - 45), "Pastel", "Absolllute", CCSprite::create("absolllute.png"_spr), 0.45f * CCDirector::get()->getContentScaleFactor() * 0.5f * 0.5f, "https://gist.github.com/absoIute/657a4c95bb92755f96e20772adbf5f32");


    auto fireeImg = SimplePlayer::create(98);
    fireeImg->setColor({125, 0, 255});
    fireeImg->setSecondColor({255, 255, 255});
    fireeImg->setGlowOutline({255, 255, 255});
    fireeImg->updateColors();

    createPanel(menu, ccp(55 + (230 / 2) * 2, menu->getContentSize().height - 45), "Copy Hack", "FireeDev", fireeImg, 1.0f, "gd6253758");


    auto catImg = SimplePlayer::create(98);
    catImg->setColor({255, 0, 0});
    catImg->setSecondColor({255, 255, 255});
    catImg->setGlowOutline({0, 255, 255});
    catImg->updateColors();

    createPanel(menu, ccp(55 + (230 / 2) * 0, menu->getContentSize().height - 45 - (85 * 1)), "", "CatXus", catImg, 1.0f, "gd14467409");


    auto kmImg = SimplePlayer::create(233);
    kmImg->setColor({255, 125, 0});
    kmImg->setSecondColor({0, 125, 255});
    kmImg->setGlowOutline({255, 255, 255});
    kmImg->updateColors();

    createPanel(menu, ccp(55 + (230 / 2) * 1, menu->getContentSize().height - 45 - (85 * 1)), "Beta Testers", "km7dev", kmImg, 1.0f, "gd7236822");


    auto mkrImg = SimplePlayer::create(242);
    mkrImg->setColor({125, 0, 255});
    mkrImg->setSecondColor({185, 0, 255});
    mkrImg->updateColors();

    createPanel(menu, ccp(55 + (230 / 2) * 2, menu->getContentSize().height - 45 - (85 * 1)), "", "MrMkr", mkrImg, 1.0f, "gd21449475");


    auto jaidImg = SimplePlayer::create(373);
    jaidImg->setColor({90, 90, 90});
    jaidImg->setSecondColor({182, 0, 255});
    jaidImg->updateColors();

    createPanel(menu, ccp(55 + (230 / 2) * 0, menu->getContentSize().height - 45 - (85 * 2)), "", "Jaid", jaidImg, 1.0f, "gd7669473");


    auto zephImg = SimplePlayer::create(482);
    zephImg->setColor({77, 77, 90});
    zephImg->setSecondColor({0, 200, 255});
    zephImg->setGlowOutline({0, 200, 255});
    zephImg->updateColors();

    createPanel(menu, ccp(55 + (230 / 2) * 1, menu->getContentSize().height - 45 - (85 * 2)), "", "imzeph", zephImg, 1.0f, "gd23948408");


    auto devImg = SimplePlayer::create(5);
    devImg->setColor({125, 125, 255});
    devImg->setSecondColor({125, 255, 175});
    devImg->setGlowOutline({255, 255, 255});
    devImg->updateColors();

    createPanel(menu, ccp(55 + (230 / 2) * 2, menu->getContentSize().height - 45 - (85 * 2)), "Developer", "TheSillyDoggo", devImg, 1.0f, "gd16778880");
}