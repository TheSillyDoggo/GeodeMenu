#include "LabelManager.hpp"
#include "../Hacks/Speedhack/Speedhack.hpp"
#include "Nodes/LabelContainerLayer.hpp"
#include "../GUI/Categories/LabelsNode.hpp"

using namespace geode::prelude;

LabelManager* LabelManager::get()
{
    static LabelManager* instance = nullptr;

    if (!instance)
    {
        instance = new LabelManager();
        instance->load();

        instance->sessionBegin = std::chrono::steady_clock::now();
    }

    return instance;
}

const std::vector<LabelConfig>& LabelManager::getConfigs()
{
    return configs;
}

void LabelManager::setConfigs(std::vector<LabelConfig> configs)
{
    this->configs = configs;

    save();

    if (LabelContainerLayer::get())
        LabelContainerLayer::get()->updateConfigs();
    
    if (LabelsNode::get())
        LabelsNode::get()->updateUI();
}

void HideLabels::onToggle()
{
    if (LabelsNode::get())
        LabelsNode::get()->updateUI();
}

void LabelManager::load()
{
    auto arr = Mod::get()->getSavedValue<matjson::Value>("selected-labels");

    if (!arr.isArray())
        return;

    configs.clear();

    for (auto obj : arr.asArray().unwrap())
    {
        if (obj.isObject())
            configs.push_back(LabelConfig::createFromObject(obj));
    }
}

void LabelManager::save()
{
    matjson::Value value = matjson::Value::array();

    for (auto conf : configs)
    {
        value.asArray().unwrap().push_back(conf.save());
    }

    Mod::get()->setSavedValue<matjson::Value>("selected-labels", value);
}

cocos2d::CCPoint LabelManager::anchorToPoint(LabelAnchor anchor)
{
    switch (anchor)
    {
        case LabelAnchor::TopLeft:
            return ccp(0, 1);

        case LabelAnchor::TopCenter:
            return ccp(0.5f, 1);

        case LabelAnchor::TopRight:
            return ccp(1, 1);

        case LabelAnchor::CenterRight:
            return ccp(1, 0.5f);

        case LabelAnchor::BottomRight:
            return ccp(1, 0);

        case LabelAnchor::BottomCenter:
            return ccp(0.5f, 0);

        case LabelAnchor::BottomLeft:
            return ccp(0, 0);

        case LabelAnchor::CenterLeft:
            return ccp(0, 0.5f);

        case LabelAnchor::Center:
            return ccp(0.5f, 0.5f);
    
        default:
            return ccp(0, 0);
    }
}

float LabelManager::getSessionDuration()
{
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - sessionBegin).count();
}

int LabelManager::getAttemptCount()
{
    if (PlayLayer::get())
        return PlayLayer::get()->m_attempts;

    return 0;
}

float LabelManager::getFPS()
{
    return fps;
}

void LabelManager::update(float dt)
{
    _timeLeft -= dt;
    _accum += 1 / dt;
    _frames++;

    if (_timeLeft <= 0)
    {
        fps = _accum / _frames;

        _timeLeft = _updateInterval;
        _accum = 0;
        _frames = 0;
    }
}

$on_mod(Loaded)
{
    LabelManager::get();
}