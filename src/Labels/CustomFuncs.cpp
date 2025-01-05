#include <rift/config.hpp>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

rift::Value cowsay(std::span<rift::Value> args)
{
    if (args.size() != 1)
        return rift::Value::string("Error: Cowsay requires one (1) string parameter.");

    if (!args[0].isString())
        return rift::Value::string("Error: Cowsay requires one (1) string parameter.");

    auto str = args[0].getString();

    std::stringstream ss;
    ss << " ";

    for (size_t i = 0; i < str.size() + 2; i++)
    {
        ss << "-";
    }

    ss << "\n";
    ss << "< ";
    ss << str;
    ss << " >";
    
    ss << "\n ";

    for (size_t i = 0; i < str.size() + 2; i++)
    {
        ss << "-";
    }

    ss << "\n";
    ss << "        \\   ^__^\n";
    ss << "         \\  (oo)\\_______\n";
    ss << "            (__)\\       )\\/\\\n";
    ss << "                ||----w |\n";
    ss << "                ||     ||";

    return rift::Value::string(ss.str());
}

rift::Value getPathProgress(std::span<rift::Value> args)
{
    if (args.size() != 1)
        return rift::Value::string("Error: getPathProgress requires one (1) string parameter.");

    if (!args[0].isString())
        return rift::Value::string("Error: getPathProgress requires one (1) string parameter.");

    auto str = args[0].getString();

    str = utils::string::toLower(str);

    if (str == "fire")
        return rift::Value::integer(GameStatsManager::get()->getStat("30"));
    else if (str == "ice")
        return rift::Value::integer(GameStatsManager::get()->getStat("31"));
    else if (str == "poison")
        return rift::Value::integer(GameStatsManager::get()->getStat("32"));
    else if (str == "shadow")
        return rift::Value::integer(GameStatsManager::get()->getStat("33"));
    else if (str == "lava")
        return rift::Value::integer(GameStatsManager::get()->getStat("34"));
    else if (str == "earth")
        return rift::Value::integer(GameStatsManager::get()->getStat("35"));
    else if (str == "blood")
        return rift::Value::integer(GameStatsManager::get()->getStat("36"));
    else if (str == "metal")
        return rift::Value::integer(GameStatsManager::get()->getStat("37"));
    else if (str == "light")
        return rift::Value::integer(GameStatsManager::get()->getStat("38"));
    else if (str == "souls")
        return rift::Value::integer(GameStatsManager::get()->getStat("39"));

    return rift::Value::string("Unknown Path");
}

rift::Value isPathActive(std::span<rift::Value> args)
{
    return rift::Value::boolean(GameStatsManager::get()->m_activePath != 0);
}

rift::Value getActivePath(std::span<rift::Value> args)
{
    switch (GameStatsManager::get()->m_activePath)
    {
        case 30:
            return rift::Value::string("Fire");

        case 31:
            return rift::Value::string("Ice");

        case 32:
            return rift::Value::string("Poison");

        case 33:
            return rift::Value::string("Shadow"); // the hedgehog

        case 34:
            return rift::Value::string("Lava");

        case 35:
            return rift::Value::string("Earth");

        case 36:
            return rift::Value::string("Blood");

        case 37:
            return rift::Value::string("Metal");

        case 38:
            return rift::Value::string("Light");

        case 39:
            return rift::Value::string("Souls");

        default:
            return rift::Value::string("None");
    }
}

$on_mod(Loaded)
{
    rift::config::addRuntimeFunction("cowsay", cowsay);
    rift::config::addRuntimeFunction("getPathProgress", getPathProgress);
    rift::config::addRuntimeFunction("isPathActive", isPathActive);
    rift::config::addRuntimeFunction("getActivePath", getActivePath);
}