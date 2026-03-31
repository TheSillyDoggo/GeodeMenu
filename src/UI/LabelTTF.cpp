/*#include "LabelTTF.hpp"

using namespace geode::prelude;
using namespace tint;

LabelTTF* LabelTTF::create(const std::string& string, const std::string& font, cocos2d::CCTextAlignment alignment) {
    auto pRet = new LabelTTF();

    if (pRet && pRet->init(string, font, alignment)) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool LabelTTF::init(std::string string, std::string font, cocos2d::CCTextAlignment alignment) {
    if (!CCNode::init())
        return false;

    setFont(font, false);
    setString(string, false);

    updateLabel();
    return true;
}

void LabelTTF::setString(const std::string& str, bool shouldUpdateLabel) {
    this->m_string = str;

    if (shouldUpdateLabel)
        updateLabel();
}

void LabelTTF::setFont(const std::string& font, bool shouldUpdateLabel) {
    this->m_fontFile = font;
    this->m_tintFont = nullptr;

    static tint::FontLibrary fl;

    m_tintFont = tint::Font::loadFile(fl, CCFileUtils::get()->fullPathForFilename(this->m_fontFile.c_str(), true), FontConfig {
        .sizePx = 32.5f * 4 * (CCDirector::get()->getContentScaleFactor() / 4),
        .dpi = 72,
        .enableLigatures = true, 
        .enableKerning = true
    });

    if (shouldUpdateLabel)
        updateLabel();
}

void LabelTTF::setString(const char *label) {
    this->m_string = label;

    updateLabel();
}

const char* LabelTTF::getString(void) {
    return m_string.c_str();
}

CCTexture2D* LabelTTF::textureForAtlas(tint::AtlasGroup& atlas, uint32_t index) {
    if (m_atlasDirty || !m_atlasTextures.contains(index)) {
        delete m_atlasTextures[index];

        auto tex = new CCTexture2D();
        auto& page = atlas.page(index);
        tex->initWithData(page.pixels().data(), kCCTexture2DPixelFormat_RGBA8888, page.width(), page.height(), CCSizeMake(page.width(), page.height()));

        m_atlasTextures[index] = tex;
    }

    return m_atlasTextures[index];
}

CCSpriteBatchNode* LabelTTF::batchForIndex(uint32_t index) {
    if (!m_batchNodes.contains(index)) {
        m_batchNodes[index] = CCSpriteBatchNode::createWithTexture(nullptr);
    }

    return m_batchNodes[index];
}

void LabelTTF::updateLabel() {
    if (!m_tintFont)
        return;

    m_atlasDirty = true;
    tint::AtlasGroup atlasGroup(
        1024, 1024, // Atlas size
        PixelFormat::RGBA8, // Pixel format
        AtlasMode::Bitmap, 1.f // Atlas mode and SDF parameters
    );
    
    // Create shaper
    tint::Shaper shaper;
    
    // Shape the text
    auto shapedText = shaper.shapeMultiline(*m_tintFont, m_string, {
        .align = TextAlign::Center
    });
    auto quads = atlasGroup.buildQuads(*m_tintFont, shapedText);

    this->setContentSize(CC_POINT_PIXELS_TO_POINTS(ccp(
        shapedText.metrics.width,
        shapedText.metrics.height
    )));

    for (auto batch : m_batchNodes)
        batch.second->removeAllChildren();

    for (auto quad : quads)
    {
        auto batch = batchForIndex(quad.pageIndex);
        auto tex = textureForAtlas(atlasGroup, quad.pageIndex);

        batch->setTexture(tex);

        if (!batch->getParent())
            this->addChild(batch);

        auto spr = CCSprite::create();
        spr->setBatchNode(batch);

        spr->setTextureRect(CC_RECT_PIXELS_TO_POINTS(CCRectMake(
            quad.u0 * tex->getPixelsWide(),
            quad.v0 * tex->getPixelsHigh(),
            (quad.u1 - quad.u0) * tex->getPixelsWide(),
            (quad.v1 - quad.v0) * tex->getPixelsHigh()
        )));
        spr->setPosition(CC_POINT_PIXELS_TO_POINTS(ccp(
            quad.x0 + (quad.x1 - quad.x0) / 2,
            shapedText.metrics.height - (quad.y0 + (quad.y1 - quad.y0) / 2)
        )));
        spr->setAnchorPoint(ccp(0.5f, 0.5f));

        batch->addChild(spr);
    }
}

#include <Geode/modify/MenuLayer.hpp>

class $modify (MenuLayer)
{
    bool init()
    {
        MenuLayer::init();

        auto l = geode::LabelTTF::create("hello friend!\nもしもし友達!", "Arial.ttf");
        l->setZOrder(8008);
        l->setAnchorPoint(ccp(0.5f, 0.5f));
        l->setPosition(getContentSize() / 2);

        this->addChild(l);
        return true;
    }
};*/