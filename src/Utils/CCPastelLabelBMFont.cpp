#include "CCPastelLabelBMFont.hpp"
#include "../Client/ColourUtility.h"

CCPastelLabelBMFont::CCPastelLabelBMFont() {
}

CCPastelLabelBMFont::~CCPastelLabelBMFont() {
}

CCPastelLabelBMFont* CCPastelLabelBMFont::create(const char* text, const char* fontFile) {
    CCPastelLabelBMFont* label = new CCPastelLabelBMFont();
    if (label && label->initWithString(text, fontFile)) {
        label->autorelease();
        return label;
    }
    CC_SAFE_DELETE(label);
    return nullptr;
}

bool CCPastelLabelBMFont::initWithString(const char* text, const char* fontFile) {
    if (!CCLabelBMFont::initWithString(text, fontFile))
        return false;
    
    this->scheduleUpdate();
    return true;
}

void CCPastelLabelBMFont::update(float dt) {
    this->setColor(ColourUtility::getPastelColour());
}