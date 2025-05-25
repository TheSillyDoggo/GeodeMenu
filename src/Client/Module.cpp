#include "Module.h"

#include "../Layers/ModuleOptionsLayer.h"
#include "Dropdown.h"
#include "Client.h"
#include "../UI/TransLabelBMFont.hpp"
#include "../UI/TransAlertLayer.hpp"

void Module::drawImGui()
{
    float sbarWidth = ImGui::GetScrollMaxY() > 0.0f ? ImGui::GetStyle().ScrollbarSize : 0;

    if (enabled)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ccc4ToVec(Client::get()->getThemeColour("ModuleEnabled", ccc4(40, 40, 40, 255))));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ccc4ToVec(Client::get()->getThemeColour("ModuleEnabledHovered", ccc4(50, 50, 50, 255))));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ccc4ToVec(Client::get()->getThemeColour("ModuleEnabledActive", ccc4(50, 50, 50, 255))));
        ImGui::PushStyleColor(ImGuiCol_Text, ccc4ToVec(Client::get()->getThemeColour("ModuleEnabledText", Client::get()->accentColour)));
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ccc4ToVec(Client::get()->getThemeColour("ModuleDisabled", ccc4(40, 40, 40, 255))));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ccc4ToVec(Client::get()->getThemeColour("ModuleDisabledHovered", ccc4(50, 50, 50, 255))));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ccc4ToVec(Client::get()->getThemeColour("ModuleDisabledActive", ccc4(50, 50, 50, 255))));
        ImGui::PushStyleColor(ImGuiCol_Text, ccc4ToVec(Client::get()->getThemeColour("ModuleDisabledText", ccc4(255, 255, 255, 255))));
    }

    if (ImGui::Button(this->name.c_str(), ImVec2(Client::get()->widgetSize.x + (options.size() > 0 ? -Client::get()->widgetSize.y - sbarWidth : 0), Client::get()->widgetSize.y)))
    {
        onToggleAndroid(nullptr);
    }

    if (options.size() > 0)
    {
        ImGui::SameLine();

        ImGui::PushID(fmt::format("{}/options-btn", id).c_str());

        float offset = Client::get()->ini->getKeyValueFloat("Offsets::ModuleSettingOffset", "6");
        auto optionBtn = Client::get()->optionsModule == this ? Client::get()->accentColour : Client::get()->getThemeColour("ModuleOptionsDeselected", ccc4(70, 70, 70, 255));
        ImVec2 bottomLeftPos = ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x + offset, ImGui::GetWindowPos().y + ImGui::GetCursorPos().y + offset);

        if (ImGui::Button("", ImVec2(Client::get()->widgetSize.y, Client::get()->widgetSize.y)))
        {
            if (Client::get()->optionsModule == this)
                Client::get()->optionsModule = nullptr;
            else
                Client::get()->optionsModule = this;
        }

        ImGui::PopID();

        ImGui::GetWindowDrawList()->AddTriangleFilled(ImVec2(bottomLeftPos.x, bottomLeftPos.y + Client::get()->widgetSize.y - offset * 2), ImVec2(bottomLeftPos.x + Client::get()->widgetSize.y - offset * 2, bottomLeftPos.y), ImVec2(bottomLeftPos.x + Client::get()->widgetSize.y - offset * 2, bottomLeftPos.y + Client::get()->widgetSize.y - offset * 2), ImColor(ccc4ToVec(optionBtn)));
    }
    else
    {
        float offset = Client::get()->ini->getKeyValueFloat("Offsets::ModuleBarOffset", "3");
        auto optionBtn = enabled ? Client::get()->accentColour : Client::get()->getThemeColour("ModuleBarDeselected", ccc4(70, 70, 70, 255));
        ImVec2 topRightPos = ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x + Client::get()->widgetSize.x - offset - sbarWidth, ImGui::GetWindowPos().y + ImGui::GetCursorPos().y - Client::get()->widgetSize.y + offset);

        ImGui::GetWindowDrawList()->AddRectFilled(topRightPos, ImVec2(topRightPos.x - Client::get()->ini->getKeyValueFloat("ModuleBarSize::Width", "3"), topRightPos.y + Client::get()->ini->getKeyValueFloat("ModuleBarSize::Height", "18")), ImColor(ccc4ToVec(optionBtn)));
    }

    lastRenderedPosition = ImGui::GetItemRectMin();

    if (ImGui::GetMousePos().x >= ImGui::GetItemRectMin().x && ImGui::GetMousePos().y >= ImGui::GetItemRectMin().y && ImGui::GetMousePos().x <= ImGui::GetItemRectMax().x && ImGui::GetMousePos().y <= ImGui::GetItemRectMax().y)
        Client::get()->hoveredModule = this;

    ImGui::PopStyleColor(4);
}

void Module::onOptionsAndroid(CCObject* sender)
{
    auto dat = static_cast<Module*>(static_cast<CCNode*>(sender)->getUserData());

    auto pop = ModuleOptionsLayer::addToScene(dat);

    if (dat->delegate)
        dat->delegate->initOptionsLayer(pop);
}

void Module::onInfoAndroid(CCObject* sender)
{
    auto dat = static_cast<Module*>(static_cast<CCNode*>(sender)->getUserData());

    if (!dat)
        return;

    auto al = TransAlertLayer::create(dat->name.c_str(), dat->description.c_str(), "OK");
    al->show();
}

void Module::onToggleAndroid(CCObject* sender)
{
    if (!sender)
    {
        enabled = !enabled;
        save();
        onChange();

        if (enabled)
            enableHooks();
        else
            disableHooks();

        if (enabled)
            enablePatches();
        else
            disablePatches();

        if (onToggle)
            onToggle(enabled);

        return;
    }

    auto dat = static_cast<Module*>(static_cast<CCNode*>(sender)->getUserData());

    if (dat->isInComp)
    {
        as<CCMenuItemToggler*>(sender)->toggle(!as<CCMenuItemToggler*>(sender)->isToggled());
        return FLAlertLayer::create(dat->name.c_str(), dat->inCompAlert.c_str(), "OK")->show();
    }

    if (dat->useAlert && !dat->vAlert)
    {
        FLAlertLayer::create(dat->name.c_str(), dat->alert.c_str(), "OK")->show();
        dat->vAlert = true;
        as<CCMenuItemToggler*>(sender)->toggle(!as<CCMenuItemToggler*>(sender)->isToggled());

        return;
    }

    dat->enabled = !dat->enabled;
    dat->save();
    dat->onChange();

    if (dat->enabled)
        dat->enableHooks();
    else
        dat->disableHooks();

    if (dat->enabled)
        dat->enablePatches();
    else
        dat->disablePatches();

    if (dat->onToggle)
        dat->onToggle(dat->enabled);
}

void Module::makeAndroid(CCNode* menu, CCPoint pos)
{
    if (inCompAlert.empty())
        this->isInComp = false;

    auto btn = CCMenuItemToggler::createWithStandardSprites(menu, menu_selector(Module::onToggleAndroid), 0.75f);
    btn->setUserData(this);
    btn->setID(id);
    btn->toggle(enabled);
    btn->setPosition(pos);

    auto label = TransLabelBMFont::create(name, "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setScale(0.575f);
    label->setPosition(pos + ccp(15, 0));
    label->limitLabelWidth(110 - ((options.size() != 0) ? 3 : 0) + (isInComp ? 17.5f : 0), 0.575f, 0.1f);

    if (description.size() != 0 && options.size() == 0 && !isInComp)
    {
        auto info = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"), menu, menu_selector(Module::onInfoAndroid));
        info->setScale(0.55f);
        info->m_baseScale = info->getScale();
        info->setPosition(pos + ccp(label->getScaledContentSize().width + 30, 0) + ccp(-5, 5));
        info->setContentSize(info->getContentSize() * 2.5f);
        static_cast<CCSprite*>(info->getChildren()->objectAtIndex(0))->setPosition(info->getContentSize() / 2);
        info->setUserData(this);

        menu->addChild(info);

        label->setOnLabelUpdated([=]
        {
            info->setPosition(pos + ccp(label->getScaledContentSize().width + 30, 0) + ccp(-5, 5));
        });
    }

    if (options.size() != 0 && !isInComp)
    {
        auto options = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_plus3Btn_001.png"), menu, menu_selector(Module::onOptionsAndroid));
        options->setUserData(this);
        options->setPosition(pos + ccp(label->getScaledContentSize().width + 30, 0) + ccp(-2, 0));

        menu->addChild(options);

        label->setOnLabelUpdated([=]
        {
            options->setPosition(pos + ccp(label->getScaledContentSize().width + 30, 0) + ccp(-2, 0));
        });
    }

    if (isInComp)
    {
        label->setOpacity(150);
        btn->m_onButton->setOpacity(150);
        btn->m_offButton->setOpacity(150);
    }

    menu->addChild(btn);
    menu->addChild(label);
}

void Module::setIncompatible(std::string str)
{
    this->isInComp = true;
    this->inCompAlert = str;
    if (this->enabled)
    {
        this->enabled = false;
        this->onChange();
    }
}

void Module::addHookRaw(Result<Hook*> hook)
{
    if (!hook.isOk() || !hook.isOk())
        return log::error("Error adding hook: {}", hook.err());

    addHook(hook.unwrapOr(nullptr));
}

void Module::addHook(Hook* hook)
{
    if (hook)
    {
        hooks.push_back(hook);
        hook->setAutoEnable(false);

        if (!enabled)
            (void)hook->disable();
    }
}

void Module::addPatch(Patch* patch)
{
    if (patch)
    {
        patches.push_back(patch);
        patch->setAutoEnable(false);

        if (!enabled)
            (void)patch->disable();
    }
}

void Module::disableHooks()
{
    for (auto hook : hooks)
    {
        if (hook)
        {
            auto v = hook->disable();
            if (v.isErr())
            {
                log::error("Error Disabling hook: {}, {}", hook->getDisplayName(), v.err());
            }
        }
    }
}

void Module::enableHooks()
{
    for (auto hook : hooks)
    {
        if (hook)
        {
            auto v = hook->enable();
            if (v.isErr())
            {
                log::error("Error Enabling hook: {}, {}", hook->getDisplayName(), v.err());
            }
        }
    }
}

void Module::disablePatches()
{
    for (auto patch : patches)
    {
        if (patch)
        {
            auto v = patch->disable();
            if (v.isErr())
            {
                log::error("Error Disabling patch: {}, {}", patch->getAddress(), v.err());
            }
        }
    }
}

void Module::enablePatches()
{
    for (auto patch : patches)
    {
        if (patch)
        {
            auto v = patch->enable();
            if (v.isErr())
            {
                log::error("Error Enabling patch: {}, {}", patch->getAddress(), v.err());
            }
        }
    }
}

CCSize Module::sizeForOptionsPage()
{
    // idk man
    return optionSizeForce == CCSizeZero ? CCSizeMake(350, std::ceil((std::max<int  >(options.size(), 3) - 1) / 2) * 35 + 110) : optionSizeForce;
}

Module* Module::get(std::string id)
{
    if (modules.contains(id))
        return modules[id];

    // https://youtu.be/r98QiJp3vX4?t=22
    log::error("Missing module: {}", id);

    return nullptr;
}

void Module::addToCache()
{
    modules.emplace(id, this);
}