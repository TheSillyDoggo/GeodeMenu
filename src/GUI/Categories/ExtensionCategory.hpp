#pragma once

#include "../../Client/CategoryNode.hpp"
#include "../../Client/ExtensionManager.hpp"

class ExtensionCategory : public CategoryNode
{
    protected:
        qolmod::ext::CategoryData* data = nullptr;
        cocos2d::CCMenu* menu = nullptr;

    public:
        static ExtensionCategory* create(qolmod::ext::CategoryData* data);

        bool init(qolmod::ext::CategoryData* data);
};