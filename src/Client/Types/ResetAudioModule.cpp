//
// Created by Leslie on 5/31/2025.
//

#include "ResetAudioModule.hpp"
#include "../../UI/TransLabelBMFont.hpp"

ResetAudioModule::ResetAudioModule()
{
	addToCache();
};

void ResetAudioModule::save()
{
	// Mod::get()->setSavedValue<bool>(fmt::format("{}_reset", id), reset);
}

void ResetAudioModule::load()
{
	// reset = Mod::get()->getSavedValue<bool>(fmt::format("{}_reset", id), reset);
}

void ResetAudioModule::makeAndroid(CCNode* menu, CCPoint pos)
{
	const auto spr = CCScale9Sprite::create("geode.loader/GE_button_04.png");
	spr->setScale(0.7f);

	const auto label = TransLabelBMFont::create("Reset Audio Sliders", "bigFont.fnt");
	label->setScale(0.75f);

	spr->setContentSize({ label->getScaledContentWidth() + 20.0f, 40.0f }); // magical numbers\

	label->setAnchorPoint({0.5f, 0.5f});
	label->setPosition(spr->getContentSize() / 2);
	spr->addChild(label);

	// the anchors drive me crazy
	auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(ResetAudioModule::onResetAudio));
	btn->setPosition({ menu->getContentWidth()/2, menu->getContentHeight()/2 });

	auto info = InfoAlertButton::create("Reset Audio Sliders", "Resets the audio sliders to 100% incase it went off screen and you can't get it back.", 0.6f);
	info->setPosition(btn->getPosition() + ccp(105, 20));

	menu->addChild(btn);
	menu->addChild(info);
}

void ResetAudioModule::onResetAudio(CCObject* sender)
{
	FMODAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0f);
	FMODAudioEngine::sharedEngine()->setEffectsVolume(1.0f);

	// OptionsLayer is added to the scene, not a child of MenuLayer
	if (auto optionsLayer = CCScene::get()->getChildByType<OptionsLayer>(0))
	{
		if (auto mainLayer = optionsLayer->getChildByType<CCLayer>(0))
		{
			for (int i = 0; i <= 1; i++) {
				if (auto slider = mainLayer->getChildByType<Slider>(i)) {
					slider->setValue(1);
					slider->updateBar();
				}
			}
		}
	}

	// PauseLayer as well
	if (auto pauseLayer = CCScene::get()->getChildByType<PauseLayer>(0))
	{
		for (int i = 0; i <= 1; i++) {
			if (auto slider = pauseLayer->getChildByType<Slider>(i)) {
				slider->setValue(1);
				slider->updateBar();
			}
		}
	}
}
