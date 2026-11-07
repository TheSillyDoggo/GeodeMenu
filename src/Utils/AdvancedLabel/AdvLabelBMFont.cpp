#include "AdvLabelBMFont.hpp"
#include "../../Localisation/LocalisationManager.hpp"
#include <FastBMFontConfig.hpp>
#include <Utils.hpp>

AdvLabelBMFont* AdvLabelBMFont::createWithString(std::string label, std::string font)
{
    auto pRet = new AdvLabelBMFont();

    if (pRet && pRet->initWithString(label, font))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

AdvLabelBMFont* AdvLabelBMFont::createWithLocalisation(std::string localisationKey, std::string font)
{
    return createWithString(LocalisationManager::get()->getLocalisedString(localisationKey), font);
}

void AdvLabelBMFont::updateLabel()
{
    forceFallback = false;
    forceRegular = false;

    auto str2 = str;

    if (str2.starts_with("<bm>"))
    {
        str2 = str2.substr(4);
        forceRegular = true;
    }

    if (str2.starts_with("<ttf>"))
    {
        str2 = str2.substr(5);
        forceFallback = true;
    }

    auto useFallback = useTTFFont();
    auto lbl = useFallback ? fallbackLabel : regularLabel;
    auto lbl2 = !useFallback ? fallbackLabel : regularLabel;

    lbl->setVisible(true);
    lbl2->setVisible(false);

    lbl->setBreakWords(wrapByLetters);
    lbl->setMaxWidth(maxWidth / lbl->getScale());
    lbl->setLineSpacing(lineSpacing);
    lbl->setText(str2);
    lbl->setColor(this->colour);
    lbl->setOpacity(this->opacity);
    lbl->setAlignment(alignment);
    
    this->setContentSize(lbl->getScaledContentSize());
}

bool AdvLabelBMFont::useTTFFont()
{
    if (forceFallback)
        return true;

    if (forceRegular)
        return false;

    switch (ttfUsage)
    {
        case AdvLabelTTFUsage::None:
            return false;

        case AdvLabelTTFUsage::Auto: {
            auto charSet = fallbackLabel->getFont()->getCharDefs();

            for (auto ch : str)
            {
                if (ch == (char)10)
                    continue;

                if (!charSet.contains(ch))
                {
                    return true;
                }
            }

            return false;
        }

        case AdvLabelTTFUsage::Forced:
            return true;
    }

    return false;
}

bool AdvLabelBMFont::initWithString(std::string str, std::string font)
{
    if (!CCNode::init())
        return false;

    ttfUsage = LocalisationManager::get()->getDefaultTTFUsage();

    this->regularLabel = geode::Label::create(font);
    this->fallbackLabel = geode::Label::create(LocalisationManager::get()->getAltFont());

    if (!fallbackLabel)
    {
        log::error("Failed to create fallback for '{}'", LocalisationManager::get()->getAltFont());
        this->fallbackLabel = geode::Label::create(font);
    }

    regularLabel->setPosition(ccp(0, 0));
    regularLabel->setAnchorPoint(ccp(0, 0));
    regularLabel->setRichTextEnabled(true);
    regularLabel->setID("regular");

    fallbackLabel->setPosition(ccp(0, 0));
    fallbackLabel->setAnchorPoint(ccp(0, 0));
    fallbackLabel->setRichTextEnabled(true);
    fallbackLabel->setID("fallback");

    this->setAnchorPoint(ccp(0.5f, 0.5f));
    this->str = std::move(str);
    this->setFntFile(font.c_str());

    this->addChildren(regularLabel, fallbackLabel);
    updateLabel();
    return true;
}

cocos2d::CCGLProgram* AdvLabelBMFont::createGoldShader()
{
    static const char* vert = R"(
        attribute vec4 a_position;
        attribute vec2 a_texCoord;
        attribute vec4 a_color;

        #ifdef GL_ES
        varying lowp vec4 v_fragmentColor;
        varying mediump vec2 v_texCoord;
        varying mediump vec2 v_position;
        #else
        varying vec4 v_fragmentColor;
        varying vec2 v_texCoord;
        varying vec2 v_position;
        #endif

        void main()
        {
            gl_Position = CC_MVPMatrix * a_position;
            v_fragmentColor = a_color;
            v_texCoord = a_texCoord;
            v_position = a_position;
        }
    )";

    static const char* frag = R"(
        #ifdef GL_ES
        precision lowp float;
        #endif

        varying vec2 v_texCoord;
        varying vec2 v_position;
        varying vec4 v_fragmentColor;
        uniform sampler2D CC_Texture0;

        void main()
        {
            gl_FragColor = texture2D(CC_Texture0, v_texCoord) * v_fragmentColor * vec4(mix(vec3(254, 227, 71), vec3(253, 161, 12), v_position.y / 30) / 255.0, 1.0);
        }
    )";

    auto prg = new CCGLProgram();
    prg->initWithVertexShaderByteArray(vert, frag);
    prg->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    prg->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
    prg->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    
    prg->autorelease();
    prg->link();
    prg->updateUniforms();
    return prg;
}

void AdvLabelBMFont::limitLabelWidth(float width, float defaultScale, float minScale)
{
    this->setScale(std::clamp<float>(width / getContentWidth(), minScale, defaultScale));
}

void AdvLabelBMFont::setString(const char *newString)
{
    this->str = newString;

    updateLabel();
}

const char* AdvLabelBMFont::getString(void)
{
    return str.c_str();
}

void AdvLabelBMFont::setFntFile(const char* fntFile)
{
    this->font = fntFile;
    regularLabel->setFont(font);

    auto shC = CCShaderCache::sharedShaderCache();

    if (!shC->programForKey("gold-shader"_spr))
        shC->addProgram(createGoldShader(), "gold-shader"_spr);

    fallbackLabel->setShaderProgram(shC->programForKey(font == "goldFont.fnt" ? "gold-shader"_spr : kCCShader_PositionTextureColor));
    fallbackLabel->setScale(regularLabel->getFont()->getCommonHeight() / fallbackLabel->getFont()->getCommonHeight());

    updateLabel();
}

const char* AdvLabelBMFont::getFntFile()
{
    return font.c_str();
}

void AdvLabelBMFont::setTTFUsage(AdvLabelTTFUsage usage)
{
    this->ttfUsage = usage;
}

AdvLabelTTFUsage AdvLabelBMFont::getTTFUsage()
{
    return ttfUsage;
}

void AdvLabelBMFont::setMaxWidth(float width)
{
    this->maxWidth = width;
    updateLabel();
}

float AdvLabelBMFont::getMaxWidth() const
{
    return maxWidth;
}

void AdvLabelBMFont::setLineSpacing(float spacing)
{
    this->lineSpacing = spacing;
    updateLabel();
}

float AdvLabelBMFont::getLineSpacing() const
{
    return lineSpacing;
}

void AdvLabelBMFont::setWrapByLetters(bool wrap)
{
    this->wrapByLetters = wrap;
    updateLabel();
}

bool AdvLabelBMFont::getWrapByLetters() const
{
    return wrapByLetters;
}

void AdvLabelBMFont::setAlignment(CCTextAlignment alignment)
{
    switch (alignment)
    {
        case kCCTextAlignmentLeft:
            this->alignment = Label::Alignment::Left;
            break;

        case kCCTextAlignmentCenter:
            this->alignment = Label::Alignment::Center;
            break;

        case kCCTextAlignmentRight:
            this->alignment = Label::Alignment::Right;
            break;
    }

    updateLabel();
}