#include "InputModule.hpp"
#include <imgui-cocos.hpp>

InputModule* mod;

void playSound()
{
    FMODAudioEngine::get()->playEffect("start.wav"_spr);
}

int imguiTextInputChanged(ImGuiInputTextCallbackData* data)
{
    int ret = 0;

    if (ImGui::IsKeyPressed(ImGuiKey_Backspace) && data->BufTextLen == 0)
        playSound();

    if (data->BufTextLen > mod->maxSize)
    {
        mod->text = mod->text.substr(0, mod->maxSize);

        playSound();
        return 0;
    }
    
    return ret;
}

void InputModule::drawImGui()
{
    mod = this;

    auto t = text.c_str();

    if (ImGui::InputText(name.c_str(), (char*)text.c_str(), maxSize, ImGuiInputTextFlags_CallbackAlways, imguiTextInputChanged))
    {
        
    }
    //text = t;
}

void InputModule::makeAndroid(CCNode* menu, CCPoint pos)
{
    auto label = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setScale(0.575f);
    label->setPosition(pos - ccp(10, 0));
    label->limitLabelWidth(70, 0.575f, 0.1f);

    auto input = TextInput::create(100, name.c_str());
    input->setMaxCharCount(maxSize);
    input->getInputNode()->setAllowedChars(allowedChars);
    input->setPosition(pos + ccp(70, 0));
    input->setAnchorPoint(ccp(0, 0.5f));
    input->setDelegate(this);
    input->getInputNode()->setID("IGNOREBYPASSES"_spr);
    input->setString(text);

    this->inputField = input;

    menu->addChild(input);
    menu->addChild(label);
}

void InputModule::textChanged(CCTextInputNode* input)
{
    text = input->getString();

    this->save();
    onChange();
}

InputModule::InputModule()
{
    this->load();
}

InputModule::InputModule(std::string name, std::string id, std::string def)
{
    this->name = name;
    this->id = id;
    this->text = def;

    this->load();
}

void InputModule::save()
{
    Mod::get()->setSavedValue<std::string>(id + "_value", text);
}

void InputModule::load()
{
    text = Mod::get()->getSavedValue<std::string>(id + "_value", text);
    
    Loader::get()->queueInMainThread([this]{
        if (this->text.size() > maxSize)
            this->text = this->text.substr(0, maxSize);
    });
}

void InputModule::updateValue()
{
    lastCheckedText = text;
    floatValue = 1;
    intValue = 0;

    auto x = numFromString<float>(text);

    floatValue = x.unwrapOr(1);

    x = numFromString<int>(text);

    if (x.isOk())
        intValue = x.value();

    if (floatValue < 0.01f)
        floatValue = 0.01f;

    if (floatValue > 99999)
        floatValue = 99999;
}

float InputModule::getFloatValue()
{
    if (lastCheckedText != text)
        updateValue();

    return floatValue;
}

int InputModule::getIntValue()
{
    if (lastCheckedText != text)
        updateValue();

    return intValue;
}