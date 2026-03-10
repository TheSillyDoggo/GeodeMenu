#pragma once

#include "../../Client/Module.hpp"

using namespace geode::prelude;

class ZoomPauseMenu : public Module
{
    public:
        MODULE_SETUP(ZoomPauseMenu)
        {
            setID("zoom-pause-menu");
            setCategory("Universal");
            setDefaultEnabled(true);
        }
};

class ZoomWithScroll : public Module
{
    public:
        MODULE_SETUP(ZoomWithScroll)
        {
            setID("zoom-pause-menu/zoom-with-scroll");
            setPriority(1);
            setDefaultEnabled(true);
        }
};

class ZoomWithTouch : public Module
{
    public:
        MODULE_SETUP(ZoomWithTouch)
        {
            setID("zoom-pause-menu/zoom-with-touch");
            setPriority(2);
            setDefaultEnabled(true);
        }
};

class ZoomPanning : public Module
{
    public:
        MODULE_SETUP(ZoomPanning)
        {
            setID("zoom-pause-menu/allow-panning");
            setPriority(3);
            setDefaultEnabled(true);
        }
};

class ShowZoomControls : public Module
{
    public:
        MODULE_SETUP(ShowZoomControls)
        {
            setID("zoom-pause-menu/show-controls");
            setPriority(4);
        }
};

class NoZoomLimit : public Module
{
    public:
        MODULE_SETUP(NoZoomLimit)
        {
            setID("zoom-pause-menu/no-zoom-limit");
            setPriority(5);
        }
};

SUBMIT_HACK(ZoomPauseMenu)
SUBMIT_OPTION(ZoomPauseMenu, ZoomWithScroll)
SUBMIT_OPTION(ZoomPauseMenu, ZoomWithTouch)
SUBMIT_OPTION(ZoomPauseMenu, ZoomPanning)
SUBMIT_OPTION(ZoomPauseMenu, ShowZoomControls)
SUBMIT_OPTION(ZoomPauseMenu, NoZoomLimit)