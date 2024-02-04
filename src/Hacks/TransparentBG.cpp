#include <Geode/Geode.hpp>
#include <Geode/modify/CCSprite.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (CCSprite)
{
    bool grad;

    virtual bool initWithFile(const char *pszFilename)
    {
        if (!CCSprite::initWithFile(pszFilename))
            return false;

        if (std::string(pszFilename).starts_with("GJ_gradientBG.png"))
        {
            m_fields->grad = true;
        }

        return true;
    }

    virtual void setColor(const ccColor3B& color3)
    {
        if (m_fields->grad && Client::GetModuleEnabled("trans-bg"))
            CCSprite::setColor({255, 255, 255});
        else
            CCSprite::setColor(color3);
    }
};