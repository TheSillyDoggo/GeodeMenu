#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify (BestPlayLayer, PlayLayer)
{
	struct Fields {
		float fromPercent;
        float toPercent;

        float bestFrom;
        float bestTo;
        float bestLength;

        bool ignoreBest;

        float lastPercent;
	};

	void resetLevel();
    virtual void destroyPlayer(PlayerObject* p0, GameObject* p1);

    std::string getRoundedString(float f);
    std::string getRunString();
};