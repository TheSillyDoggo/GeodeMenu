#include "BlurLayer.hpp"

std::vector<CCBlurLayer*> layers;

CCBlurLayer* CCBlurLayer::create()
{
    auto pRet = new CCBlurLayer();

    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool CCBlurLayer::init()
{
    if (!CCLayerColor::init())
        return false;

    layers.push_back(this);

    render = CCRenderTexture::create(getContentWidth(), getContentHeight());
    render->retain();

    render2 = CCRenderTexture::create(getContentWidth(), getContentHeight());
    render2->getSprite()->setAnchorPoint(ccp(0, 1));
    render2->retain();

    program = createProgram(true);
    
    //program2 = createProgram(false);

    auto res = CCEGLView::get()->getFrameSize();

    program->setUniformLocationWith2f(program->getUniformLocationForName("u_resolution"), res.width, res.height);

    this->uniformFirst = program->getUniformLocationForName("_first");
    this->uniformFast = program->getUniformLocationForName("fast");
    this->uniformRadius = program->getUniformLocationForName("radius");

    render->getSprite()->setShaderProgram(program);
    //render2->getSprite()->setShaderProgram(program);
    // render->setShaderProgram(program2);

    this->addChild(render, 69);
    this->addChild(render2, 69);
    return true;
}

CCGLProgram* CCBlurLayer::createProgram(bool horizontal)
{
    CCGLProgram* program = new CCGLProgram();

    program->initWithVertexShaderFilename("box_blur.vsh"_spr, "box_blur.fsh"_spr);

    program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

    program->link();
    program->updateUniforms();
    program->retain();

    return program;
}

CCBlurLayer::~CCBlurLayer()
{
    program->release();
    render->release();

    layers.erase(std::remove(layers.begin(), layers.end(), this), layers.end());
}

void CCBlurLayer::setFirst(bool first)
{
    first = false;

    program->setUniformLocationWith1i(uniformFirst, first ? 1 : 0);
}

void CCBlurLayer::setRadius(float radius)
{
    program->setUniformLocationWith1f(uniformRadius, radius);
}

void CCBlurLayer::visit()
{
    if (this != layers[layers.size() - 1])
        return;

    if (auto scene = CCScene::get())
    {
        if (auto parent = this->getParent())
        {
            parent->setVisible(false);

            render->beginWithClear(0, 0, 0, 0);
            scene->visit();
            render->end();

            parent->setVisible(true);

            render->setPosition(CCDirector::get()->getWinSize() / 2);

            //draw

            render->visit();
            return;
        }
    }
}