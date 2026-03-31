/*#pragma once

#include <Geode/Geode.hpp>
#include <Tint/Tint.hpp>

namespace geode
{
    class LabelTTF : public cocos2d::CCNode, cocos2d::CCLabelProtocol
    {
        protected:
            std::string m_string;
            std::string m_fontFile;
            cocos2d::CCTextAlignment m_alignment;
            bool m_atlasDirty = true;
            std::map<uint32_t, cocos2d::CCTexture2D*> m_atlasTextures = {};
            std::map<uint32_t, cocos2d::CCSpriteBatchNode*> m_batchNodes = {};
            std::unique_ptr<tint::Font> m_tintFont = nullptr;

            bool init(std::string string, std::string font, cocos2d::CCTextAlignment alignment);
            cocos2d::CCTexture2D* textureForAtlas(tint::AtlasGroup& atlas, uint32_t index);
            cocos2d::CCSpriteBatchNode* batchForIndex(uint32_t index);

        public:
            static LabelTTF* create(const std::string& string = "", const std::string& font = "", cocos2d::CCTextAlignment alignment = cocos2d::kCCTextAlignmentLeft);

            void setString(const std::string& str, bool shouldUpdateLabel = true);
            void setFont(const std::string& font, bool shouldUpdateLabel = true);

            virtual void setString(const char *label);
            virtual const char* getString(void);

            void updateLabel();
    };
};*/