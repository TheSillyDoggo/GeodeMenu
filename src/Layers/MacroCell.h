/*#include <Geode/Geode.hpp>
#include <Geode/modify/TextAlertPopup.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class MacroCell : public CCLayerColor
{
    public:
        std::string name;

        void onLoad(CCObject*)
        {
            std::string name = this->name;

            if (GJReplayManager::replay.inputs.size() != 0)
            {
                geode::createQuickPopup(
                    "Overwrite Macro?",
                    "Are you sure you want to load this <cl>macro</c>? This <cr>will</c> overwrite any non saved macro!",
                    "Cancel", "Load",
                    [name, this](auto, bool btn2) {
                        if (btn2) {
                            
                            std::stringstream ss;
                            ss << name;
                            ss << ".gdr";

                            auto path = Mod::get()->getConfigDir() / "macros" / ss.str();

                            log::info("Opening macro: {}", path);

                            auto v = utils::file::readString(path);

                            if (v.isOk())
                            {
                                GJReplayManager::replay = MyReplay::importDataS(v.value());

                                CCScene::get()->addChild(TextAlertPopup::create("Successfully loaded '" + name + ".gdr'", 0.5f, 0.6f, 150, ""), 9999999);


                                this->getParent()->getParent()->getParent()->getParent()->removeFromParent();
                                return;
                            }

                            CCScene::get()->addChild(TextAlertPopup::create("Error loading '" + name + ".gdr'", 0.5f, 0.6f, 150, ""), 9999999);
                            this->getParent()->getParent()->getParent()->getParent()->removeFromParent();

                        }
                    }
                );

                return;
            }

            std::stringstream ss;
            ss << name;
            ss << ".gdr";

            auto path = Mod::get()->getConfigDir() / "macros" / ss.str();

            log::info("Opening macro: {}", path);

            auto v = utils::file::readString(path);

            if (v.isOk())
            {
                GJReplayManager::replay = MyReplay::importDataS(v.value());

                CCScene::get()->addChild(TextAlertPopup::create("Successfully loaded '" + name + ".gdr'", 0.5f, 0.6f, 150, ""), 9999999);

                this->getParent()->getParent()->getParent()->getParent()->removeFromParent();
                return;
            }

            CCScene::get()->addChild(TextAlertPopup::create("Error loading '" + name + ".gdr'", 0.5f, 0.6f, 150, ""), 9999999);
            this->getParent()->getParent()->getParent()->getParent()->removeFromParent();
        }

        bool initWithUnloadedMacro(std::string name, int i)
        {
            ccColor4B col = ccc4(0, 0, 0, 25);
            if (i % 2 == 0)
                col = ccc4(0, 0, 0, 75);

            if (!CCLayerColor::initWithColor(col))
                return false;

            this->name = name;

            this->setContentSize(ccp(320, 35));
            this->setAnchorPoint(ccp(0, 0));

            auto title = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
            title->limitLabelWidth(230, 0.65f, 0.1f);
            title->setAnchorPoint(ccp(0, 1));
            this->addChildAtPosition(title, Anchor::TopLeft, ccp(3, -3));

            auto menu = CCMenu::create();
            menu->setContentSize(this->getContentSize());
            menu->setPosition(ccp(0, 0));
            menu->setAnchorPoint(ccp(0, 0));
            menu->ignoreAnchorPointForPosition(false);
            this->addChild(menu);

            auto btn = ButtonSprite::create("Load");
            btn->updateBGImage("GJ_button_04.png");
            btn->setScale(0.75f);
            auto load = CCMenuItemSpriteExtra::create(btn, this, menu_selector(MacroCell::onLoad));
            menu->addChildAtPosition(load, Anchor::Right, ccp(-35, 0));
    
            return true;
        }

        static MacroCell* createWithUnloadedMacro(std::string name, int i)
        {
            MacroCell* pRet = new MacroCell();
            if (pRet && pRet->initWithUnloadedMacro(name, i)) {
                pRet->autorelease();
                return pRet;
            } else {
                delete pRet;
                return nullptr;
            }
        }
};
*/