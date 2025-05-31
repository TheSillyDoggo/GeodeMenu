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
	spr->setScale(1.f);

	const auto label = TransLabelBMFont::create("Reset Audio Sliders", "bigFont.fnt");
	label->setScale(0.75f);

	spr->setContentSize({ label->getScaledContentWidth() + 20.0f, 40.0f }); // magical numbers

	label->setAnchorPoint({0.5f, 0.5f});
	label->setPosition(spr->getContentSize() / 2);
	spr->addChild(label);

	// the anchors drive me crazy
	const auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(ResetAudioModule::onResetAudio));
	btn->setPosition({ menu->getContentWidth()/2, menu->getContentHeight()/2 });

	menu->addChild(btn);
}
// ReSharper disable once CppMemberFunctionMayBeStatic
void ResetAudioModule::onResetAudio(CCObject* sender)
{
	// what it says on the tin
	FMODAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.f);
	FMODAudioEngine::sharedEngine()->setEffectsVolume(1.f);
}
