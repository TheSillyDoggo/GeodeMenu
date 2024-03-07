/*#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class $modify (GameObject)
{
    virtual void setVisible(bool p0)
    {
        if (PlayLayer::get())
        {
            if (m_objectType == GameObjectType::Decoration && m_objectID != 44) // thanks firee (in the hole) for the practice checkpoint id
                GameObject::setVisible(false);
            else
                GameObject::setVisible(true);
        }
        else
            GameObject::setVisible(p0);
    }

    virtual void setOpacity(unsigned char p0)
    {
        GameObject::setOpacity(255);
    }

    virtual void setChildColor(cocos2d::ccColor3B const& p0)
    {

    }
};*/

/*
class $modify(GameObject) {
    void setVisible(bool v) {
        if (!Hacks::isHackEnabled("Layout Mode")) return GameObject::setVisible(v);
        //m_hasGroupParent == 0
        std::vector<int> outerPortal = {};
        //i really dont want to have to check every single object id
        if (m_objectType == GameObjectType::Decoration && m_objectID != 44) { // 44 being practice mode checkpoint, because thats a "decoration"
            GameObject::setVisible(false);
        } else {
            GameObject::setVisible(v);
        }
    }
};*/
/*

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/EffectGameObject.hpp>

using namespace geode::prelude;

std::vector<char const*> objs = {

    // SHADERS
    "edit_eShaderBtn_001.png",
    "edit_eSh_ShockWaveBtn_001.png",
    "edit_eSh_ShockLineBtn_001.png",
    "edit_eSh_GlitchBtn_001.png",
    "edit_eSh_ChromaticBtn_001.png",
    "edit_eSh_ChromaticGlitchBtn_001.png",
    "edit_eSh_PixelateBtn_001.png",
    "edit_eSh_LensCircleBtn_001.png",
    "edit_eSh_RadialBlurBtn_001.png",
    "edit_eSh_MotionBlurBtn_001.png",
    "edit_eSh_BulgeBtn_001.png",
    "edit_eSh_PinchBtn_001.png",
    "edit_eSh_GrayscaleBtn_001.png",
    "edit_eSh_SepiaBtn_001.png",
    "edit_eSh_InvertColorBtn_001.png",
    "edit_eSh_HueBtn_001.png",
    "edit_eSh_EditColorBtn_001.png",
    "edit_eSh_SplitScreenBtn_001.png",

    // CAMERA
    "edit_eCamModeBtn_001.png",
    "edit_eGPOffsetBtn_001.png",
    "edit_eEdgeBtn_001.png",
    "edit_eCamGuideBtn_001.png",
    "edit_eCamRotBtn_001.png",
    "edit_eOffsetBtn_001.png",
    "edit_eStaticBtn_001.png",
    "edit_eZoomBtn_001.png",

    // idk what to call this
    "edit_eAlphaBtn_001.png",

    // AREA
    "edit_eTintCol01Btn_001.png",
};

class $modify(EffectGameObject) {
	bool init(char const* p0) {
		if (!EffectGameObject::init(p0)) 
            return false;

        if (std::find(objs.begin(), objs.end(), p0) != objs.end())
            return false;

		return true;
	}
};

class $modify(GameObject) {
    void setVisible(bool v) {
        //m_hasGroupParent == 0
        std::vector<int> outerPortal = {};
        //i really dont want to have to check every single object id
        if (m_objectType == GameObjectType::Decoration && m_objectID != 44) { // 44 being practice mode checkpoint, because thats a "decoration"
            GameObject::setVisible(false);
        } else {
            GameObject::setVisible(v);
        }
    }
};*/