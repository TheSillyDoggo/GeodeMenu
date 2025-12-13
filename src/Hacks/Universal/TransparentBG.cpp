#include "../../Client/Module.hpp"
#include "../../Client/ColourModule.hpp"
#include <Geode/modify/AppDelegate.hpp>
#include <Geode/modify/AchievementNotifier.hpp>

using namespace geode::prelude;

class TransparentBG : public Module
{
    public:
        MODULE_SETUP(TransparentBG)
        {
            setName("Transparent BG");
            setID("trans-bg");
            setCategory("Universal");
            setDescription("Sets the colour for all gradient backgrounds to white");
        }

        CCSprite* getBG(CCScene* scene);
        virtual void onToggle();

        void onUpdate(float);
};

class TransparentBGColour : public ColourModule
{
    public:
        MODULE_SETUP(TransparentBGColour)
        {
            setName("Colour:");
            setID("trans-bg/colour");
            setDefaultConfig({ ccc3(255, 255, 255) });
            setPriority(1);
        }
};

class TransparentBGOnlyBlue : public Module
{
    public:
        MODULE_SETUP(TransparentBGOnlyBlue)
        {
            setName("Only Blue BG");
            setID("trans-bg/only-blue");
            setDescription("Only changes the colour of blue backgrounds.\nCan mess up some areas like the vaults if disabled");
            setDefaultEnabled(true);
            setPriority(2);
        }
};

SUBMIT_HACK(TransparentBG);
SUBMIT_OPTION(TransparentBG, TransparentBGColour);
SUBMIT_OPTION(TransparentBG, TransparentBGOnlyBlue);

CCSprite* TransparentBG::getBG(CCScene* scene)
{
    if (scene->getChildrenCount() > 0)
    {
        if (auto l = scene->getChildByType<CCLayer>(0))
        {
            if (scene->getChildByType<LevelEditorLayer>(0))
                return nullptr;

            if (scene->getChildByType<LoadingLayer>(0))
                return nullptr;

            if (!l->getChildren())
                return nullptr;

            l->sortAllChildren();

            if (auto b = typeinfo_cast<CCSprite*>(l->getChildren()->objectAtIndex(0)))
            {
                if (!b->getUserObject("default-colour_r"_spr))
                {
                    if (TransparentBGOnlyBlue::get()->getRealEnabled())
                    {
                        if (!(b->getColor().r == 0 && b->getColor().g == 102 && b->getColor().b == 255))
                        {
                            return nullptr;
                        }
                    }

                    b->setUserObject("default-colour_r"_spr, CCInteger::create(b->getColor().r));
                    b->setUserObject("default-colour_g"_spr, CCInteger::create(b->getColor().g));
                    b->setUserObject("default-colour_b"_spr, CCInteger::create(b->getColor().b));
                }

                return b;
            }
        }
    }

    return nullptr;
}

void TransparentBG::onToggle()
{
    if (auto bg = getBG(CCScene::get()))
    {
        if (getRealEnabled())
            bg->schedule(schedule_selector(TransparentBG::onUpdate));
        else
        {
            bg->unschedule(schedule_selector(TransparentBG::onUpdate));
            bg->setColor(ccc3(
                static_cast<CCInteger*>(bg->getUserObject("default-colour_r"_spr))->getValue(),
                static_cast<CCInteger*>(bg->getUserObject("default-colour_g"_spr))->getValue(),
                static_cast<CCInteger*>(bg->getUserObject("default-colour_b"_spr))->getValue()
            ));
        }
    }
}

void TransparentBG::onUpdate(float)
{
    auto self = reinterpret_cast<CCSprite*>(this);

    self->setColor(TransparentBGColour::get()->getColour());
}

#ifdef __APPLE__
class $modify (AchievementNotifier)
#else
class $modify (AppDelegate)
#endif
{
    virtual void willSwitchToScene(CCScene* scene)
    {
        #ifdef __APPLE__
        AchievementNotifier::willSwitchToScene(scene);
        #else
        AppDelegate::willSwitchToScene(scene);
        #endif

        if (!TransparentBG::get()->getRealEnabled())
            return;

        if (auto bg = TransparentBG::get()->getBG(scene))
        {
            bg->schedule(schedule_selector(TransparentBG::onUpdate));
        }
    }
};