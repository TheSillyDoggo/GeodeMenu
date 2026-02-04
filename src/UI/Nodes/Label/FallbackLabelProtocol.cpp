#include "FallbackLabelProtocol.hpp"

bool FallbackLabelProtocol::shouldUseFallback(std::string_view string, gd::set<unsigned int>* charSet)
{
    if (forceFallback)
        return true;

    if (auto ttf = geode::cast::typeinfo_cast<TTFFallbackLabelProtocol*>(this))
    {
        if (ttf->getTTFFallback())
            return true;
        else
        {
            if (fallbackFont.empty())
                return false;
        }
    }
    else
    {
        if (fallbackFont.empty())
            return false;
    }

    if (!charSet)
        return false;

    for (auto& ch : string)
    {
        if (!charSet->contains((int)ch))
            return true;
    }

    return false;
}

void FallbackLabelProtocol::setFallbackFnt(const char* fntFile)
{
    this->fallbackFont = fntFile;
}

const char* FallbackLabelProtocol::getFallbackFnt()
{
    return fallbackFont.c_str();
}

void FallbackLabelProtocol::setForceFallback(bool force)
{
    this->forceFallback = force;
}

bool FallbackLabelProtocol::getForceFallback()
{
    return forceFallback;
}

// TTFFallbackLabelProtocol

void TTFFallbackLabelProtocol::setTTFFallback(bool ttf)
{
    this->useTTFFallback = ttf;
}

bool TTFFallbackLabelProtocol::getTTFFallback()
{
    return useTTFFallback;
}