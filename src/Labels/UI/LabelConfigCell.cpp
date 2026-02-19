#include "LabelConfigCell.hpp"
#include "../LabelManager.hpp"
#include "../../GUI/GrabNodeLayer.hpp"
#include "../../Hacks/Speedhack/Speedhack.hpp"
#include "SetupLabelConfigUI.hpp"

LabelConfigCell* LabelConfigCell::create(CCSize size, LabelConfig config, int index)
{
    auto pRet = new LabelConfigCell();

    if (pRet && pRet->init(size, config, index))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool LabelConfigCell::init(CCSize size, LabelConfig config, int index)
{
    if (!CCNode::init())
        return false;

    this->size = size;
    this->config = config;
    this->index = index;

    this->setContentSize(size);

    bg = NineSlice::create("square02b_001.png");
    bg->setColor(ccc3(0, 0, 0));
    bg->setOpacity(100);
    bg->setID("background");
    bg->setAnchorPoint(ccp(0.5f, 0.5f));
    bg->setScale(1.0f / 2.5f);
    bg->setContentHeight(size.height - 4);
    bg->setContentWidth(size.width - 4);
    bg->setContentSize(bg->getContentSize() * 2.5f);

    bool vis = HideLabels::get()->getRealEnabled() ? false : config.visible;

    auto nameLbl = CCLabelBMFont::create(config.displayName.c_str(), "bigFont.fnt");
    nameLbl->setAnchorPoint(ccp(0, 0.5f));
    nameLbl->limitLabelWidth(120, 0.375f, 0);
    nameLbl->setOpacity(vis ? 255 : 150);

    for (size_t i = 0; i < 4; i++)
    {
        auto back = NineSlice::create("square02_small.png");
        back->setOpacity(100);
        back->setContentSize(ccp(18, 18) * 3);
        back->setScale(1.0f / 3.0f);
        back->setZOrder(1);
        
        this->addChildAtPosition(back, Anchor::Right, ccp((-back->getScaledContentWidth() / 2 - (7 / 2)) - 2 - ((back->getScaledContentWidth() + 3.5f / 2) * i), 0));
    }

    auto btnsMenu = CCMenu::create();
    btnsMenu->setZOrder(2);
    
    auto optionsSpr = CCSprite::createWithSpriteFrameName("accountBtn_settings_001.png");
    optionsSpr->setScale(0.4f);
    auto optionsBtn = CCMenuItemSpriteExtra::create(optionsSpr, this, menu_selector(LabelConfigCell::onOptions));

    auto deleteSpr = CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
    deleteSpr->setScale(0.55f);

    auto deleteBtn = CCMenuItemSpriteExtra::create(deleteSpr, this, menu_selector(LabelConfigCell::onDelete));
    deleteBtn->setPositionX(-19.75f);

    auto grabSpr = CCSprite::create("draggable.png"_spr);
    grabSpr->setPositionX(-39.5f);
    grabSpr->setScale(0.6f);

    auto grab = GrabNodeLayer::create();
    grab->setContentSize(ccp(size.height - 2, size.height - 2));
    grab->setPosition(grabSpr->getPosition());
    grab->setNodeToGrab(this);
    grab->setLockedAxis(LockedAxis::Vertical);
    grab->setOnStartDrag([this]
    {
        this->runAction(CCEaseInOut::create(CCScaleTo::create(0.2f, 0.95f), 2));

        this->setZOrder(42069);
    });
    grab->setOnMoveDrag([this]
    {
        auto cells = CCArrayExt<LabelConfigCell*>(getParent()->getChildren());

        std::sort(cells.begin(), cells.end(), [](CCNode* a, CCNode* b)
        {
            return a->getPositionY() > b->getPositionY();
        });

        for (size_t i = 0; i < cells.size(); i++)
        {
            if (cells[i] != this)
            {
                auto pos = ccp(getContentWidth() / 2, getParent()->getContentHeight() - (getContentHeight() * (i + 1)) + getContentHeight() / 2);
                cells[i]->stopAllActions();
                cells[i]->runAction(CCEaseInOut::create(CCMoveTo::create(0.15f, pos), 2));
            }
        }
    });
    grab->setOnEndDrag([this]
    {
        auto cells = CCArrayExt<LabelConfigCell*>(getParent()->getChildren());

        std::sort(cells.begin(), cells.end(), [](CCNode* a, CCNode* b)
        {
            return a->getPositionY() > b->getPositionY();
        });

        int ind = 0;
        LabelConfigCell* last = nullptr;
        int lastInd = -1;

        for (size_t i = 0; i < cells.size(); i++)
        {
            if (cells[i] == this)
            {
                ind = i;
                break;
            }

            last = cells[i];
            lastInd = i;
        }

        auto confs = LabelManager::get()->getConfigs();

        confs.erase(confs.begin() + this->index);
        confs.insert(confs.begin() + lastInd + (lastInd == -1 ? 1 : 1), this->config);
        
        LabelManager::get()->setConfigs(confs);
    });

    visibleToggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(LabelConfigCell::onToggleVisible), 0.45f);
    visibleToggler->setPositionX(-59.25f);
    visibleToggler->toggle(config.visible);

    btnsMenu->addChild(optionsBtn);
    btnsMenu->addChild(deleteBtn);
    btnsMenu->addChild(grabSpr);
    btnsMenu->addChild(grab);
    btnsMenu->addChild(visibleToggler);

    this->addChildAtPosition(bg, Anchor::Center);
    this->addChildAtPosition(nameLbl, Anchor::Left, ccp(7, 0));
    this->addChildAtPosition(btnsMenu, Anchor::Right, ccp((-18 / 2 - (7 / 2)) - 2, 0));
    return true;
}

void LabelConfigCell::onOptions(CCObject* sender)
{
    auto ui = SetupLabelConfigUI::create([this](LabelConfig conf)
    {
        auto confs = LabelManager::get()->getConfigs();

        confs[index] = conf;
        
        LabelManager::get()->setConfigs(confs);
    }, config.type);

    auto confs = LabelManager::get()->getConfigs();

    ui->setStartConfig(confs[index]);
    ui->show();
}

void LabelConfigCell::onDelete(CCObject* sender)
{
    geode::createQuickPopup("Delete Label",
        "Are you sure you want to <cr>delete</c> this <cc>label</c>?",
        "Cancel",
        "Delete",
        [this](FLAlertLayer* alert, bool right){
            if (right)
            {
                auto confs = LabelManager::get()->getConfigs();

                confs.erase(confs.begin() + index);

                LabelManager::get()->setConfigs(confs);
            }
        }
    );
}

void LabelConfigCell::onToggleVisible(CCObject* sender)
{
    auto confs = LabelManager::get()->getConfigs();

    confs[index].visible = !confs[index].visible;

    LabelManager::get()->setConfigs(confs);
}