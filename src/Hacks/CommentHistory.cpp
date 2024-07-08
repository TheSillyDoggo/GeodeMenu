#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (ProfilePage)
{
	void loadPageFromUserInfo(GJUserScore* p0)
	{
		auto state = p0->m_commentHistoryStatus;
		p0->m_commentHistoryStatus = 0;

		ProfilePage::loadPageFromUserInfo(p0);

		p0->m_commentHistoryStatus = state;
	}


	QOLMOD_MOD_HOOK("comment-history", "ProfilePage::loadPageFromUserInfo")
};