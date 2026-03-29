#pragma once

#include <BetterAlertLayer.hpp>

class Module;

namespace qolmod
{
    class ModuleInfoAlert : public BetterAlertLayer
    {
        protected:
            Module* mod = nullptr;

        public:
            static ModuleInfoAlert* create(Module* module);

            void onToggleFavourite(CCObject* sender);
            void onChangeKeybind(CCObject* sender);
            void onChangeShortcut(CCObject* sender);

            bool init(Module* module);
    };
};