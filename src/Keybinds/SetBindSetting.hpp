#pragma once
#include <Geode/Geode.hpp>
#include <Geode/loader/SettingNode.hpp>
#include "../Utils/Utils.hpp"

using namespace geode::prelude;

class SetBindValue;

class SetBindValue : public SettingValue
{
    public:
        static inline SetBindValue* instance = nullptr;
        
        std::vector<int> buttons = { enumKeyCodes::KEY_Tab, enumKeyCodes::KEY_Insert };

        SetBindValue(std::string const& key, std::string const& modID)
        : SettingValue(key, modID) {}

        bool load(matjson::Value const& json) override {
            instance = this;

            if (!json.is_array())
                return true;

            buttons.clear();

            auto btns = json.as_array();

            for (auto btn : btns)
            {
                if (btn.is_number())
                    buttons.push_back(btn.as_int());
            }

            return true;
        }

        bool save(matjson::Value& json) const override {
            matjson::Array v;

            for (auto btn : buttons)
            {
                v.push_back(btn);
            }

            json = v;

            return true;
        }

        SettingNode* createNode(float width) override;
};


class SetBindNode : public SettingNode
{
    public:
        std::vector<int> startButtons;
        std::vector<int> buttons;

        CCMenu* menu;

        std::map<int, CCNode*> btns = {

        };

        std::string nameForKey(int key);

        bool init(SetBindValue* value, float width);

        void onAddBtn(CCObject* sender);
        void onSetBtn(CCObject* sender);

        void onKeySubmit(CCObject* sender);

        void commit() override {
            as<SetBindValue*>(m_value)->buttons = buttons;
            startButtons = buttons;
            this->dispatchCommitted();

            if (!Mod::get()->saveData().isOk())
                log::error("Error saving value");
        }

        bool hasUncommittedChanges() override {
            return buttons != startButtons;
        }

        bool hasNonDefaultValue() override {
            if (as<SetBindValue*>(m_value)->buttons.size() != 3)
                return true;

            if (as<SetBindValue*>(m_value)->buttons[0] != enumKeyCodes::KEY_Tab)
                return true;

            if (as<SetBindValue*>(m_value)->buttons[0] != enumKeyCodes::KEY_Insert)
                return true;

            return false;
        }

        void changed()
        {
            this->dispatchChanged();
        }

        void resetToDefault() override {
            as<SetBindValue*>(m_value)->buttons = { enumKeyCodes::KEY_Tab, enumKeyCodes::KEY_Insert };
        }
        
        static SetBindNode* create(SetBindValue* value, float width) {
            auto ret = new SetBindNode;
            if (ret && ret->init(value, width)) {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }
};