#pragma once

#include "../Window.h"

class Level : public Window {
public:
    void onFix(CCObject*);

    void onFPS(CCObject*) {}

    void onUnc(CCObject* sender);
};