#include "../../Client/Module.hpp"
#include <Geode/modify/EditorOptionsLayer.hpp>

using namespace geode::prelude;

class ToolboxButtonBypass : public Module
{
    public:
        MODULE_SETUP(ToolboxButtonBypass)
        {
            setName("Toolbox Button Bypass");
            setID("toolbox-buttons");
            setCategory("Creator");
            setDescription("Unlimited <cc>Rows</c> and <cc>Columns</c> in your <cl>Object Toolbox</c>");
        }
};

SUBMIT_HACK(ToolboxButtonBypass);

class $modify (EditorOptionsLayer)
{
    void onButtonRows(cocos2d::CCObject* sender)
    {
        if (!ToolboxButtonBypass::get()->getRealEnabled())
            return EditorOptionsLayer::onButtonRows(sender);

        m_buttonRows += sender->getTag() ? 1 : -1;
        m_buttonRows = std::max<int>(1, m_buttonRows);
        m_buttonRowsLabel->setString(fmt::format("{}", m_buttonRows).c_str());
    }

    void onButtonsPerRow(cocos2d::CCObject* sender)
    {
        if (!ToolboxButtonBypass::get()->getRealEnabled())
            return EditorOptionsLayer::onButtonsPerRow(sender);

        m_buttonsPerRow += sender->getTag() ? 1 : -1;
        m_buttonsPerRow = std::max<int>(1, m_buttonsPerRow);
        m_buttonsPerRowLabel->setString(fmt::format("{}", m_buttonsPerRow).c_str());
    }
};