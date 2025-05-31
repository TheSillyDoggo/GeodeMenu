#pragma once

#include <Geode/Geode.hpp>
#include "../Module.h"

using namespace geode::prelude;

class ResetAudioModule : public Module
{
	public:
		ResetAudioModule();

		void save();
		void load();

		void makeAndroid(CCNode* menu, CCPoint pos);


	private:
		void onResetAudio(CCObject* sender);
};