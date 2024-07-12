#pragma once

#include "../Window.h"

class Universal : public Window {
public:
    void onTransCustomizer(CCObject*) {}

    void cocosCreate(CCMenu* menu);
};