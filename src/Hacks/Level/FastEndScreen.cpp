#include "../../Client/Module.hpp"
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class FastEndScreen : public Module
{
    public:
        MODULE_SETUP(FastEndScreen)
        {
            setID("fast-end-screen");
            setCategory("Level");
        }
};

SUBMIT_HACK(FastEndScreen);

class $modify (PlayLayer)
{
    struct Fields
    {
        bool played = false;
    };

    void playEndAnimationToPos(cocos2d::CCPoint position)
    {
        PlayLayer::playEndAnimationToPos(position);

        if (FastEndScreen::get()->getRealEnabled())
            PlayLayer::showCompleteText();
    }

    void playPlatformerEndAnimationToPos(cocos2d::CCPoint position, bool instant)
    {
        PlayLayer::playPlatformerEndAnimationToPos(position, instant);

        if (FastEndScreen::get()->getRealEnabled())
            PlayLayer::showCompleteText();
    }

    void showCompleteText()
    {
        m_fields->played = !m_fields->played;

        if (FastEndScreen::get()->getRealEnabled() && !m_fields->played)
            return;

        PlayLayer::showCompleteText();
    }
};