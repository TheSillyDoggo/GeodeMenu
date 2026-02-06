#pragma once

#include <Geode/Geode.hpp>
#include "../Utils/AdvancedLabel/AdvLabelBMFont.hpp"

enum class VersionInfoType
{
    Geode,
    GD,
    GeodeAndGD,
    QOLMod
};

class VersionInfoNode : public cocos2d::CCNodeRGBA
{
    protected:
        AdvLabelBMFont* label = nullptr;
        LoadingCircleSprite* circle = nullptr;
        VersionInfoType type = VersionInfoType::GD;
        geode::async::TaskHolder<geode::Mod::CheckUpdatesTask> updateListener;
        bool updateChecksFinished = false;
        static inline bool updateAvailable = false;

        bool isUpdateAvailable();
        bool isChecking();
        void checkForUpdate();
    
    public:
        static VersionInfoNode* create(VersionInfoType type);

        virtual void setColor(const ccColor3B& color);
        virtual void setOpacity(GLubyte opacity);

        void updateLabel();
        bool init(VersionInfoType type);
};