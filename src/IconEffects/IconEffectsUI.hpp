#pragma once

#include "../Client/CategoryNode.hpp"
#include "IconicCommon.hpp"

class IconEffectsUI : public CategoryNode
{
    protected:
        

    public:
        CREATE_FUNC(IconEffectsUI)

        void onTest(CCObject* sender);
        void onTest2(CCObject* sender);
        void onTest3(CCObject* sender);

        CCNode* getPreviewNode(IconicGamemodeType gamemode);

        virtual void update(float dt);
        bool init();
};

SUBMIT_CATEGORY("Icon Effects", IconEffectsUI)