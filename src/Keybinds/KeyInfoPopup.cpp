#include "KeyInfoPopup.hpp"

KeyInfoPopup* KeyInfoPopup::createWithKeyAndBind(int key, SetBindNode* node)
{
    KeyInfoPopup* pRet = new KeyInfoPopup();
    
    pRet->key = key;
    pRet->node = node;

    if (pRet && pRet->initWithSizeAndName(ccp(230, 210), "Edit Keybind")) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return nullptr;
    }
}

void KeyInfoPopup::customSetup()
{
    auto lbl = CCLabelBMFont::create(CCKeyboardDispatcher::get()->keyToString(as<enumKeyCodes>(key)), "bigFont.fnt");

    auto lblBG = CCScale9Sprite::create("black-square.png");
    lblBG->setContentWidth(lbl->getScaledContentSize().width + 5);
    lblBG->addChildAtPosition(lbl, Anchor::Center);

    auto removeSpr = CCScale9Sprite::create("white-square.png");
    removeSpr->setColor(ccc3(255, 0, 0));
    removeSpr->setScale(30.0f / 32.5f);

    auto removeLbl = CCLabelBMFont::create("Remove", "bigFont.fnt");
    removeLbl->setScale(0.9f);
    removeSpr->setContentSize(removeLbl->getContentSize() + ccp(0, 0));
    removeSpr->addChildAtPosition(removeLbl, Anchor::Center, ccp(0, 1));

    auto removeBtn = CCMenuItemSpriteExtra::create(removeSpr, this, menu_selector(KeyInfoPopup::onRemove));

    baseLayer->addChildAtPosition(removeBtn, Anchor::Bottom, ccp(removeBtn->getContentWidth() / 2 - ok->getContentWidth() / 2, 23));
    baseLayer->addChildAtPosition(lblBG, Anchor::Center, ccp(0, 10));

    ok->setPositionX(ok->getPositionX() - removeBtn->getContentWidth() / 2);
}

void KeyInfoPopup::onRemove(CCObject* sender)
{
    geode::createQuickPopup(
        "Delete bind",
        "Are you sure you want to <cr>delete</c> this bind?",
        "Cancel", "Delete",
        [this, sender](FLAlertLayer*, bool btn2) {
            if (btn2)
            {
                if (node->buttons.size() <= 1)
                {
                    this->onClose(nullptr);

                    return FLAlertLayer::create("Error", "You <cr>must</c> have at least <cl>one</c> bind.", "OK")->show();
                }

                node->buttons.erase(std::find(node->buttons.begin(), node->buttons.end(), key));
                node->btns.find(key)->second->removeFromParent();
                node->btns.erase(key);
                
                node->updateLayout();
                node->changed();

                this->onClose(nullptr);
            }
        }
    );
}