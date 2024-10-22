#include "LabelNode.hpp"
#include "LabelLayer.hpp"
#include "../Hacks/Noclip/Noclip.hpp"
#include <chrono>

bool LabelNode::init(LabelModule* mod)
{
    if (!CCLabelBMFont::initWithString("l", mod->getFont().c_str()))
        return false;

    this->mod = mod;
    mod->labelNode = this;

    return true;
}

std::string LabelNode::getFormatString()
{
    if (mod->presetType == 1)
        return ".";

    if (mod->presetType == 2)
        return "{precision(fps, 0)} FPS";

    if (mod->presetType == 3)
        return "{total_cps} CPS";

    if (mod->presetType == 4)
        return "{noclip_accuracy}%";

    if (mod->presetType == 5)
        return "{noclip_deaths} Death{noclip_deaths == 1 ? \"\" : \"s\"}";

    if (mod->presetType == 6)
        return "Attempt {attempt}";

    return mod->format;
}

void LabelNode::update(float dt)
{
    this->setOpacity(mod->getOpacity() * 255.0f);
    this->setScale(mod->getScale() * 0.5f * (32.5f / this->getContentHeight()));

    auto res = rift::compile(getFormatString());

    script = res.unwrapOr(nullptr);
    
    if (!script)
    {
        return this->setString(fmt::format("Error Compiling Script: {}", res.getMessage()).c_str());
    }
    
    script->setVariable("attempt", rift::Value::integer(LabelLayer::get()->getAttempts()));
    script->setVariable("fps", rift::Value::floating(LabelLayer::get()->getFPS()));

    script->setVariable("player1_cps", rift::Value::integer(LabelLayer::get()->getCPS(false)));
    script->setVariable("player2_cps", rift::Value::integer(LabelLayer::get()->getCPS(true)));
    script->setVariable("total_cps", rift::Value::integer(LabelLayer::get()->getTotalCPS()));

    script->setVariable("player1_clicks", rift::Value::integer(LabelLayer::get()->getClicks(false)));
    script->setVariable("player2_clicks", rift::Value::integer(LabelLayer::get()->getClicks(true)));
    script->setVariable("total_clicks", rift::Value::integer(LabelLayer::get()->getTotalClicks()));

    std::chrono::milliseconds duration(static_cast<long long>(ColourUtility::totalSessionTime * 1000));

    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    duration -= hours;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
    duration -= minutes;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);

    script->setVariable("session_seconds", rift::Value::integer(seconds.count()));
    script->setVariable("session_minutes", rift::Value::integer(minutes.count()));
    script->setVariable("session_hours", rift::Value::integer(hours.count()));

    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);

    script->setVariable("clock_seconds", rift::Value::integer(localTime->tm_sec));
    script->setVariable("clock_minutes", rift::Value::integer(localTime->tm_min));
    script->setVariable("clock_hours", rift::Value::integer(localTime->tm_hour));

    script->setVariable("isEditor", rift::Value::boolean(LevelEditorLayer::get()));
    script->setVariable("isLevel", rift::Value::boolean(PlayLayer::get()));

    script->setVariable("noclip_deaths", rift::Value::integer(as<NoclipBaseGameLayer*>(GJBaseGameLayer::get())->getNoclipDeaths()));
    script->setVariable("noclip_accuracy", rift::Value::floating(as<NoclipBaseGameLayer*>(GJBaseGameLayer::get())->getNoclipAccuracy() * 100));

    if (PlayLayer::get())
    {
        script->setVariable("bestRun_from", rift::Value::floating(as<BestPlayLayer*>(PlayLayer::get())->m_fields->bestFrom));
        script->setVariable("bestRun_to", rift::Value::floating(as<BestPlayLayer*>(PlayLayer::get())->m_fields->bestTo));
    }

    auto res2 = script->run();

    this->setString(res2.c_str());

    if (numberOfRunningActions() == 0)
    {
        if (mod->isCheatIndicator)
            this->setColor(mod->getColour());
    }

    this->setVisible(mod->noclipOnly ? Client::GetModuleEnabled("noclip") : true);

    if (getChildrenCount() == 1 && res2 == ".")
    {
        as<CCNode*>(this->getChildren()->objectAtIndex(0))->setScale(2.25f);
        as<CCNode*>(this->getChildren()->objectAtIndex(0))->setAnchorPoint(ccp(0.2f, 0.35f));
    }
}

LabelNode* LabelNode::create(LabelModule* mod)
{
    auto pRet = new LabelNode();

    if (pRet && pRet->init(mod))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

LabelNode::~LabelNode()
{
    mod->labelNode = nullptr;
}