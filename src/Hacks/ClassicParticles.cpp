/*#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (PlayerObject)
{
	virtual void update(float p0)
	{
		PlayerObject::update(p0);

		#ifdef GEODE_IS_ANDROID
		auto particle = m_unk6e4;
		#else
		auto particle = m_shipClickParticles;
		#endif

		particle->setGrouped()
	}
};*/