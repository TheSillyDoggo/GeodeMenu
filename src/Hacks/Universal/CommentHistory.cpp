#include "../../Client/Module.hpp"
#include <Geode/modify/ProfilePage.hpp>

using namespace geode::prelude;

class CommentHistoryBypass : public Module
{
    public:
        MODULE_SETUP(CommentHistoryBypass)
        {
            setName("Comment History Bypass");
            setID("comment-history");
            setCategory("Universal");
            setDescription("Allows you to see the comment history of any player");
        }
};

SUBMIT_HACK(CommentHistoryBypass)

class $modify (ProfilePage)
{
	void loadPageFromUserInfo(GJUserScore* p0)
	{
		auto state = p0->m_commentHistoryStatus;

        if (CommentHistoryBypass::get()->getRealEnabled())
		    p0->m_commentHistoryStatus = 0;

		ProfilePage::loadPageFromUserInfo(p0);

		p0->m_commentHistoryStatus = state;
	}
};