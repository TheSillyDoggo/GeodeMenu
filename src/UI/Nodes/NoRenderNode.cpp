#include "NoRenderNode.hpp"
#include <AndroidBall.hpp>
#include <FloatingButton/FloatingUIManager.hpp>
#include "../../Hacks/Universal/Paint/PaintNode.hpp"

void qolmod::NoRenderNode::visit()
{

}

$on_game(Loaded)
{
    auto render = qolmod::NoRenderNode::create();

    render->addChild(AndroidBall::get());
    render->addChild(FloatingUIManager::get());
    render->addChild(qolmod::PaintNode::get());

    geode::OverlayManager::get()->addChild(render);
};