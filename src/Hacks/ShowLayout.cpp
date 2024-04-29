#ifndef GEODE_IS_MACOS

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/GJGroundLayer.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/EffectGameObject.hpp>
#include <Geode/modify/CCNode.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

Module* showLayout = nullptr;
Module* showLayoutCamera = nullptr;

std::vector<std::string> objs = {

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

    // Alpha Trigger
    "edit_eAlphaBtn_001.png",

    // Colour Trigger
    "edit_eTintCol01Btn_001.png",

    // Gradient
    "edit_eGradientBtn_001.png",

    // Pulse
    "edit_ePulseBtn_001.png",
};

std::vector<std::string> camera = {
    "edit_eCamModeBtn_001.png",
    "edit_eGPOffsetBtn_001.png",
    "edit_eEdgeBtn_001.png",
    "edit_eCamGuideBtn_001.png",
    "edit_eCamRotBtn_001.png",
    "edit_eOffsetBtn_001.png",
    "edit_eStaticBtn_001.png",
    "edit_eZoomBtn_001.png",
};

class $modify (PlayLayer)
{
    CCSprite* background;
    Ref<GJGroundLayer> ground1;
    Ref<GJGroundLayer> ground2;
    Ref<CCNode> customNode;

    static void onModify(auto& self) {
        std::vector<geode::Hook*> hooks;

        hooks.push_back(self.getHook("PlayLayer::postUpdate").unwrap());

        Loader::get()->queueInMainThread([hooks] 
        {
            auto modu = Client::GetModule("show-layout");

            for (auto hook : hooks)
            {
                hook->setAutoEnable(false);

                if (!modu->enabled)
                    hook->disable();

                modu->hooks.push_back(hook);
            }
        });
    }

    void addObject(GameObject* p0)
    {
        if (!showLayout)
        {
            showLayout = Client::GetModule("show-layout");
            showLayoutCamera = showLayout->options[0];
        }

        if (!m_fields->customNode)
        {
            m_fields->customNode = CCNode::create();
            m_fields->customNode->setID("IMPORTANT"_spr);

            this->m_objectLayer->addChild(m_fields->customNode);
        }
            

        if (showLayout->enabled)
        {
            if (p0->m_isHide && p0->m_objectType != GameObjectType::Modifier && p0->m_objectType != GameObjectType::Special)
                m_fields->customNode->addChild(p0);
        }

        PlayLayer::addObject(p0);
    }

    void onQuit()
    {
        if (m_fields->customNode)
            m_fields->customNode->setID("");

        PlayLayer::onQuit();
    }

    virtual void postUpdate(float dt)
    {
        PlayLayer::postUpdate(dt);

        if (!showLayout)
        {
            showLayout = Client::GetModule("show-layout");
            showLayoutCamera = showLayout->options[0];
        }

        if (!showLayout->enabled)
            return;

        if (!m_fields->background)
            m_fields->background = getChildOfType<CCSprite>(getChildOfType<CCNode>(this, 0), 0);
        if (m_fields->background)
        {
            m_fields->background->setColor(as<ColourModule*>(showLayout->options[1])->colour);
        }
    }
};

class $modify (GJBaseGameLayer)
{
    static void onModify(auto& self) {
        std::vector<geode::Hook*> hooks;

        hooks.push_back(self.getHook("GJBaseGameLayer::createMiddleground").unwrap());
        hooks.push_back(self.getHook("GJBaseGameLayer::createBackground").unwrap());
        hooks.push_back(self.getHook("GJBaseGameLayer::createGroundLayer").unwrap());
        hooks.push_back(self.getHook("GJBaseGameLayer::updateColor").unwrap());

        Loader::get()->queueInMainThread([hooks] 
        {
            auto modu = Client::GetModule("show-layout");

            for (auto hook : hooks)
            {
                hook->setAutoEnable(false);

                if (!modu->enabled)
                    hook->disable();

                modu->hooks.push_back(hook);
            }
        });
    }

    void createMiddleground(int p0)
    {
        if (!showLayout)
        {
            showLayout = Client::GetModule("show-layout");
            showLayoutCamera = showLayout->options[0];
        }

        if (!PlayLayer::get() || !showLayout->enabled)
            GJBaseGameLayer::createMiddleground(p0);
    }

    void createBackground(int p0)
    {
        if (!showLayout)
        {
            showLayout = Client::GetModule("show-layout");
            showLayoutCamera = showLayout->options[0];
        }

        GJBaseGameLayer::createBackground((PlayLayer::get() && showLayout->enabled) ? 0 : p0);
    }

    void createGroundLayer(int p0, int p1)
    {
        if (!showLayout)
        {
            showLayout = Client::GetModule("show-layout");
            showLayoutCamera = showLayout->options[0];
        }

        GJBaseGameLayer::createGroundLayer(PlayLayer::get() && showLayout->enabled ? 0 : p0, PlayLayer::get() && showLayout->enabled ? 0 : p1);
    }

    virtual void updateColor(cocos2d::ccColor3B& p0, float p1, int p2, bool p3, float p4, cocos2d::ccHSVValue& p5, int p6, bool p7, EffectGameObject* p8, int p9, int p10)
    {
        if (!showLayout)
        {
            showLayout = Client::GetModule("show-layout");
            showLayoutCamera = showLayout->options[0];
        }

        if (!PlayLayer::get() || !showLayout->enabled)
            GJBaseGameLayer::updateColor(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);

        if (p2 == 1010 || p6 == 1010 || p9 == 1010 || p10 == 1010)
            GJBaseGameLayer::updateColor(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
    }
};

class $modify (CCNode)
{
    static void onModify(auto& self) {
        std::vector<geode::Hook*> hooks;

        auto it = self.m_hooks.begin();
        std::advance(it, 0);

        hooks.push_back(it->second.get());

        Loader::get()->queueInMainThread([hooks] 
        {
            auto modu = Client::GetModule("show-layout");

            for (auto hook : hooks)
            {
                if (hook)
                {
                    hook->setAutoEnable(false);

                    if (!modu->enabled)
                        hook->disable();

                    modu->hooks.push_back(hook);
                }
            }
        });
    }

    void removeFromParentAndCleanup(bool p0) // shitty workaround but it is what it is
    {
        if (!PlayLayer::get() || !this->getParent())
            return CCNode::removeFromParentAndCleanup(p0);

        if (this->getParent()->getID() != std::string("IMPORTANT"_spr))
            return CCNode::removeFromParentAndCleanup(p0);
    }
};

class $modify(EffectGameObject) {

    static void onModify(auto& self) {
        std::vector<geode::Hook*> hooks;

        hooks.push_back(self.getHook("EffectGameObject::init").unwrap());

        Loader::get()->queueInMainThread([hooks] 
        {
            auto modu = Client::GetModule("show-layout");

            for (auto hook : hooks)
            {
                hook->setAutoEnable(false);

                if (!modu->enabled)
                    hook->disable();

                modu->hooks.push_back(hook);
            }
        });
    }

	bool init(char const* p0) {
		if (!EffectGameObject::init(p0)) 
            return false;

        if (!showLayout)
        {
            showLayout = Client::GetModule("show-layout");
            showLayoutCamera = showLayout->options[0];
        }

        if (!PlayLayer::get() || !showLayout->enabled)
            return true;

        if (std::find(objs.begin(), objs.end(), std::string(p0)) != objs.end())
            return false;

        if (!showLayoutCamera->enabled)
        {
            if (std::find(camera.begin(), camera.end(), std::string(p0)) != camera.end())
                return false;
        }

		return true;
	}
};

class $modify (GJGroundLayer)
{
    static void onModify(auto& self) {
        std::vector<geode::Hook*> hooks;

        hooks.push_back(self.getHook("GJGroundLayer::updateGround01Color").unwrap());
        hooks.push_back(self.getHook("GJGroundLayer::updateGround02Color").unwrap());

        Loader::get()->queueInMainThread([hooks] 
        {
            auto modu = Client::GetModule("show-layout");

            for (auto hook : hooks)
            {
                hook->setAutoEnable(false);

                if (!modu->enabled)
                    hook->disable();

                modu->hooks.push_back(hook);
            }
        });
    }

    void updateGround01Color(cocos2d::ccColor3B p0)
    {
        GJGroundLayer::updateGround01Color(PlayLayer::get() ? as<ColourModule*>(showLayout->options[2])->colour : p0);
    }

    void updateGround02Color(cocos2d::ccColor3B p0)
    {
        GJGroundLayer::updateGround02Color(PlayLayer::get() ? as<ColourModule*>(showLayout->options[2])->colour : p0);
    }
};

class $modify(GameObject) {

    static void onModify(auto& self) {
        std::vector<geode::Hook*> hooks;

        hooks.push_back(self.getHook("GameObject::setVisible").unwrap());
        hooks.push_back(self.getHook("GameObject::setOpacity").unwrap());
        hooks.push_back(self.getHook("GameObject::setObjectColor").unwrap());

        Loader::get()->queueInMainThread([hooks] 
        {
            auto modu = Client::GetModule("show-layout");

            for (auto hook : hooks)
            {
                hook->setAutoEnable(false);

                if (!modu->enabled)
                    hook->disable();

                modu->hooks.push_back(hook);
            }
        });
    }

    void setVisible(bool v) {
        if (!showLayout)
        {
            showLayout = Client::GetModule("show-layout");
            showLayoutCamera = showLayout->options[0];
        }

        if (!PlayLayer::get() || !showLayout->enabled)
            return GameObject::setVisible(v);
        
        //m_hasGroupParent == 0
        std::vector<int> outerPortal = {};
        //i really dont want to have to check every single object id
        if (m_objectType == GameObjectType::Decoration && m_objectID != 44) { // 44 being practice mode checkpoint, because thats a "decoration"
            GameObject::setVisible(false);
        } else {
            GameObject::setVisible(showLayout->enabled ? true : v);
        }
    }

    void setOpacity(unsigned char g) {

        if (!showLayout)
        {
            showLayout = Client::GetModule("show-layout");
            showLayoutCamera = showLayout->options[0];
        }

		GameObject::setOpacity(showLayout->enabled ? 255 : g);

        if (!PlayLayer::get() || !showLayout->enabled)
            return;

		if (true && m_objectType != GameObjectType::Decoration) { // true being layout enabled
			CCSpritePlus::setOpacity(255);
			if (m_glowSprite)
				m_glowSprite->setOpacity(255);
		}
	}

	void setObjectColor(const cocos2d::ccColor3B& cor) {
        if (!showLayout)
        {
            showLayout = Client::GetModule("show-layout");
            showLayoutCamera = showLayout->options[0];
        }

        if (!PlayLayer::get() || !showLayout->enabled)
            return GameObject::setObjectColor(cor);

		if (true && m_objectType != GameObjectType::Decoration) {
			GameObject::setObjectColor(ccc3(255, 255, 255));
		} else {
			GameObject::setObjectColor(cor);
		}
	}
};

#endif