#pragma once

#include "../Window.h"

class Variables : public Window {
public:
    Variables() {
        name = "Variables";
        id = "vars-window";
    }

    void onChangeMode(CCObject* sender);

    void cocosCreate(CCMenu* menu);
};