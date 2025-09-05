#include "../../Client/Module.hpp"
#include <Geode/modify/ShareLevelLayer.hpp>

using namespace geode::prelude;

class CustomStarRating : public Module
{
    public:
        MODULE_SETUP(CustomStarRating)
        {
            setName("Custom star rating");
            setID("custom-star-rating");
            setCategory("Creator");
            setDescription("Allows you to set a custom star rating on a level");
        }
};

SUBMIT_HACK(CustomStarRating);

class $modify (ShareLevelLayer)
{
    bool init(GJGameLevel* level)
    {
        if (!ShareLevelLayer::init(level))
            return false;

        if (CustomStarRating::get()->getRealEnabled())
        {
            for (auto btn : CCArrayExt<CCNode*>(m_starButtons))
            {
                btn->setVisible(false);
            }

            m_difficultySprite->setVisible(false);

            auto input = TextInput::create(90, "Stars");
            input->setPosition(CCDirector::get()->getWinSize() / 2 + ccp(30, -25));
            input->setCommonFilter(CommonFilter::Int);
            input->setMaxCharCount(3);
            input->setString(fmt::format("{}", level->m_stars));
            input->setCallback([this, input, level](const std::string& str)
            {
                auto stars = utils::numFromString<int>(str).unwrapOr(level->m_stars.value());

                level->m_stars = stars;
            });

            auto lbl = CCLabelBMFont::create("Stars:", "bigFont.fnt");
            lbl->setAnchorPoint(ccp(1, 0.5f));
            lbl->setPosition(input->getPosition() + ccp(-60, 0));
            lbl->setScale(0.5f);

            auto lblHelp = CCLabelBMFont::create("Value should be between 0 - 127 to be accepted by the servers", "chatFont.fnt");
            lblHelp->setScale(0.4f);
            lblHelp->setPosition(ccp(CCDirector::get()->getWinSize().width / 2, input->getPositionY() + -25));

            m_mainLayer->addChild(input);
            m_mainLayer->addChild(lbl);
            m_mainLayer->addChild(lblHelp);
        }

        return true;
    }
};