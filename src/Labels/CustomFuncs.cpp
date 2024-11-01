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

$on_mod(Loaded)
{
    rift::config::addRuntimeFunction("cowsay", cowsay);
}