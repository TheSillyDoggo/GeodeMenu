#include "../../Client/Module.hpp"
#include <Geode/modify/CCTextInputNode.hpp>

using namespace geode::prelude;

class CharacterFilterBypass : public Module
{
    public:
        MODULE_SETUP(CharacterFilterBypass)
        {
            setName("Character Filter");
            setID("char-filter");
            setCategory("Universal");
            setDescription("Allows you to type any character in any input field");
        }
};

class CharacterLimitBypass : public Module
{
    public:
        MODULE_SETUP(CharacterLimitBypass)
        {
            setName("Character Limit");
            setID("char-limit");
            setCategory("Universal");
            setDescription("Allows you to type as many letters as you want in any input field");
        }
};

SUBMIT_HACK(CharacterFilterBypass)
SUBMIT_HACK(CharacterLimitBypass)

class $modify (CCTextInputNode)
{
    virtual bool onTextFieldInsertText(cocos2d::CCTextFieldTTF* pSender, char const* text, int nLen, cocos2d::enumKeyCodes keyCodes)
    {
        auto filter = m_allowedChars;
        auto limit = m_maxLabelLength;

        bool forceChar = this->getUserObject("force-char-bypass"_spr);
        bool forceFilter = this->getUserObject("force-filter-bypass"_spr);

        if (!this->getUserObject("disable-char-bypass"_spr))
        {
            if (CharacterFilterBypass::get()->getRealEnabled() || forceChar)
                m_allowedChars = text;

            if (CharacterLimitBypass::get()->getRealEnabled() || forceFilter)
                m_maxLabelLength = getString().size() + nLen + 69;
        }

        auto pRet = CCTextInputNode::onTextFieldInsertText(pSender, text, nLen, keyCodes);

        m_allowedChars = filter;
        m_maxLabelLength = limit;

        return pRet;
    }
};