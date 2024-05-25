#ifndef GEODE_IS_MACOS

#include <Geode/Geode.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class EndLevelPopup
{
	public:
		static inline EndLevelLayer* ins = nullptr;
		static inline CCNode* arr = nullptr;

		void toggleVisible(CCObject* sender)
		{
			auto moveToAction = CCMoveTo::create(1.0f, ccp(0.0f, CCDirector::get()->getWinSize().height + 5));

			auto easeInOutAction = CCEaseInOut::create(moveToAction, 2.0f);
			auto fadeToAction = CCFadeTo::create(1.0f, 0);

			CCArray* ar = CCArray::create();
			ar->addObject(easeInOutAction);
			ar->addObject(CCDelayTime::create(0.1f));
			ar->addObject(CCCallFuncN::create(arr, callfuncN_selector(EndLevelPopup::startArrowOut)));

			auto act = CCSequence::create(ar);

			ins->runAction(fadeToAction);
			ins->m_mainLayer->runAction(act);
		}

		void startIn(CCObject* sender)
		{
			auto moveToAction = CCMoveTo::create(1.0f, ccp(0.0f, 5));

			auto easeInOutAction = CCEaseBounceOut::create(moveToAction);

			auto fadeToAction = CCFadeTo::create(1.0f, 100);

			ins->runAction(fadeToAction);
			ins->m_mainLayer->runAction(easeInOutAction);

			arr->setVisible(false);
		}

		void startArrowOut(CCObject* sender)
		{
			arr->setVisible(true);

			arr->setPositionY(-1 * (CCDirector::get()->getWinSize().height / 2 - 10));

			arr->runAction(CCEaseBackOut::create(CCMoveBy::create(0.25f, ccp(0, -30))));
		}

		void bringBack(CCObject* sender)
		{
			CCArray* ar = CCArray::create();
			ar->addObject(CCEaseInOut::create(CCMoveBy::create(0.25f, ccp(0, 30)), 2.0f));
			ar->addObject(CCDelayTime::create(0.1f));
			ar->addObject(CCCallFuncN::create(arr, callfuncN_selector(EndLevelPopup::startIn)));

			auto act = CCSequence::create(ar);

			arr->runAction(act);
		}
};

class $modify(EndLevelLayer)
{
	void toggleVisible(CCObject * sender)
	{
		this->runAction(CCMoveTo::create(1, ccp(0, CCDirector::get()->getWinSize().height + 5)));
	}

	void customSetup()
	{
		EndLevelLayer::customSetup();

		if (auto m_buttonMenu = getChildOfType<CCMenu>(m_mainLayer, -1))
		{
			CCSprite* btnSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
			if (btnSpr)
			{
				btnSpr->setRotation(90);
				btnSpr->setScale(0.65f);

				CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(btnSpr, m_buttonMenu, menu_selector(EndLevelPopup::toggleVisible));

				btn->setPosition(ccp(0, as<CCNode*>(m_buttonMenu->getChildren()->objectAtIndex(0))->getPositionY() * -1 + 2));
				btn->setID("hideButton");

				m_buttonMenu->addChild(btn);
			}

			CCSprite* btnSpr2 = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
			btnSpr2->setRotation(-90);
			btnSpr2->setScale(0.65f);

			EndLevelPopup::ins = this;
			//CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(btnSpr, b, menu_selector(EndLevelPopup::toggleVisible));

			CCMenuItemSpriteExtra* btn2 = CCMenuItemSpriteExtra::create(btnSpr2, m_buttonMenu, menu_selector(EndLevelPopup::bringBack));
			btn2->setVisible(false);
			btn2->setID("showButton");

			EndLevelPopup::arr = btn2;

			btn2->setPosition(ccp(0, -1 * (CCDirector::get()->getWinSize().height / 2 + 20)));

			m_buttonMenu->addChild(btn2);
		}
	}

	static void onModify(auto& self) {
        self.setHookPriority("EndLevelLayer::customSetup", 69420);
    }
};

#endif