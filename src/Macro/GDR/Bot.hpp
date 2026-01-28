#pragma once

#include <gdr/gdr.hpp>

struct QOLBot : gdr::Replay<MyBot, gdr::Input<>>
{
    QOLBot() : Replay("QOLBot", 1)
    {

    }
};