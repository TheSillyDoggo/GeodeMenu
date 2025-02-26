#include "SFXModuleManageLayer.hpp"

void SFXModuleManageLayer::customSetup()
{
    this->scheduleUpdate();
    bg->updateTheme(2);

    warning = TextArea::create("Nothing here yet <cc>3:</c>", "bigFont.fnt", 1.0f, 6969, ccp(0.5f, 0.5f), 20, false);
    warning->setPosition(ccp(340, 220) / 2);
    warning->setScale(0.45f);

    auto outline = GJCommentListLayer::create(nullptr, "", ccc4(0, 0, 0, 0), 340, 220, true);
    outline->ignoreAnchorPointForPosition(false);

    auto stencil = CCScale9Sprite::create("GJ_square01.png");
    stencil->setContentSize(ccp(340, 220));
    stencil->setAnchorPoint(ccp(0, 0));

    auto clip = CCClippingNode::create(stencil);
    clip->setAlphaThreshold(0.01f);

    outline->addChild(clip);

    scroll = ScrollLayer::create(ccp(340, 220));
    scroll->m_contentLayer->setColor(ccc3(50, 50, 50));
    scroll->m_contentLayer->setOpacity(255);

    clip->addChild(scroll);
    clip->addChild(warning);

    auto menu = CCMenu::create();
    menu->setAnchorPoint(ccp(0, 0));
    menu->setScale(0.75f);

    auto sfxBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_newBtn_001.png"), this, menu_selector(SFXModuleManageLayer::onNewSFX));

    auto browseSpr = CircleButtonSprite::create(CCSprite::createWithSpriteFrameName("folderIcon_001.png"), CircleBaseColor::Pink);
    browseSpr->setScale(0.8f);

    auto browseBtn = CCMenuItemSpriteExtra::create(browseSpr, this, menu_selector(SFXModuleManageLayer::onFileSFX));
    browseBtn->setPositionY(50);

    menu->addChild(sfxBtn);
    menu->addChild(browseBtn);

    updateList();

    baseLayer->addChildAtPosition(outline, Anchor::Center, ccp(0, 12.5f));
    baseLayer->addChildAtPosition(menu, Anchor::BottomRight, ccp(-30, 30));
}

void SFXModuleManageLayer::updateList()
{
    scroll->m_contentLayer->removeAllChildren();
    
    std::vector<CCNode*> cells;
    int i = 0;

    for (auto sfx : mod->sfxs)
    {
        auto node = CCNode::create();
        node->setPositionY(i * 20);

        auto bg = CCLayerColor::create(i % 2 == 0 ? ccc4(75, 75, 75, 255) : ccc4(100, 100, 100, 255), 340, 20);

        auto fs = std::filesystem::path(sfx);

        if (fs.filename().string().starts_with("s") && fs.filename().string().ends_with(".ogg"))
        {
            auto n = fs.filename().string().substr(1, fs.filename().string().size() - 5);

            auto label = CCLabelBMFont::create(fmt::format("SFX ID: {}", n).c_str(), "bigFont.fnt");
            label->setPosition(ccp(10 / 2, 20 / 2));
            label->setAnchorPoint(ccp(0, 0.5f));
            label->limitLabelWidth(250, 0.35f, 0);
            node->addChild(label);
        }
        else
        {
            auto label = CCLabelBMFont::create(sfx.c_str(), "bigFont.fnt");
            label->setPosition(ccp(10 / 2, 20 / 2));
            label->setAnchorPoint(ccp(0, 0.5f));
            label->limitLabelWidth(250, 0.35f, 0);
            node->addChild(label);
        }

        auto menu = CCMenu::create();
        menu->setContentSize(ccp(0, 0));
        menu->setPosition(ccp(340, 20 / 2));

        auto spr = CCSprite::createWithSpriteFrameName("GJ_playMusicBtn_001.png");
        spr->setScale(0.4f);

        auto preview = CCMenuItemSpriteExtra::create(spr, this, menu_selector(SFXModuleManageLayer::onPreviewSFX));
        preview->setID(sfx);
        preview->setPositionX(-10);

        auto deleteSpr = CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
        deleteSpr->setScale(0.55f);

        auto deleteBtn = CCMenuItemSpriteExtra::create(deleteSpr, this, menu_selector(SFXModuleManageLayer::onRemoveSFX));
        deleteBtn->setTag(i);
        deleteBtn->setPositionX(-27);

        menu->addChild(preview);
        menu->addChild(deleteBtn);
        
        node->addChild(bg, -2);
        node->addChild(menu);

        if (i != 0)
        {
            auto line = CCLayerColor::create(ccc4(0, 0, 0, 100), 320, 0.5f);
            line->setPosition(ccp(340 / 2, 20));
            line->ignoreAnchorPointForPosition(false);

            node->addChild(line);
        }

        cells.push_back(node);
        i++;
    }

    auto height = std::max<float>(cells.size() * 20, 220);

    scroll->m_contentLayer->setContentHeight(height);
    scroll->setTouchEnabled(height != 220);

    i = 1;

    for (auto cell : cells)
    {
        cell->setPositionY(height - i * 20);
        
        scroll->m_contentLayer->addChild(cell);

        i++;
    }

    warning->setVisible(cells.size() == 0);
}

void SFXModuleManageLayer::onRemoveSFX(CCObject* sender)
{
    mod->sfxs.erase(mod->sfxs.begin() + sender->getTag());

    updateList();
}

void SFXModuleManageLayer::onPreviewSFX(CCObject* sender)
{
    FMODAudioEngine::get()->stopAllEffects();
    FMODAudioEngine::get()->playEffect(as<CCNode*>(sender)->getID());
}

void SFXModuleManageLayer::onNewSFX(CCObject* sender)
{
    browser = SFXBrowser::create(0);

    browser->show();
}

void SFXModuleManageLayer::onFileSFX(CCObject* sender)
{
    file::FilePickOptions options;

    file::FilePickOptions::Filter filter;
    filter.description = "Audio File";
    filter.files = { "*.mp3", "*.ogg", "*.wav" };

    options.filters.push_back(filter);

    file::pickMany(options).listen([this](Result<std::vector<std::filesystem::path>>* path)
    {
        if (path->isOk())
        {
            auto paths = path->unwrap();

            for (auto path : paths)
            {
                mod->sfxs.push_back(path.string());
            }

            mod->save();
            updateList();
        }
    });
}

void SFXModuleManageLayer::update(float)
{
    if (browser)
    {
        if (!browser->getParent())
        {
            if (browser->m_sfxID != 0)
            {
                mod->sfxs.push_back(MusicDownloadManager::sharedState()->pathForSFX(browser->m_sfxID));

                mod->save();
                updateList();
            }

            browser = nullptr;
        }
    }
}

SFXModuleManageLayer* SFXModuleManageLayer::create(SFXModule* mod)
{
    auto pRet = new SFXModuleManageLayer();

    pRet->mod = mod;

    if (pRet && pRet->initWithSizeAndName(ccp(440, 290), ""))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}