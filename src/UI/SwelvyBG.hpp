/*#pragma once

#include <Geode/ui/General.hpp>
#include <Geode/utils/ColorProvider.hpp>

using namespace geode::prelude;

class SwelvyBG : public CCNodeRGBA {
    protected:
        std::vector<ListenerHandle> m_colorHandles;
        Ref<cocos2d::CCClippingNode> clip = nullptr;

        void updateSpritePosition(float dt);
        void animateInAll();

        virtual bool init();
        virtual void visit();

    public:
        static SwelvyBG* create();
};*/