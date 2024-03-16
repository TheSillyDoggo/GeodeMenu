#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (EditorUI)
{
    Module* mod = nullptr;

    TodoReturn constrainGameLayerPosition()
    {
        if (!m_fields->mod)
            m_fields->mod = Client::GetModule("free-scroll");

        if (!m_fields->mod->enabled)
            EditorUI::constrainGameLayerPosition();
    }
};