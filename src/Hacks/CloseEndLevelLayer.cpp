#include <Geode/Geode.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

/*

class EndLevelPopup
{
	public:
		static inline EndLevelLayer* ins = nullptr;
		static inline CCNode* arr = nullptr;

		void toggleVisible(CCObject* sender)
		{
			// Create a move-to action with a specified duration and target position
			auto moveToAction = CCMoveTo::create(1.0f, ccp(0.0f, CCDirector::get()->getWinSize().height + 5));

			// Create an ease-in-out action with the move-to action and a rate
			auto easeInOutAction = CCEaseInOut::create(moveToAction, 2.0f);

			// Create a fade-to action with a specified duration and opacity
			auto fadeToAction = CCFadeTo::create(1.0f, 0);

			// Run the fade-to action on the CCNode

			CCArray* ar = CCArray::create();
			ar->addObject(easeInOutAction);
			ar->addObject(CCDelayTime::create(0.1f));
			ar->addObject(CCCallFuncN::create(arr, callfuncN_selector(EndLevelPopup::startArrowOut)));

			auto act = CCSequence::create(ar);

			ins->runAction(fadeToAction);
			safe_cast<CCNode*>(ins->getChildren()->objectAtIndex(0))->runAction(act);
		}

		void startIn(CCObject* sender)
		{
			// Create a move-to action with a specified duration and target position
			auto moveToAction = CCMoveTo::create(1.0f, ccp(0.0f, 5));

			// Create an ease-in-out action with the move-to action and a rate
			auto easeInOutAction = CCEaseBounceOut::create(moveToAction);

			// Create a fade-to action with a specified duration and opacity
			auto fadeToAction = CCFadeTo::create(1.0f, 100);

			// Run the fade-to action on the CCNode
			ins->runAction(fadeToAction);
			safe_cast<CCNode*>(ins->getChildren()->objectAtIndex(0))->runAction(easeInOutAction);

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

		if (Client::GetModuleEnabled("end-screen"))
			return;

		if (safe_cast<CCLayer*>(getChildren()->objectAtIndex(0)))
		{
			CCLayer* a = safe_cast<CCLayer*>(getChildren()->objectAtIndex(0));

			if (a)
			{
				CCLayer* b = safe_cast<CCMenu*>(a->getChildren()->objectAtIndex(9));

				if (b)
				{
					CCSprite* btnSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
					if (btnSpr)
					{
						btnSpr->setRotation(90);
						btnSpr->setScale(0.65f);

						CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(btnSpr, b, menu_selector(EndLevelPopup::toggleVisible));

						btn->setPosition(ccp(0, safe_cast<CCNode*>(b->getChildren()->objectAtIndex(0))->getPositionY() * -1 + 2));
						btn->setID("hideButton");

						b->addChild(btn);
					}

					CCSprite* btnSpr2 = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
					btnSpr2->setRotation(-90);
					btnSpr2->setScale(0.65f);

					EndLevelPopup::ins = this;
					//CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(btnSpr, b, menu_selector(EndLevelPopup::toggleVisible));

					CCMenuItemSpriteExtra* btn2 = CCMenuItemSpriteExtra::create(btnSpr2, b, menu_selector(EndLevelPopup::bringBack));
					btn2->setVisible(false);
					btn2->setID("showButton");

					EndLevelPopup::arr = btn2;

					btn2->setPosition(ccp(0, -1 * (CCDirector::get()->getWinSize().height / 2 + 20)));

					b->addChild(btn2);
				}
			}
		}
	}
};

*/