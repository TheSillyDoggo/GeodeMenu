#include "LayoutModule.hpp"
#include "ButtonModuleNode.hpp"
#include <EditLayoutUI.hpp>

using namespace geode::prelude;

void LayoutModule::save()
{
    Mod::get()->setSavedValue<int>(fmt::format("{}_anchor", getID()), (int)anchor);
    Mod::get()->setSavedValue<float>(fmt::format("{}_offset.x", getID()), offset.x);
    Mod::get()->setSavedValue<float>(fmt::format("{}_offset.y", getID()), offset.y);
}

void LayoutModule::load()
{
    anchor = (Anchor)Mod::get()->getSavedValue<int>(fmt::format("{}_anchor", getID()), (int)defaultAnchor);
    offset.x = Mod::get()->getSavedValue<float>(fmt::format("{}_offset.x", getID()), defaultOffset.x);
    offset.y = Mod::get()->getSavedValue<float>(fmt::format("{}_offset.y", getID()), defaultOffset.y);
}

void LayoutModule::setPreviewNode(std::function<cocos2d::CCNode*()> preview)
{
    this->previewNode = std::move(preview);
}

void LayoutModule::setDefaults(geode::Anchor an, cocos2d::CCPoint off)
{
    this->defaultAnchor = an;
    this->defaultOffset = off;
}

cocos2d::CCNode* LayoutModule::createPreviewNode()
{
    return previewNode();
}

ModuleNode* LayoutModule::getNode()
{
    return ButtonModuleNode::create(this);
}

void LayoutModule::onClick()
{
    qolmod::EditLayoutUI::create(this)->show();
}

void LayoutModule::setPosition(CCPoint point)
{
    auto anc = ccp(0.5f, 0.5f);
    Anchor anch = Anchor::BottomLeft;
    auto win = CCDirector::get()->getWinSize();

    if (point.x < win.width * 0.3f)
        anc.x = 0;

    if (point.x > win.width * (1 - 0.3f))
        anc.x = 1;

    if (point.y < win.height * 0.3f)
        anc.y = 0;

    if (point.y > win.height * (1 - 0.3f))
        anc.y = 1;

    if (anc == ccp(0, 0))       anch = Anchor::BottomLeft;
    else if (anc == ccp(0.5f, 0))  anch = Anchor::Bottom;
    else if (anc == ccp(1, 0))  anch = Anchor::BottomRight;
    else if (anc == ccp(0, 0.5f))  anch = Anchor::Left;
    else if (anc == ccp(0.5f, 0.5f))  anch = Anchor::Center;
    else if (anc == ccp(1, 0.5f))  anch = Anchor::Right;
    else if (anc == ccp(0, 1))  anch = Anchor::TopLeft;
    else if (anc == ccp(0.5f, 1))  anch = Anchor::Top;
    else if (anc == ccp(1, 1))  anch = Anchor::TopRight;

    anchor = anch;
    offset = point - (win * anc);

    save();
}

CCPoint LayoutModule::getPosition()
{
    CCPoint off = CCPointZero;
    switch (anchor)
    {
        case Anchor::BottomLeft: break;
        case Anchor::Bottom: off = CCDirector::get()->getWinSize() * ccp(0.5f, 0); break;
        case Anchor::BottomRight: off = CCDirector::get()->getWinSize() * ccp(1, 0); break;
        case Anchor::Left: off = CCDirector::get()->getWinSize() * ccp(0, 0.5f); break;
        case Anchor::Center: off = CCDirector::get()->getWinSize() * ccp(0.5f, 0.5f); break;
        case Anchor::Right: off = CCDirector::get()->getWinSize() * ccp(1, 0.5f); break;
        case Anchor::TopLeft: off = CCDirector::get()->getWinSize() * ccp(0, 1); break;
        case Anchor::Top: off = CCDirector::get()->getWinSize() * ccp(0.5f, 1); break;
        case Anchor::TopRight: off = CCDirector::get()->getWinSize() * ccp(1, 1); break;
    }

    return off + offset;
}

void LayoutModule::applyToNode(CCNode* node)
{
    auto sel = schedule_selector(LayoutModule::nodeUpdate);

    node->setUserData(this);
    node->schedule(sel);

    if (node && sel)
    {
        (node->*sel)(1.0f);
    }
}

void LayoutModule::nodeUpdate(float dt)
{
    auto self = reinterpret_cast<CCNode*>(this);
    auto mod = reinterpret_cast<LayoutModule*>(self->getUserData());

    self->setPosition(mod->getPosition());
}