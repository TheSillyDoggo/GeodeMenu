#pragma once

struct KeyState
{
    bool shiftHeld = false;
    bool ctrlHeld = false;
    bool altHeld = false;
    bool cmdHeld = false;

    int code = -1;
    bool isDown = false;
    bool isRepeat = false;
};