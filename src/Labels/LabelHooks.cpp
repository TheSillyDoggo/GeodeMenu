#include "LabelHooks.hpp"

bool LabelsUILayer::init(GJBaseGameLayer* bgl)
{
    if (!UILayer::init(bgl))
        return false;

    if (!bgl)
        return true;

    m_fields->labelLayer = LabelLayer::create(this);

    this->addChild(m_fields->labelLayer, 69 + 1);
    return true;
}

void LabelsUILayer::regenerateLabels()
{
    if (m_fields->labelLayer)
        m_fields->labelLayer->removeFromParent();

    m_fields->labelLayer = LabelLayer::create(this);

    this->addChild(m_fields->labelLayer, 69 + 1);
}

void LabelBaseGameLayer::resetLevelVariables()
{
    GJBaseGameLayer::resetLevelVariables();

    if (LevelEditorLayer::get() ? !LevelEditorLayer::get()->m_editorUI->m_playtestStopBtn->isVisible() : true)
        as<LabelsUILayer*>(m_uiLayer)->m_fields->labelLayer->incrementAttempts();

    as<LabelsUILayer*>(m_uiLayer)->m_fields->labelLayer->resetCPS();
}

void LabelBaseGameLayer::handleButton(bool down, int button, bool isPlayer1)
{
    GJBaseGameLayer::handleButton(down, button, isPlayer1);

    if (down && button == 1)
        as<LabelsUILayer*>(m_uiLayer)->m_fields->labelLayer->increateCPS(!isPlayer1);

    if (auto ll = as<LabelsUILayer*>(m_uiLayer)->m_fields->labelLayer)
    {
        ll->triggerEvent(down ? LabelEventType::ClickStarted : LabelEventType::ClickEnded);

        if (isPlayer1)
            ll->triggerEvent(down ? LabelEventType::P1ClickStarted : LabelEventType::P1ClickEnded);
        else
            ll->triggerEvent(down ? LabelEventType::P2ClickStarted : LabelEventType::P2ClickEnded);
    }
}

void LabelPlayLayer::destroyPlayer(PlayerObject* p0, GameObject* p1)
{
    if (p1 != m_unk3688)
        as<LabelsUILayer*>(m_uiLayer)->m_fields->labelLayer->setLastPercentage(getCurrentPercent());

    PlayLayer::destroyPlayer(p0, p1);
}