#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class PlayerDeathAnimation : public CCNode
{
    private:
        std::vector<CCSprite*> sprites;
        int index = 0;
        bool isDefault;

        void tick(float);

    public:
        bool init(int id);
        void run();

        static PlayerDeathAnimation* create(int id);
        static PlayerDeathAnimation* createAndRun(int id);
};