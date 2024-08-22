#include "LabelNode.hpp"
#include "Labels.h"

LabelNode* LabelNode::create(LabelModule* module)
{
    auto pRet = new LabelNode();

    if (pRet && pRet->init(module))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool LabelNode::init(LabelModule* module)
{
    if (!CCLabelBMFont::initWithString("", module->getFont().c_str()))
        return false;

    this->module = module;
    module->labelNode = this;

    this->setScale(module->getScale() * 0.5f);
    this->setOpacity(module->getOpacity() * 255);

    return true;
}

void LabelNode::update(float delta)
{
    auto res = rift::compile(module->format);

    script = res.unwrapOr(nullptr);
    
    if (!script)
    {
        return this->setString(fmt::format("Error Compiling Script: {}", res.getMessage()).c_str());
    }
    
    script->setVariable("fps", rift::Value::integer(StatusNode::get()->fps));
    //script->setVariable("name", "World");

    auto res2 = script->run();

    this->setString(res2.c_str());
}

LabelNode::~LabelNode()
{
    module->labelNode = nullptr;
}