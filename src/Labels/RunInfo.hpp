#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (RunPlayLayer, PlayLayer)
{
	struct Fields
    {
        float fromPercent;
	};

	void resetLevel();
};