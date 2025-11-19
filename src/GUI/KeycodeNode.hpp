#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class KeycodeNode : public CCNode
{
    protected:
        CCScale9Sprite* bg = nullptr;
        CCLabelBMFont* label = nullptr;
        int keycode = -1;

    public:
        static KeycodeNode* create(int keycode);

        void setKeycode(int keycode);
        int getKeycode();

        virtual void setContentSize(const CCSize& contentSize);
        
        bool init(int keycode);
};