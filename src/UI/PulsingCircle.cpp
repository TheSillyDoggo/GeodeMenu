#include "PulsingCircle.hpp"

using namespace geode::prelude;
using namespace qolmod;

PulsingCircle* PulsingCircle::create(float startRadius, float endRadius, float duration, bool fadeIn, bool easeOut)
{
    auto pRet = new PulsingCircle();
    pRet->autorelease();
    pRet->startRadius = startRadius;
    pRet->endRadius = endRadius;
    pRet->duration = duration;
    pRet->fadeIn = fadeIn;
    pRet->easeOut = easeOut;
    return pRet;
}

void PulsingCircle::addCircle()
{
    auto circle = CCCircleWave::create(startRadius, endRadius, duration, fadeIn, easeOut);
    circle->m_delegate = this;
    circle->m_color = color;
    circle->m_circleMode = circleMode;
    circle->m_lineWidth = lineWidth;
    circle->m_opacityMod = opacityMod;
    circle->m_blendAdditive = blendAdditive;

    this->addChild(circle);
}

void PulsingCircle::circleWaveWillBeRemoved(CCCircleWave* circleWave)
{
    addCircle();
}

void PulsingCircle::onEnter()
{
    CCNode::onEnter();
    addCircle();
}