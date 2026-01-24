#include "CustomRewardSetupUI.hpp"

CustomRewardSetupUI* CustomRewardSetupUI::create()
{
    auto pRet = new CustomRewardSetupUI();

    CCSize size = ccp(250, 270);

    if (pRet && pRet->initAnchored(size.width, size.height))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool CustomRewardSetupUI::setup()
{
    m_bgSprite->setVisible(false);
    bg = BackgroundSprite::create();
    bg->setContentSize(this->m_size);
    bg->setPosition(this->m_size / 2);

    m_buttonMenu->setVisible(false);
    m_mainLayer->addChild(bg);

    auto title = AdvLabelBMFont::createWithLocalisation("names/create-custom-reward", "goldFont.fnt");
    title->setScale(0.7f);

    auto menu = CCMenu::create();
    menu->setLayout(AxisLayout::create(Axis::Row)->setAutoScale(false)->setGap(10));

    auto menu2 = CCMenu::create();
    menu2->setLayout(AxisLayout::create(Axis::Column)->setAutoScale(false)->setGap(5)->setGrowCrossAxis(true)->setAxisReverse(true)->setCrossAxisReverse(true));
    menu2->setContentHeight(200 / 0.8f);
    menu2->setScale(0.8f);

    menu2->addChild(createCreateInput("mana-orbs", "currencyOrbIcon_001.png"));
    menu2->addChild(createCreateInput("fire-shard", "fireShardBig_001.png"));
    menu2->addChild(createCreateInput("ice-shard", "iceShardBig_001.png"));
    menu2->addChild(createCreateInput("poison-shard", "poisonShardBig_001.png"));
    menu2->addChild(createCreateInput("shadow-shard", "shadowShardBig_001.png"));
    menu2->addChild(createCreateInput("lava-shard", "lavaShardBig_001.png"));
    menu2->addChild(createCreateInput("keys", "GJ_bigKey_001.png")); // column 2
    menu2->addChild(createCreateInput("earth-shard", "shard0201ShardBig_001.png"));
    menu2->addChild(createCreateInput("blood-shard", "shard0202ShardBig_001.png"));
    menu2->addChild(createCreateInput("metal-shard", "shard0203ShardBig_001.png"));
    menu2->addChild(createCreateInput("light-shard", "shard0204ShardBig_001.png"));
    menu2->addChild(createCreateInput("soul-shard", "shard0205ShardBig_001.png"));

    menu2->updateLayout();

    auto sprCreate = BetterButtonSprite::createWithLocalisation(ccp(65, 30), "create-custom-reward/create-button", "goldFont.fnt", "GJ_button_01.png");
    auto btnCreate = CCMenuItemSpriteExtra::create(sprCreate, this, menu_selector(CustomRewardSetupUI::onCreateReward));

    auto spr = BetterButtonSprite::createWithLocalisation(ccp(54.25f, 30), "ui/cancel-button", "goldFont.fnt", "GJ_button_01.png");
    auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(CustomRewardSetupUI::onClose));
    
    menu->addChild(btnCreate);
    menu->addChild(btn);
    menu->updateLayout();

    m_mainLayer->addChildAtPosition(title, Anchor::Top, ccp(0, -18));
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 24.5f));
    m_mainLayer->addChildAtPosition(menu2, Anchor::Center, ccp(0, 5));
    return true;
}

void CustomRewardSetupUI::onCreateReward(CCObject* sender)
{
    std::unordered_map<std::string, int> values = {};

    for (auto input : textInputs)
    {
        int v = utils::numFromString<int>(input.second->getString()).unwrapOr(0);

        values[input.first] = v;
    }

    CCArray* array = CCArray::create();

    #define STAT(name, key) \
    if (values[key] > 0) \
    { \
        array->addObject(GJRewardObject::create(name, values[key], -80085)); \
    }

    STAT(SpecialRewardItem::Orbs, "mana-orbs");
    STAT(SpecialRewardItem::FireShard, "fire-shard");
    STAT(SpecialRewardItem::IceShard, "ice-shard");
    STAT(SpecialRewardItem::PoisonShard, "poison-shard");
    STAT(SpecialRewardItem::ShadowShard, "shadow-shard");
    STAT(SpecialRewardItem::LavaShard, "lava-shard");
    STAT(SpecialRewardItem::BonusKey, "keys");
    STAT(SpecialRewardItem::EarthShard, "earth-shard");
    STAT(SpecialRewardItem::BloodShard, "blood-shard");
    STAT(SpecialRewardItem::MetalShard, "metal-shard");
    STAT(SpecialRewardItem::LightShard, "light-shard");
    STAT(SpecialRewardItem::SoulShard, "soul-shard");

    if (array->count() > 0)
    {
        auto item = GJRewardItem::createWithObjects((GJRewardType)-1, array);
        item->m_chestID = -80085;

        bool pickedID = false;
        std::string str = "";

        while (!pickedID)
        {
            str = fmt::format("{}{}", ""_spr, (int)(rand() % 800856969));

            if (!GameStatsManager::get()->m_miscChests->objectForKey(str))
                pickedID = true;
        }

        GameStatsManager::get()->m_miscChests->setObject(item, str);

        GameStatsManager::get()->registerRewardsFromItem(item);

        auto page = RewardUnlockLayer::create((int)GJRewardType::Large, nullptr);
        page->showCollectReward(item);
        page->show();
    }

    onClose(sender);
}

CCNode* CustomRewardSetupUI::createCreateInput(std::string id, std::string str)
{
    auto node = CCNode::create();
    node->setContentSize(ccp(70 + 2.5f * 2 + (50), 35));

    auto bg = CCScale9Sprite::create("square02b_small.png");
    bg->setColor(ccc3(0, 0, 0));
    bg->setOpacity(100);
    bg->setContentSize(node->getContentSize() / 0.5f);
    bg->setScale(0.5f);

    auto input = TextInput::create(70, "Count");
    input->setCommonFilter(CommonFilter::Int);
    input->setAnchorPoint(ccp(1, 0.5f));

    textInputs.emplace(id, input);

    auto spr = CCSprite::createWithSpriteFrameName(str.c_str());
    spr->setScale(25.0f / spr->getContentHeight());

    node->addChildAtPosition(bg, Anchor::Center);
    node->addChildAtPosition(input, Anchor::Right, ccp(-2.5f, 0));
    node->addChildAtPosition(spr, Anchor::Left, ccp(50 / 2, 0));

    return node;
}