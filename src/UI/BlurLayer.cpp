#include "BlurLayer.hpp"
#include <Geode/modify/MenuLayer.hpp>

bool BlurLayer::init()
{
    if (!CCLayerColor::init())
        return false;

    rtex = CCRenderTexture::create(569, 320);
    node = CCNode::create();
    this->addChild(node);

    std::string vert = R"(
        attribute vec4 a_position;
        attribute vec2 a_texCoord;
        varying vec2 v_texCoord;

        void main() {
            gl_Position = CC_PMatrix * CC_MVMatrix * a_position;
            v_texCoord = a_texCoord;
        }
    )";
    
    // https://www.shadertoy.com/view/ltScRG
    std::string frag = R"(
        #ifdef GL_ES
            precision mediump float;
        #endif

        varying vec2 v_texCoord;

        uniform sampler2D CC_Texture0;
        uniform vec2 resolution;
        uniform vec2 texScale;
        const int samples = 35;
        const float sigma = float(samples) * 0.25;

        // Gaussian function
        float gaussian(vec2 i) {
            return exp(-0.5 * dot(i /= sigma, i)) / (6.2831 * sigma * sigma);
        }

        vec4 blur(sampler2D sp, vec2 U, vec2 scale) {
            vec4 O = vec4(0.0);
            int s = samples / 2;  // Assuming LOD = 2, tile size is 4x4

            for (int i = 0; i < s * s; i++) {
                vec2 d = vec2(i % s, i / s) * 2.0 - float(samples) / 2.0;
                O += gaussian(d) * texture2D(sp, U + scale * d);
            }

            return O / O.a;
        }

        void main() {
            vec2 uv = v_texCoord * resolution;
            gl_FragColor = blur(CC_Texture0, uv / resolution, texScale);
        }
    )";


    program = new CCGLProgram();
    program->initWithVertexShaderByteArray(vert.c_str(), frag.c_str());
    program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
    program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    
    program->retain();
    program->link();
    program->updateUniforms();

    GLint resolutionLoc = program->getUniformLocationForName("resolution");
    GLint texScaleLoc = program->getUniformLocationForName("texScale");

    float screenWidth = CCDirector::get()->getWinSize().width;
    float screenHeight = CCDirector::get()->getWinSize().height;

    program->use();
    program->setUniformLocationWith2f(resolutionLoc, screenWidth, screenHeight);
    program->setUniformLocationWith2f(texScaleLoc, 1.0f / screenWidth, 1.0f / screenHeight);

    //sizeLocation = program->getUniformLocationForName("blurSize");

    rtex->getSprite()->setShaderProgram(program);

    instances.push_back(this);

    this->setDrawOnce(true);

    return true;
}

BlurLayer::~BlurLayer()
{
    std::erase(instances, this);
}

void BlurLayer::visit()
{
    CCLayerColor::visit();
}

bool __blurlayer__drawing__ = false;
BlurLayer* __blurlayer__being__drawn__ = nullptr;

void BlurLayer::draw()
{
    if (this != instances[instances.size() - 1])
        return;

    if (!CCScene::get() || !rtex || !rtex->getSprite())
        return;

    if (__blurlayer__drawing__ && __blurlayer__being__drawn__ != this)
        return;

    if (drawOnce ? !hasDrawn : true)
    {
        if (__blurlayer__drawing__ && __blurlayer__being__drawn__ == this)
        {
            rtex->end();

            __blurlayer__drawing__ = false;
            __blurlayer__being__drawn__ = nullptr;

            return;
        }

        __blurlayer__drawing__ = true;
        __blurlayer__being__drawn__ = this;

        rtex->beginWithClear(0, 0, 0, 0);

        auto scene = CCScene::get();
        scene->visit();

        __blurlayer__drawing__ = false;
        __blurlayer__being__drawn__ = nullptr;

        hasDrawn = true;
    }

    node->setPosition(-ccp(boundingBox().getMinX(), boundingBox().getMinY()));

    GameToolbox::preVisitWithClippingRect(node, boundingBox());

    rtex->getSprite()->setPosition(CCDirector::get()->getWinSize() / 2 - ccp(boundingBox().getMinX(), boundingBox().getMinY()));
    rtex->visit();

    glDisable(0xc11);
}



void BlurLayer::setBlurSize(float size)
{
    blurSize = size;

    //program->setUniformLocationWith1f(sizeLocation, size);
}

float BlurLayer::getBlurSize()
{
    return blurSize;
}

void BlurLayer::setBlurPasses(int passes)
{
    blurPasses = passes;
}

int BlurLayer::getBlurPasses()
{
    return blurPasses;
}

void BlurLayer::setDrawOnce(bool draw)
{
    drawOnce = draw;
}

bool BlurLayer::getDrawOnce()
{
    return drawOnce;
}