#include "ExtraThemeSettingsUI.hpp"
#include "Modules/GradientBGColours.hpp"
#include "BetterButtonSprite.hpp"
#include <AndroidUI.hpp>

ExtraThemeSettingsUI* ExtraThemeSettingsUI::create(bool image)
{
    auto pRet = new ExtraThemeSettingsUI();

    pRet->image = image;
    CCSize size = image ? ccp(360, 240) : ccp(300, 220);

    if (pRet && pRet->initAnchored(size.width, size.height))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool ExtraThemeSettingsUI::setup()
{
    m_bgSprite->setVisible(false);
    bg = BackgroundSprite::create();
    bg->setContentSize(this->m_size);
    bg->setZOrder(-10);
    bg->setTheme(-6);

    m_buttonMenu->setVisible(false);
    m_mainLayer->addChildAtPosition(bg, Anchor::Center);

    auto title = AdvLabelBMFont::createWithLocalisation(image ? "extra-theme-settings/title-image" : "extra-theme-settings/title", "goldFont.fnt");
    title->setScale(0.7f);

    auto menu = CCMenu::create();

    auto spr = BetterButtonSprite::createWithLocalisation(ccp(54.25f, 30), "ui/ok-button", "goldFont.fnt", "GJ_button_01.png");
    auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(ExtraThemeSettingsUI::onClose));
    menu->addChild(btn);

    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, -18));
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));

    if (image)
    {
        imagePreview = BackgroundSprite::create();
        imagePreview->setTheme(-7);
        imagePreview->setContentSize(ccp(260, 120));

        auto spr1 = BetterButtonSprite::createWithLocalisation(ccp(120, 30), "ui/import-from-file-button", "bigFont.fnt", "geode.loader/GE_button_01.png");
        spr1->setMaxTextScale(0.6f);
        auto btn1 = CCMenuItemSpriteExtra::create(spr1, this, menu_selector(ExtraThemeSettingsUI::onSelectImage));
        btn1->setPosition(ccp(-120 / 2 - 2.5f, 37.5f));
        btn1->m_scaleMultiplier = 1.1f;

        auto spr2 = BetterButtonSprite::createWithLocalisation(ccp(120, 30), "ui/default-button", "bigFont.fnt", "geode.loader/GE_button_01.png");
        spr2->setMaxTextScale(0.6f);
        auto btn2 = CCMenuItemSpriteExtra::create(spr2, this, menu_selector(ExtraThemeSettingsUI::onResetDefault));
        btn2->setPosition(ccp(120 / 2 + 2.5f, 37.5f));
        btn2->m_scaleMultiplier = 1.1f;
        
        m_mainLayer->addChildAtPosition(imagePreview, Anchor::Center, ccp(0, 25));
        menu->addChild(btn1);
        menu->addChild(btn2);
    }
    else
    {
        auto bg2 = CCScale9Sprite::create("square02b_small.png");
        bg2->setContentSize(ccp(165, 120) / 0.5f);
        bg2->setScale(0.5f);
        bg2->setColor(ccc3(0, 0, 0));
        bg2->setOpacity(100);

        auto mod1 = GradientBGStart::get()->getNode();
        auto mod2 = GradientBGEnd::get()->getNode();
        auto mod3 = GradientBGDirection::get()->getNode();

        m_mainLayer->addChildAtPosition(bg2, Anchor::Center, ccp(0, 5));
        m_mainLayer->addChildAtPosition(mod1, Anchor::Center, ccp(6, 5 + 40));
        m_mainLayer->addChildAtPosition(mod2, Anchor::Center, ccp(6, 5));
        m_mainLayer->addChildAtPosition(mod3, Anchor::Center, ccp(6, 5 - 40));
    }
    return true;
}

void ExtraThemeSettingsUI::onSelectImage(CCObject* sender)
{
    file::FilePickOptions options;
    options.defaultPath = Mod::get()->getConfigDir();

    file::FilePickOptions::Filter filter;
    filter.description = "Image File";
    filter.files = { "*.png" };
    options.filters.push_back(filter);

    async::spawn(file::pick(file::PickMode::OpenFile, options), [this](Result<std::optional<std::filesystem::path>> result)
    {
        if (result.isOk())
        {
            if (result.unwrap().has_value())
            {
                auto filePath = result.unwrap().value_or("");

                Mod::get()->setSavedValue<std::string>("custom-background-path", filePath.string());
                bg->updateCustomSprite();
                imagePreview->updateCustomSprite();

                if (AndroidUI::get())
                {
                    AndroidUI::get()->bg->updateCustomSprite();
                }
            }
        }
    });
}

void ExtraThemeSettingsUI::onResetDefault(CCObject* sender)
{
    Mod::get()->setSavedValue<std::string>("custom-background-path", "");
    bg->updateCustomSprite();
    imagePreview->updateCustomSprite();

    if (AndroidUI::get())
    {
        AndroidUI::get()->bg->updateCustomSprite();
    }
}