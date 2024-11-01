#include "SimpleINI.hpp"

SimpleINI* SimpleINI::createWithString(std::string ini)
{
    auto pRet = new SimpleINI();

    if (pRet->init(ini))
    {
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

SimpleINI* SimpleINI::createWithFile(std::string file)
{
    return SimpleINI::createWithString(utils::file::readString(file).unwrapOr("NULL"));
}

bool SimpleINI::init(std::string data)
{
    std::string header = "default";

    auto lines = utils::string::split(data, "\n");

    std::unordered_map<std::string, std::string> kk;

    for (auto line : lines)
    {
        line = utils::string::trim(line);

        if (line.starts_with(";") || line.starts_with("#"))
            continue;

        if (line.starts_with("[") && line.ends_with("]"))
        {
            header = utils::string::trim(line.substr(1, line.size() - 2));

            continue;
        }

        if (line.find("=") == std::string::npos)
            continue;

        auto parts = utils::string::split(line, "=");

        std::string value;

        int i = 0;

        for (auto part : parts)
        {
            i++;
            if (i == 1)
                continue;

            value += part;

            if (i != parts.size())
                value += "=";
        }

        auto key = fmt::format("{}::{}", header, utils::string::trim(parts[0]));

        headerKeys.emplace(key, utils::string::trim(value));
    }

    return true;
}

bool SimpleINI::hasKey(std::string key)
{
    return headerKeys.contains(key);
}

std::string SimpleINI::getKeyValue(std::string key, std::string def)
{
    if (!headerKeys.contains(key))
        return def;

    return headerKeys[key];
}

float SimpleINI::getKeyValueFloat(std::string key, std::string def)
{
    auto f = numFromString<float>(hasKey(key) ? headerKeys[key] : def).unwrapOr(1.0f);

    return f;
}

int SimpleINI::getKeyValueInt(std::string key, std::string def)
{
    auto i = numFromString<int>(hasKey(key) ? headerKeys[key] : def).unwrapOr(1);

    return i;
}