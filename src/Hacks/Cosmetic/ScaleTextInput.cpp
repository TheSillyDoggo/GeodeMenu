#include <Geode/Geode.hpp>
#include <../../../build/bindings/bindings/Geode/modify/GJScaleControl.hpp>
#include "../../Client/Client.h"

using namespace geode::prelude;

class $modify (InputScaleControl, GJScaleControl)
{
    struct Fields
    {
        TextInput* scaleXInput;
        TextInput* scaleYInput;
        TextInput* scaleXYInput;

        CCLabelBMFont* scaleXLabel;
        CCLabelBMFont* scaleYLabel;
        CCLabelBMFont* scaleXYLabel;
    };

    void updateScaleXY(std::string str)
    {
        auto v = numFromString<float>(str);

        if (v.isOk())
        {
            auto value = v.unwrapOr(1);
            auto wrappedValue = unscaleFloat(value, m_lowerBound, m_upperBound);

            m_sliderXY->setValue(clamp<float>(wrappedValue, 0, 1));
            //this->sliderChanged(m_sliderXY->m_touchLogic);

            if (EditorUI::get())
            {
                EditorUI::get()->scaleXYChanged(value, value, m_scaleLocked);
            }
        }
    }

    void updateScaleX(std::string str)
    {
        auto v = numFromString<float>(str);

        if (v.isOk())
        {
            auto value = v.unwrapOr(1);
            auto wrappedValue = unscaleFloat(value, m_lowerBound, m_upperBound);

            m_sliderX->setValue(clamp<float>(wrappedValue, 0, 1));
            //this->sliderChanged(m_sliderX->m_touchLogic);

            if (EditorUI::get())
            {
                EditorUI::get()->scaleXChanged(value, m_scaleLocked);
            }
        }
    }

    void updateScaleY(std::string str)
    {
        auto v = numFromString<float>(str);

        if (v.isOk())
        {
            auto value = v.unwrapOr(1);
            auto wrappedValue = unscaleFloat(value, m_lowerBound, m_upperBound);

            m_sliderY->setValue(clamp<float>(wrappedValue, 0, 1));
            //this->sliderChanged(m_sliderY->m_touchLogic);

            if (EditorUI::get())
            {
                EditorUI::get()->scaleYChanged(value, m_scaleLocked);
            }
        }
    }

    void addControl()
    {
        if (Loader::get()->getLoadedMod("hjfod.betteredit"))
            return;

        m_scaleLabel->setOpacity(0);
        m_scaleXLabel->setOpacity(0);
        m_scaleYLabel->setOpacity(0);

        auto scaleXYInput = TextInput::create(70, "Scale");
        scaleXYInput->setScale(0.6f);
        scaleXYInput->setPosition(ccp(32, 28));
        scaleXYInput->setFilter("1234567890.");
        scaleXYInput->setCallback([this](const std::string& str){
            updateScaleXY(str);
        });

        auto scaleXInput = TextInput::create(70, "Scale");
        scaleXInput->setScale(0.6f);
        scaleXInput->setPosition(ccp(32, 28));
        scaleXInput->setFilter("1234567890.");
        scaleXInput->setCallback([this](const std::string& str){
            updateScaleX(str);
        });

        auto scaleYInput = TextInput::create(70, "Scale");
        scaleYInput->setScale(0.6f);
        scaleYInput->setPosition(ccp(32, 28));
        scaleYInput->setFilter("1234567890.");
        scaleYInput->setCallback([this](const std::string& str){
            updateScaleY(str);
        });

        auto scaleXLabel = CCLabelBMFont::create("ScaleX:", "bigFont.fnt");
        scaleXLabel->setScale(0.5f);
        scaleXLabel->setAnchorPoint(ccp(1, 0.5f));
        scaleXLabel->setPosition(m_scaleXLabel->getPosition());

        auto scaleYLabel = CCLabelBMFont::create("ScaleY:", "bigFont.fnt");
        scaleYLabel->setScale(0.5f);
        scaleYLabel->setAnchorPoint(ccp(1, 0.5f));
        scaleYLabel->setPosition(m_scaleYLabel->getPosition() - m_sliderY->getPosition());

        auto scaleXYLabel = CCLabelBMFont::create("Scale:", "bigFont.fnt");
        scaleXYLabel->setScale(0.5f);
        scaleXYLabel->setAnchorPoint(ccp(1, 0.5f));
        scaleXYLabel->setPosition(m_scaleLabel->getPosition());

        m_sliderX->addChild(scaleXInput);
        m_sliderY->addChild(scaleYInput);
        m_sliderXY->addChild(scaleXYInput);

        m_sliderX->addChild(scaleXLabel);
        m_sliderY->addChild(scaleYLabel);
        m_sliderXY->addChild(scaleXYLabel);

        m_fields->scaleXInput = scaleXInput;
        m_fields->scaleYInput = scaleYInput;
        m_fields->scaleXYInput = scaleXYInput;

        m_fields->scaleXLabel = scaleXLabel;
        m_fields->scaleYLabel = scaleYLabel;
        m_fields->scaleXYLabel = scaleXYLabel;

        m_fields->scaleXInput->setString(fmt::format("{:.2f}", m_valueX));
        m_fields->scaleYInput->setString(fmt::format("{:.2f}", m_valueY));
        m_fields->scaleXYInput->setString(fmt::format("{:.2f}", m_valueX > m_valueY ? m_valueY : m_valueX));
    }

    void removeControl()
    {
        if (Loader::get()->getLoadedMod("hjfod.betteredit"))
            return;

        m_scaleLabel->setOpacity(255);
        m_scaleXLabel->setOpacity(255);
        m_scaleYLabel->setOpacity(255);

        m_fields->scaleXInput->removeFromParent();
        m_fields->scaleYInput->removeFromParent();
        m_fields->scaleXYInput->removeFromParent();

        m_fields->scaleXLabel->removeFromParent();
        m_fields->scaleYLabel->removeFromParent();
        m_fields->scaleXYLabel->removeFromParent();
    }

    void loadValues(GameObject* p0, cocos2d::CCArray* p1, gd::unordered_map<int, GameObjectEditorState>& p2)
    {
        GJScaleControl::loadValues(p0, p1, p2);

        if (Loader::get()->getLoadedMod("hjfod.betteredit"))
            return;

        m_fields->scaleXInput->setString(fmt::format("{:.2f}", m_valueX));
        m_fields->scaleYInput->setString(fmt::format("{:.2f}", m_valueY));
        m_fields->scaleXYInput->setString(fmt::format("{:.2f}", m_valueX > m_valueY ? m_valueY : m_valueX));
    }

    /*
    void updateLabelX(float p0)
    {
        GJScaleControl::updateLabelX(p0);

        if (Loader::get()->getLoadedMod("hjfod.betteredit"))
            return;

        m_fields->scaleXInput->setString(fmt::format("{:.2f}", p0));
    }

    void updateLabelY(float p0)
    {
        GJScaleControl::updateLabelY(p0);

        if (Loader::get()->getLoadedMod("hjfod.betteredit"))
            return;

        m_fields->scaleYInput->setString(fmt::format("{:.2f}", p0));
    }
    
    void updateLabelXY(float p0)
    {
        GJScaleControl::updateLabelXY(p0);

        if (Loader::get()->getLoadedMod("hjfod.betteredit"))
            return;

        m_fields->scaleXYInput->setString(fmt::format("{:.2f}", p0));
    }
    */

    virtual bool init()
    {
        if (!GJScaleControl::init())
            return false;

        if (!Loader::get()->getLoadedMod("hjfod.betteredit"))
            addControl();

        return true;
    }

    QOLMOD_MOD_ALL_HOOKS("scale-input-control")
};

$execute
{
    Loader::get()->queueInMainThread([] {
        Client::GetModule("scale-input-control")->onToggle = [](bool enabled){
            if (auto editor = LevelEditorLayer::get())
            {
                if (auto ui = editor->m_editorUI)
                {
                    if (auto control = as<InputScaleControl*>(ui->m_scaleControl))
                    {
                        if (enabled)
                            control->addControl();
                        else
                            control->removeControl();
                    }
                }
            }
        };
    });
}