#include "LabelNode.hpp"

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
    if (!CCLabelBMFont::initWithString("", "bigFont.fnt"))
        return false;

    this->module = module;

    return true;
}

void LabelNode::update(float delta)
{
    script = rift::compile(module->format);
    
    if (!script)
    {
        return this->setString("Error Compiling Script");
    }
    
    script->setVariable("number", 210);
    script->setVariable("name", "World");

    auto res = script->run();

    this->setString(res.c_str());
}