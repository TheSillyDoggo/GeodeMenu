#include <Geode/Geode.hpp>
#include "../../Client/ButtonModule.hpp"

class TogglePractice : public ButtonModule
{
    public:
        MODULE_SETUP(TogglePractice)
        {
            setID("shortcut/toggle-practice");
            setCategory("Level");

            setPriority(sortPriority + 2);
        }

        virtual void onClick()
        {
            if (PlayLayer::get())
                PlayLayer::get()->togglePracticeMode(!PlayLayer::get()->m_isPracticeMode);
        }

        virtual bool shouldShortcutShowActivated()
        {
            if (PlayLayer::get())
                return PlayLayer::get()->m_isPracticeMode;
            
            return false;
        }
};

SUBMIT_HACK(TogglePractice);