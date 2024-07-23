#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (BestPlayLayer, PlayLayer)
{
	struct Fields {
		float fromPercent;
        float toPercent;

        float bestFrom;
        float bestTo;
        float bestLength;
	};

	void resetLevel();
    std::string getRunString();
};