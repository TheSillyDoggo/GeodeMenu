#include "CCBlurLayer.hpp"


struct Shader {
    GLuint vertex = 0;
    GLuint fragment = 0;
    GLuint program = 0;

    Result<std::string> compile(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
    Result<std::string> link();
    void cleanup();
};

struct RenderTexture {
    GLuint fbo = 0;
    GLuint tex = 0;
    GLuint rbo = 0;

    void setup(GLsizei width, GLsizei height);
    void cleanup();
};

namespace Blur
{
    RenderTexture ppRt0;
    RenderTexture ppRt1;
    GLuint ppVao = 0;
    GLuint ppVbo = 0;
    Shader ppShader;
    GLint ppShaderFast = 0;
    GLint ppShaderFirst = 0;
    GLint ppShaderRadius = 0;

    float blurTimer = 0.f;
};

using namespace Blur;

CCBlurLayer::~CCBlurLayer()
{

}

bool CCBlurLayer::init()
{
    if (!setup)
    {
        setup = true;

        doSetup();
    }

    if (!CCLayerColor::init())
        return false;

    this->addChild(CCSprite::create());

    return true;
}

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

void CCBlurLayer::visit()
{
    if (this->getOpacity())
    {
        float v = this->getOpacity() / 255.0f;

        blurStrength = v;
    }
    else
    {
        blurStrength = 0;
    }

    CCLayerColor::visit();
}

void CCBlurLayer::draw()
{
    #ifndef GEODE_IS_MACOS
    
    if (blurStrength == 0)
        return CCLayerColor::draw();

    GLint drawFbo = 0;
    GLint readFbo = 0;
    glGetIntegerv(0x8CA6, &drawFbo);
    glGetIntegerv(0x8CAA, &readFbo);

    glBindFramebuffer(0x8D40, ppRt0.fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);



    CCLayerColor::draw();

    if (getParent())
        getParent()->setVisible(false);
    
    CCNode* parent = CCScene::get();
    if (parent && !visiting) {
        visiting = true;

        kmGLPushMatrix();

        parent->transform();
        parent->visit();

        kmGLPopMatrix();

        visiting = false;
    }

    if (getParent())
        getParent()->setVisible(true);

    glBindVertexArray(ppVao);
    ccGLUseProgram(ppShader.program);
    glUniform1i(ppShaderFast, true);
    glUniform1f(ppShaderRadius, blurStrength);

    glBindFramebuffer(0x8D40, ppRt1.fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glUniform1i(Blur::ppShaderFirst, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, ppRt0.tex);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindFramebuffer(0x8CA9, drawFbo);
    glBindFramebuffer(0x8CA8, readFbo);

    glUniform1i(ppShaderFirst, GL_FALSE);
    glBindTexture(GL_TEXTURE_2D, ppRt1.tex);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);

    #endif
}

Result<std::string> Shader::compile(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) {
    auto vertexSource = file::readString(vertexPath);
    if (!vertexSource)
        return Err("failed to read vertex shader at path {}: {}", vertexPath.string(), vertexSource.unwrapErr());

    auto fragmentSource = file::readString(fragmentPath);
    if (!fragmentSource)
        return Err("failed to read fragment shader at path {}: {}", fragmentPath.string(), fragmentSource.unwrapErr());

    auto getShaderLog = [](GLuint id) -> std::string {
        GLint length, written;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        if (length <= 0)
            return "";
        auto stuff = new char[length + 1];
        glGetShaderInfoLog(id, length, &written, stuff);
        std::string result(stuff);
        delete[] stuff;
        return result;
    };
    GLint res;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    auto oglSucks = vertexSource.unwrap().c_str();
    glShaderSource(vertex, 1, &oglSucks, nullptr);
    glCompileShader(vertex);
    auto vertexLog = getShaderLog(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &res);
    if(!res) {
        glDeleteShader(vertex);
        vertex = 0;
        return Err("vertex shader compilation failed:\n{}", vertexLog);
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    oglSucks = fragmentSource.unwrap().c_str();
    glShaderSource(fragment, 1, &oglSucks, nullptr);
    glCompileShader(fragment);
    auto fragmentLog = getShaderLog(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &res);
    if(!res) {
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        vertex = 0;
        fragment = 0;
        return Err("fragment shader compilation failed:\n{}", fragmentLog);
    }

    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    return Ok(fmt::format(
        "shader compilation successful. logs:\nvert:\n{}\nfrag:\n{}",
        vertexLog, fragmentLog
    ));
}

Result<std::string> Shader::link() {
    if (!vertex)
        return Err("vertex shader not compiled");
    if (!fragment)
        return Err("fragment shader not compiled");

    auto getProgramLog = [](GLuint id) -> std::string {
        GLint length, written;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
        if (length <= 0)
            return "";
        auto stuff = new char[length + 1];
        glGetProgramInfoLog(id, length, &written, stuff);
        std::string result(stuff);
        delete[] stuff;
        return result;
    };
    GLint res;

    glLinkProgram(program);
    auto programLog = getProgramLog(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    vertex = 0;
    fragment = 0;

    glGetProgramiv(program, GL_LINK_STATUS, &res);
    if(!res) {
        glDeleteProgram(program);
        program = 0;
        return Err("shader link failed:\n{}", programLog);
    }

    return Ok(fmt::format("shader link successful. log:\n{}", programLog));
}

void Shader::cleanup() {
    if (program)
        glDeleteProgram(program);
    program = 0;
}

void RenderTexture::setup(GLsizei width, GLsizei height) {
    GLint drawFbo = 0;
    GLint readFbo = 0;
    glGetIntegerv(0x8CA6, &drawFbo);
    glGetIntegerv(0x8CAA, &readFbo);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(0x8D40, fbo);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(0x8D40, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(0x8D40, 0x821A, GL_RENDERBUFFER, rbo);

    if(glCheckFramebufferStatus(0x8D40) != GL_FRAMEBUFFER_COMPLETE)
        log::error("pp fbo not complete, uh oh! i guess i will have to cut off ur pp now");
    glBindFramebuffer(0x8CA9, drawFbo);
    glBindFramebuffer(0x8CA8, readFbo);
}

void RenderTexture::cleanup() {
    if (fbo)
        glDeleteFramebuffers(1, &fbo);
    if (tex)
        glDeleteTextures(1, &tex);
    if (rbo)
        glDeleteRenderbuffers(1, &rbo);
    fbo = 0;
    tex = 0;
    rbo = 0;
}

void setupPostProcess() {
    auto size = CCDirector::get()->getOpenGLView()->getFrameSize();

    ppRt0.setup((GLsizei)size.width, (GLsizei)size.height);
    ppRt1.setup((GLsizei)size.width, (GLsizei)size.height);

    GLfloat ppVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };
    glGenVertexArrays(1, &ppVao);
    glGenBuffers(1, &ppVbo);
    glBindVertexArray(ppVao);
    glBindBuffer(GL_ARRAY_BUFFER, ppVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ppVertices), &ppVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    #ifdef GEODE_IS_ANDROID
    auto vertexPath = (std::string)CCFileUtils::get()->fullPathForFilename("pp-vert-android.glsl"_spr, false);
    #else
    auto vertexPath = (std::string)CCFileUtils::get()->fullPathForFilename("pp-vert.glsl"_spr, false);
    #endif
    #ifdef GEODE_IS_ANDROID
    auto fragmentPath = (std::string)CCFileUtils::get()->fullPathForFilename("pp-frag-android.glsl"_spr, false);
    #else
    auto fragmentPath = (std::string)CCFileUtils::get()->fullPathForFilename("pp-frag.glsl"_spr, false);
    #endif

    auto res = ppShader.compile(vertexPath, fragmentPath);
    if (!res) {
        log::error("Failed to compile shader: {}", res.unwrapErr());
        return;
    }
    log::info("{}", res.unwrap());

    glBindAttribLocation(ppShader.program, 0, "aPosition");
    glBindAttribLocation(ppShader.program, 1, "aTexCoords");

    res = ppShader.link();
    if (!res) {
        log::error("Failed to link shader: {}", res.unwrapErr());
        return;
    }
    log::info("{}", res.unwrap());

    ccGLUseProgram(ppShader.program);
    glUniform1i(glGetUniformLocation(ppShader.program, "screen"), 0);
    glUniform2f(glGetUniformLocation(ppShader.program, "screenSize"), size.width, size.height);
    ppShaderFast = glGetUniformLocation(ppShader.program, "fast");
    ppShaderFirst = glGetUniformLocation(ppShader.program, "first");
    ppShaderRadius = glGetUniformLocation(ppShader.program, "radius");
}

void cleanupPostProcess() {
    ppRt0.cleanup();
    ppRt1.cleanup();

    if (ppVao)
        glDeleteVertexArrays(1, &ppVao);
    if (ppVbo)
        glDeleteBuffers(1, &ppVbo);
    ppVao = 0;
    ppVbo = 0;

    ppShader.cleanup();
    ppShaderFast = 0;
    ppShaderFirst = 0;
    ppShaderRadius = 0;
}

#ifdef GEODE_IS_WINDOWS

#include <Geode/modify/CCEGLViewProtocol.hpp>

class $modify(CCEGLViewProtocol) {
    void setFrameSize(float width, float height) {
        CCEGLViewProtocol::setFrameSize(width, height);
        if (!CCDirector::get()->getOpenGLView())
            return;
        
        CCBlurLayer::setup = false;
    }
};

#endif

class $modify(GameManager) {
    void reloadAllStep5() {
        GameManager::reloadAllStep5();
        CCBlurLayer::setup = false;
    }
};

$on_mod(Unloaded) {
    cleanupPostProcess();
}

void CCBlurLayer::doSetup()
{
    cleanupPostProcess();
    setupPostProcess();
}