#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ThreadedLabelBMFont : public CCNode
{
    private:
        CCLabelBMFont* label;
        std::string text;
        std::string font;
        MiniFunction<void(ThreadedLabelBMFont*)> callback;

        static inline std::vector<ThreadedLabelBMFont*> labels = {};

        void addLabel();
        void queueStep();
    
    public:
        CCLabelBMFont* getLabel();
        std::string getFont();

        bool init(std::string text, std::string font, MiniFunction<void(ThreadedLabelBMFont*)> callback);

        // Callback is always run on main thread
        static ThreadedLabelBMFont* create(std::string text, std::string font, MiniFunction<void(ThreadedLabelBMFont*)> callback = nullptr);
};