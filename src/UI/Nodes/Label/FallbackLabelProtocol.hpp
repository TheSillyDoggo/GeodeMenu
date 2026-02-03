#pragma once

#include <Geode/Geode.hpp>

class FallbackLabelProtocol
{
    protected:
        std::string fallbackFont = "";
        bool forceFallback = false;

        virtual bool shouldUseFallback(std::string_view string, gd::set<unsigned int>* charSet);

    public:
        virtual void setFallbackFnt(const char* fntFile);
        virtual const char* getFallbackFnt();

        virtual void setForceFallback(bool force);
        virtual bool getForceFallback();
};

class TTFFallbackLabelProtocol : public FallbackLabelProtocol
{
    protected:
        bool useTTFFallback = false;

    public:
        virtual void setTTFFallback(bool ttf);
        virtual bool getTTFFallback();
};