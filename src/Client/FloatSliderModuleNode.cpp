#include "FloatSliderModuleNode.hpp"

FloatSliderModuleNode* FloatSliderModuleNode::create(FloatSliderModule* module)
{
    auto pRet = new FloatSliderModuleNode();

    if (pRet && pRet->init(module))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void FloatSliderModuleNode::setup()
{
    auto mod = static_cast<FloatSliderModule*>(module);

    auto label = CCLabelBMFont::create(mod->getName().c_str(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->limitLabelWidth(65, 0.575f, 0);

    slider = BetterSlider::create(this, menu_selector(FloatSliderModuleNode::onSliderMoved));
    slider->setAnchorPoint(ccp(1, 0.5f));
    slider->setRange(mod->minRange, mod->maxRange);
    slider->setValueRanged(mod->getValue());
    slider->setSnapValuesRanged(mod->snaps);
    slider->setContentWidth(getContentWidth() - label->getScaledContentWidth());
    slider->setScale(0.8f);
    slider->getThumb()->setScale(0.75f);

    this->addChildAtPosition(label, Anchor::Left, ccp(4, 0));
    this->addChildAtPosition(slider, Anchor::Right, ccp(-9, -1.5f));
}

void FloatSliderModuleNode::onSliderMoved(CCObject* sender)
{
    auto mod = static_cast<FloatSliderModule*>(module);

    mod->setValue(slider->getValueRanged());
}

void FloatSliderModuleNode::updateNode()
{
    auto mod = static_cast<FloatSliderModule*>(module);

    slider->setValueRanged(mod->getValue());
}