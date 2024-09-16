#include "QOLModMenuBG.hpp"

bool QOLModMenuBG::init()
{
    if (!CCNode::init())
        return false;

    colouredBG = CCScale9Sprite::create("GJ_square01.png");
    outlineSpr = CCScale9Sprite::create("GJ_square07.png");

    if (auto gradPages = Loader::get()->getLoadedMod("thesillydoggo.gradientpages"))
    {
        gradientBG = CCLayerGradient::create(ccc4(0, 0, 0, 255), ccc4(0, 0, 0, 255));
        gradientBG->ignoreAnchorPointForPosition(false);

        if (gradPages->getSettingValue<bool>("use-custom-colours"))
        {
            gradientBG->setStartColor(gradPages->getSettingValue<ccColor3B>("primary-colour"));
            gradientBG->setEndColor(gradPages->getSettingValue<ccColor3B>("secondary-colour"));
        }
        else
        {
            auto gm = GameManager::get();

            gradientBG->setStartColor(gm->colorForIdx(gm->m_playerColor.value()));
            gradientBG->setEndColor(gm->colorForIdx(gm->m_playerColor2.value()));
        }

        if (gradPages->getSettingValue<bool>("reverse-order"))
            gradientBG->setScaleY(-1);

        gradientOutline = CCScale9Sprite::createWithSpriteFrameName("thesillydoggo.gradientpages/square-outline.png");
        gradientDarken = CCScale9Sprite::createWithSpriteFrameName("thesillydoggo.gradientpages/square-fill.png");

        this->addChildAtPosition(gradientBG, Anchor::Center);
        this->addChildAtPosition(gradientOutline, Anchor::Center);
        this->addChildAtPosition(gradientDarken, Anchor::Center);
    }

    updateTheme(Mod::get()->getSavedValue<int>("theme", 2));

    this->addChildAtPosition(colouredBG, Anchor::Center);
    this->addChildAtPosition(outlineSpr, Anchor::Center);
    return true;
}

void QOLModMenuBG::updateTheme(int theme)
{
    this->theme = theme;

    public_cast(colouredBG, _scale9Image)->setTexture(CCSprite::create(fmt::format("GJ_square0{}.png", theme < 0 ? 6 : theme).c_str())->getTexture());

    colouredBG->setColor(theme == -2 ? ccc3(0, 0, 0) : ccc3(255, 255, 255));
    colouredBG->setOpacity(theme == -2 ? 175 : 255);

    colouredBG->setVisible(theme != -1);
    outlineSpr->setVisible(theme == -2);

    if (gradientBG)
    {
        gradientBG->setVisible(theme == -1);
        gradientOutline->setVisible(theme == -1);
        gradientDarken->setVisible(theme == -1 && gradientDarkenVisible);
    }
}

void QOLModMenuBG::setGradientDarkenVisible(bool visible)
{
    gradientDarkenVisible = visible;

    if (gradientDarken)
        gradientDarken->setVisible(theme == -1 && gradientDarkenVisible);
}

void QOLModMenuBG::setContentSize(const CCSize& contentSize)
{
    CCNode::setContentSize(contentSize);

    colouredBG->setContentSize(contentSize);
    outlineSpr->setContentSize(contentSize);

    if (gradientBG)
    {
        gradientBG->setContentSize(contentSize);
        gradientOutline->setContentSize(contentSize);
        gradientDarken->setContentSize(contentSize - ccp(15, 15));
    }
}