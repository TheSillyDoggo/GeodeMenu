/*
#include "PCGuiNode.hpp"

PCGuiNode* PCGuiNode::create()
{
    auto pRet = new PCGuiNode();

    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

PCGuiNode::~PCGuiNode()
{
    this->release();
}

PCGuiNode* PCGuiNode::get()
{
    if (!instance)
    {
        instance = PCGuiNode::create();
        instance->retain();
    }

    return instance;
}

void PCGuiNode::visit()
{
    CCNode::visit();
}

void PCGuiNode::draw()
{
    CCNode::draw();
    ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //if (!Client::instance)
    //    return;

    ccDrawSolidRect(ccp(0, 0), ccp(100, 100), ccc4f(drawScene ? 1 : 0, 0, 0, 1));

    for (auto window : Client::instance->windows)
    {
        //window->draw();
    }
}

class $modify (CCEGLView)
{
    virtual void swapBuffers()
    {
        PCGuiNode::get()->visit();

        CCEGLView::swapBuffers();
    }
};

class $modify (CCDirector)
{
    void drawScene()
    {
        CCDirector::drawScene();

        PCGuiNode::get()->visit();
    }
};*/