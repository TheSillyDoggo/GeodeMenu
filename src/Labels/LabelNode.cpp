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
    script = rift::compile(module->format);
    
    if (!script)
    {
        return this->setString("Error Compiling Script");
    }
    
    script->setVariable("fps", StatusNode::get()->fps);
    script->setVariable("name", "World");

    auto res = script->run();

    this->setString(res.c_str());
}

LabelNode::~LabelNode()
{
    module->labelNode = nullptr;
}