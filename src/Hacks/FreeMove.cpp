#define FREEMOVE

#ifdef FREEMOVE

#include <Geode/Geode.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

CCPoint pos = ccp(0, 0);
float speed = 1.0f;
bool a = false;
bool b = true;

class $modify (CCKeyboardDispatcher)
{
    bool dispatchKeyboardMSG(enumKeyCodes key, bool down, bool idk)
    {
        if (PlayLayer::get())
        {
            if (key == enumKeyCodes::KEY_End && down)
            {
                a = !a;

                if (a)
                    geode::Notification::create("Enabled free mode", NotificationIcon::None, 0.25f)->show();
                else
                    geode::Notification::create("Disabled free mode", NotificationIcon::None, 0.25f)->show();

                if (a)
                    pos = PlayLayer::get()->m_player1->m_position;
            }

            if (key == enumKeyCodes::KEY_F1 && down)
            {
                speed -= 0.1f;

                if (speed < 0.7f)
                {
                    speed = 0.7f;
                }

                std::stringstream ss;
                ss << "Speed decreased to ";
                ss << speed;
                ss << "x";

                geode::Notification::create(ss.str(), NotificationIcon::None, 0.25f)->show();
            }

            if (key == enumKeyCodes::KEY_F2 && down)
            {
                speed += 0.1f;

                if (speed > 5)
                {
                    speed = 5;
                }

                std::stringstream ss;
                ss << "Speed increased to ";
                ss << speed;
                ss << "x";

                geode::Notification::create(ss.str(), NotificationIcon::None, 0.25f)->show();
            }

            if (key == enumKeyCodes::KEY_F3 && down)
            {
                b = !b;

                geode::Notification::create("Toggled update call", NotificationIcon::None, 0.25f)->show();
            }
        }

        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, idk);
    }
};

class $modify (PlayerObject)
{
    virtual void update(float p0)
    {
        if (!a)
            PlayerObject::update(p0);
    }
};

class $modify (GJBaseGameLayer)
{
    virtual void update(float delta)
    {
        if (b)
            GJBaseGameLayer::update(delta);

        pos = m_player1->m_position;

        if (a) {
            if (GetAsyncKeyState(87) & 0x8000) {
                pos.y += 30 * delta * 5 * 3 * speed;
            }
            if (GetAsyncKeyState(83) & 0x8000) {
                pos.y -= 30 * delta * 5 * 3 * speed;
            }
            if (GetAsyncKeyState(65) & 0x8000) {
                pos.x -= 30 * delta * 5 * 3 * speed;
            }
            if (GetAsyncKeyState(68) & 0x8000) {
                pos.x += 30 * delta * 5 * 3 * speed;
            }

            m_player1->m_position = pos;
        }
    }
};

#endif