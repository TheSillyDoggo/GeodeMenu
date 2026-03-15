#include "../../Client/Module.hpp"
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class NoEditorZoomLimit : public Module
{
    public:
        MODULE_SETUP(NoEditorZoomLimit)
        {
            setID("no-editor-zoom-limit");
            setCategory("Creator");
        }
};

SUBMIT_HACK(NoEditorZoomLimit);

class $modify (NoZoomLimitEditorUI, EditorUI)
{
    void zoomNoLimit(bool zoomingIn)
    {
        float scale = m_editorLayer->m_groundLayer->getScale();

        if (zoomingIn)
            scale += 0.1f;
        else
            scale -= 0.1f;
            
        scale = std::max<float>(scale, 0.1f);

        updateZoom(scale);
    }

    #if defined(GEODE_IS_WINDOWS) || defined(GEODE_IS_IOS)
    void zoomIn(cocos2d::CCObject* sender)
    {
        if (NoEditorZoomLimit::get()->getRealEnabled())
            return zoomNoLimit(true);

        EditorUI::zoomIn(sender);
    }

    void zoomOut(cocos2d::CCObject* sender)
    {
        if (NoEditorZoomLimit::get()->getRealEnabled())
            return zoomNoLimit(false);

        EditorUI::zoomOut(sender);
    }

    virtual void scrollWheel(float y, float x)
    {
        auto scale = m_editorLayer->m_groundLayer->getScale();

        EditorUI::scrollWheel(y, x);

        if (NoEditorZoomLimit::get()->getRealEnabled())
        {
            if (m_editorLayer->m_playbackMode != PlaybackMode::Playing && CCKeyboardDispatcher::get()->getControlKeyPressed())
            {
                m_editorLayer->m_groundLayer->setScale(scale);

                if (y < 0 || x < 0)
                    zoomNoLimit(true);
                else
                    zoomNoLimit(false);
            }
        }
    }
    #else
    void zoomGameLayer(bool zoomingIn)
    {
        if (NoEditorZoomLimit::get()->getRealEnabled())
            return zoomNoLimit(zoomingIn);

        EditorUI::zoomGameLayer(zoomingIn);
    }
    #endif
};