#include "TextLabelNode.hpp"
#include "../../SafeMode/SafeMode.hpp"
#include "../LabelManager.hpp"
#include "../../Hacks/Level/Noclip/Hooks.hpp"
#include "LabelContainerLayer.hpp"
#include "../BestRun.hpp"

void TextLabelNode::setup()
{
    label = AdvLabelBMFont::createWithStruct({}, "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0));
    label->setTTFUsage(AdvLabelTTFUsage::None);

    this->addChild(label);
}

void TextLabelNode::labelConfigUpdated()
{
    auto font = CCFileUtils::sharedFileUtils()->isFileExist(CCFileUtils::sharedFileUtils()->fullPathForFilename(config.font.c_str(), false)) ? config.font : "bigFont.fnt";
    label->setFntFile(font.c_str());
    label->setString("L");
    label->setScale((32.5f / label->getContentHeight()) * 0.5f);

    auto anchorX = LabelManager::get()->anchorToPoint(config.anchor).x;

    label->setAlignment(anchorX == 0 ? kCCTextAlignmentLeft : (anchorX == 0.5f ? kCCTextAlignmentCenter : kCCTextAlignmentRight));
    
    CC_SAFE_DELETE(script);
    script = rift::compile(config.formatString).unwrapOr(nullptr);
}

ccColor3B TextLabelNode::getDesiredColour()
{
    return config.cheatIndicator ? SafeMode::get()->getIndicatorColour() : label->getColor();
}

void TextLabelNode::update(float dt)
{
    if (!isActionActive())
    {
        label->setOpacity(config.opacity * 255);
        label->setColor(getDesiredColour());
    }

    std::string str = "Error";

    if (script)
    {
        updateVariables();
        
        str = script->run();
    }

    label->setString(str.c_str());
    this->setContentSize(label->getScaledContentSize());

    if (label->getVisibleLabels().size() == 1 && str == ".")
    {
        auto anchorX = LabelManager::get()->anchorToPoint(config.anchor).x;

        static_cast<CCNode*>(label->getVisibleLabels()[0]->getChildren()->objectAtIndex(0))->setScale(2.25f);
        static_cast<CCNode*>(label->getVisibleLabels()[0]->getChildren()->objectAtIndex(0))->setAnchorPoint(ccp(anchorX == 0 ? 0.2f : (anchorX == 1.0f ? 0.6f : 0.45f), 0.35f));
    }
}

void TextLabelNode::onEventTriggered(LabelEventType type)
{
    for (auto event : config.events)
    {
        if (event.type == type)
        {
            label->stopAllActions();

            auto array = CCArray::create();
            array->retain();

            if (event.fadeIn != -1)
                array->addObject(CCTintTo::create(event.fadeIn, event.colour.r, event.colour.g, event.colour.b));

            if (event.hold != -1)
                array->addObject(CCDelayTime::create(event.hold));

            if (event.fadeOut != -1)
                array->addObject(CCTintTo::create(event.fadeOut, ccWHITE.r, ccWHITE.g, ccWHITE.b));

            auto seq = CCSequence::create(array);
            seq->setTag(80085);

            label->runAction(seq);

            array->release();

            array = CCArray::create();
            array->retain();

            if (event.fadeIn != -1)
                array->addObject(CCFadeTo::create(event.fadeIn, event.colour.a));

            if (event.hold != -1)
                array->addObject(CCDelayTime::create(event.hold));

            if (event.fadeOut != -1)
                array->addObject(CCFadeTo::create(event.fadeOut, config.opacity * 255));

            seq = CCSequence::create(array);
            seq->setTag(800851);

            label->runAction(seq);

            array->release();
        }
    }
}

void TextLabelNode::updateVariables()
{
    if (!GJBaseGameLayer::get())
        return;

    auto noclipBGL = static_cast<NoclipBaseGameLayer*>(GJBaseGameLayer::get());

    std::chrono::milliseconds duration(static_cast<long long>(LabelManager::get()->getSessionDuration() * 1000));

    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    duration -= hours;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
    duration -= minutes;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);

    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);

    script->setVariable("attempt", rift::Value::integer(LabelManager::get()->getAttemptCount()));
    script->setVariable("fps", rift::Value::floating(LabelManager::get()->getFPS()));

    script->setVariable("player1_cps", rift::Value::integer(LabelContainerLayer::get()->getCPS(NoclipPlayerSelector::Player1)));
    script->setVariable("player2_cps", rift::Value::integer(LabelContainerLayer::get()->getCPS(NoclipPlayerSelector::Player2)));
    script->setVariable("total_cps", rift::Value::integer(LabelContainerLayer::get()->getCPS(NoclipPlayerSelector::All)));

    script->setVariable("player1_max_cps", rift::Value::integer(LabelContainerLayer::get()->getHighestCPS(NoclipPlayerSelector::Player1)));
    script->setVariable("player2_max_cps", rift::Value::integer(LabelContainerLayer::get()->getHighestCPS(NoclipPlayerSelector::Player2)));
    script->setVariable("max_cps", rift::Value::integer(LabelContainerLayer::get()->getHighestCPS(NoclipPlayerSelector::All)));

    script->setVariable("player1_clicks", rift::Value::integer(LabelContainerLayer::get()->getTotalClicks(NoclipPlayerSelector::Player1)));
    script->setVariable("player2_clicks", rift::Value::integer(LabelContainerLayer::get()->getTotalClicks(NoclipPlayerSelector::Player2)));
    script->setVariable("total_clicks", rift::Value::integer(LabelContainerLayer::get()->getTotalClicks(NoclipPlayerSelector::All)));

    script->setVariable("session_seconds", rift::Value::integer(seconds.count()));
    script->setVariable("session_minutes", rift::Value::integer(minutes.count()));
    script->setVariable("session_hours", rift::Value::integer(hours.count()));

    script->setVariable("clock_seconds", rift::Value::integer(localTime->tm_sec));
    script->setVariable("clock_minutes", rift::Value::integer(localTime->tm_min));
    script->setVariable("clock_hours", rift::Value::integer(localTime->tm_hour));

    script->setVariable("isEditor", rift::Value::boolean(LevelEditorLayer::get()));
    script->setVariable("isLevel", rift::Value::boolean(PlayLayer::get()));

    script->setVariable("noclip_deaths", rift::Value::integer(noclipBGL->getNoclipDeaths(NoclipPlayerSelector::All)));
    script->setVariable("noclip_accuracy", rift::Value::floating(noclipBGL->getNoclipAccuracy(NoclipPlayerSelector::All) * 100));
    script->setVariable("player1_noclip_deaths", rift::Value::integer(noclipBGL->getNoclipDeaths(NoclipPlayerSelector::Player1)));
    script->setVariable("player1_noclip_accuracy", rift::Value::floating(noclipBGL->getNoclipAccuracy(NoclipPlayerSelector::Player1) * 100));
    script->setVariable("player2_noclip_deaths", rift::Value::integer(noclipBGL->getNoclipDeaths(NoclipPlayerSelector::Player2)));
    script->setVariable("player2_noclip_accuracy", rift::Value::floating(noclipBGL->getNoclipAccuracy(NoclipPlayerSelector::Player2) * 100));

    if (auto lvl = GJBaseGameLayer::get()->m_level)
    {
        script->setVariable("level_name", rift::Value::string(lvl->m_levelName));
        script->setVariable("level_creator", rift::Value::string(lvl->m_creatorName.empty() ? "RobTop" : lvl->m_creatorName));
        // script->setVariable("level_description", rift::Value::string(lvl->getUnpackedLevelDescription()));
        script->setVariable("level_upload", rift::Value::string(lvl->m_uploadDate));
        script->setVariable("level_update", rift::Value::string(lvl->m_updateDate));
        script->setVariable("level_likes", rift::Value::integer(lvl->m_likes));
        script->setVariable("level_downloads", rift::Value::integer(lvl->m_downloads));
        script->setVariable("level_id", rift::Value::integer(lvl->m_levelID.value()));
        script->setVariable("level_verified", rift::Value::boolean(lvl->m_isVerifiedRaw));
        script->setVariable("level_object_count", rift::Value::integer(lvl->m_objectCount.value()));
        script->setVariable("level_version", rift::Value::integer(lvl->m_levelVersion));
        script->setVariable("level_game_version", rift::Value::integer(lvl->m_gameVersion));

        script->setVariable("normal_best", rift::Value::integer(GJBaseGameLayer::get()->m_level->m_normalPercent.value()));
        script->setVariable("practice_best", rift::Value::integer(GJBaseGameLayer::get()->m_level->m_practicePercent));
    }

    if (PlayLayer::get())
    {
        script->setVariable("bestRun_from", rift::Value::floating(static_cast<BestPlayLayer*>(PlayLayer::get())->m_fields->bestFrom));
        script->setVariable("bestRun_to", rift::Value::floating(static_cast<BestPlayLayer*>(PlayLayer::get())->m_fields->bestTo));
        script->setVariable("percentage", rift::Value::floating(PlayLayer::get()->getCurrentPercent()));
        script->setVariable("last_percentage", rift::Value::floating(static_cast<BestPlayLayer*>(PlayLayer::get())->m_fields->lastPercent));
        script->setVariable("run_from", rift::Value::floating(static_cast<BestPlayLayer*>(PlayLayer::get())->m_fields->fromPercent));
        script->setVariable("isPractice", rift::Value::boolean(PlayLayer::get()->m_isPracticeMode));
    }
}

TextLabelNode::~TextLabelNode()
{
    CC_SAFE_DELETE(script);
}

bool TextLabelNode::isActionActive()
{
    return label->getActionByTag(80085) || label->getActionByTag(800851);
}