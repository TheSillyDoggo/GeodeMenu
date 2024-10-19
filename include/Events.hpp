#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

using UIOpenEvent = geode::DispatchEvent<Popup<>*>;
using UIOpenFilter = geode::DispatchFilter<Popup<>*>;

using UITabChangedEvent = geode::DispatchEvent<Popup<>*, CCMenu*, std::string, std::string>;
using UITabChangedFilter = geode::DispatchFilter<Popup<>*, CCMenu*, std::string, std::string>;

using UITabInitEvent = geode::DispatchEvent<Popup<>*, CCMenu*, std::string, std::string>;
using UITabInitFilter = geode::DispatchFilter<Popup<>*, CCMenu*, std::string, std::string>;

using UICloseEvent = geode::DispatchEvent<Popup<>*>;
using UICloseFilter = geode::DispatchFilter<Popup<>*>;