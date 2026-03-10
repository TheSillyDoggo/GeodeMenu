#pragma once

#include "../../Client/ButtonModule.hpp"
#include <LocalisationManager.hpp>
#include <UI/LanguageOptionsUI.hpp>

using namespace geode::prelude;

class TempLoadLanguage : public ButtonModule
{
    public:
        MODULE_SETUP(TempLoadLanguage)
        {
            setID("temp-load-language");
        }

        virtual void onClick()
        {
            #if GEODE_COMP_GD_VERSION >= 22081

            file::FilePickOptions options;
            options.defaultPath = Mod::get()->getConfigDir();

            file::FilePickOptions::Filter filter;
            filter.description = "JSON File";
            filter.files = { "*.json" };
            options.filters.push_back(filter);

            async::spawn(file::pick(file::PickMode::OpenFile, options), [this](Result<std::optional<std::filesystem::path>> result)
            {
                if (result.isOk())
                {
                    if (result.unwrap().has_value())
                    {
                        auto filePath = result.unwrap().value_or("");

                        if (LanguageOptionsUI::get())
                            LanguageOptionsUI::get()->onClose(nullptr);

                        LocalisationManager::get()->switchTempPath(filePath);
                    }
                }
            });

            #endif
        }
};

SUBMIT_HACK(TempLoadLanguage);