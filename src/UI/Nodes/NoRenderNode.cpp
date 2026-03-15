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

    #if GEODE_COMP_GD_VERSION == 22081
    geode::SceneManager::get()->keepAcrossScenes(render);
    #else
    geode::OverlayManager::get()->addChild(render);
    #endif
};