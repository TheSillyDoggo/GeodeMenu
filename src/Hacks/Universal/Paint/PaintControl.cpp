#include "PaintControl.hpp"
#include <FloatingButton/FloatingUIManager.hpp>
#include <ColourUtils.hpp>
#include <BlurAPI.hpp>
#include <Utils.hpp>
#include "Paint.hpp"
#include "PaintNode.hpp"
#include <RealtimeAction.hpp>

using namespace geode::prelude;
using namespace qolmod;

PaintControl* PaintControl::get()
{
    static Ref<PaintControl> instance = nullptr;

    if (!instance)
    {
        instance = create();
    }

    return instance;
}

bool PaintControl::init()
{
    if (!FloatingUIButton::init())
        return false;

    setMovable(true);
    setBaseScale(0.8f);
    setBaseOpacity(1);
    setOnClick([this]
    {
        toggleExpanded(!isExpanded);
    });
    updateSprites("geode.loader/baseCircle_Medium_Gray.png", "icon effects.png"_spr, true, true);
    this->updatePosition(ccp(
        Mod::get()->getSavedValue<float>("paintcontrol_position.x", 25),
        Mod::get()->getSavedValue<float>("paintcontrol_position.y", CCDirector::get()->getWinSize().height - 25)
    ));
    this->setPosition(position);

    bg = EasyBG::create();
    bg->setAnchorPoint(ccp(0, 0.5f));
    bg->setPosition(getContentSize() / 2);
    bg->setContentSize(ccp(150, 26));
    bg->setScaleX(0);
    BlurAPI::addBlur(bg);

    menu = CCMenu::create();
    menu->ignoreAnchorPointForPosition(false);
    menu->setAnchorPoint(ccp(0, 0));
    menu->setPosition(getContentSize() / 2);
    menu->setScaleX(0);
    menu->setTouchEnabled(false);

    addToolButton(PaintTool::Brush);
    addToolButton(PaintTool::Rubber);
    addToolButton(PaintTool::Clear);

    colourSpr = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    colourSpr->setScale(0.55f);

    auto colourBtn = Button::create(colourSpr, this, menu_selector(PaintControl::onChangeColour));
    colourBtn->setPosition(ccp(bg->getContentWidth() - bg->getContentHeight() / 2, 0));
    menu->addChild(colourBtn);

    input = BetterInputNode::create(22, "");
    input->setContentSize(ccp(19, 19));
    input->setPosition(ccp(colourBtn->getPositionX() - 25, 0));
    input->setCharFilter("1234567890");
    input->setMaxChars(2);
    input->setNumHoldValues(true, 1, 20, 0);
    input->setAlignment(kCCTextAlignmentCenter);
    input->setDelegate(this);
    input->useCheckOnClick = false;
    menu->addChild(input);

    colour.fromJson(Mod::get()->getSavedValue<matjson::Value>("paintcontrol_colour", colour.toJson()));

    updateOrientation();
    toggleExpanded(false);
    setupChildren();
    setTool(PaintTool::Brush);
    return true;
}

void PaintControl::setupChildren()
{
    if (!menu || !bg)
        return;

    this->addChild(bg, -7);
    this->addChild(menu, -6);

    colourSpr->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName("GJ_colorBtn_001.png"));
    
    for (auto tool : toolSprs)
    {
        tool.second.sprite->setTexture(CCTextureCache::get()->addImage(tool.second.sprite->getID().c_str(), false));
    }
}

void PaintControl::update(float dt)
{
    FloatingUIButton::update(dt);

    if (colourSpr)
        colourSpr->setColor(getColour());

    if (overlaySpr)
        overlaySpr->setColor(ColourUtils::get()->getPastel("paint-icon"));
}

cocos2d::ccColor3B PaintControl::getColour()
{
    return colour.colourForConfig("paint");
}

void PaintControl::updateOrientation(bool horizontal)
{
    if (!bg || !menu)
        return;

    float orientation = 0;

    if (PaintVerticalControl::get()->getRealEnabled())
        orientation = 90;

    /*if (PaintVerticalControl::get()->getRealEnabled())
    {
        orientation = 90;

        if (position.y - bg->getContentWidth() < 0)
            orientation = -90;
    }
    else
    {
        if (position.x + bg->getContentWidth() > CCDirector::get()->getWinSize().width)
            orientation = -180;
    }*/

    bg->setRotation(orientation);
    menu->setRotation(orientation);

    for (auto child : menu->getChildrenExt())
    {
        child->setRotation(-orientation);
    }
}

void PaintControl::toggleExpanded(bool value)
{
    isExpanded = value;

    menu->stopAllActions();
    bg->stopAllActions();

    if (value)
    {
        menu->runAction(RealtimeAction::create(CCEaseBackOut::create(CCScaleTo::create(0.2f, value ? 1 : 0, 1))));
        bg->runAction(RealtimeAction::create(CCEaseBackOut::create(CCScaleTo::create(0.2f, value ? 1 : 0, 1))));
    }
    else
    {
        menu->runAction(RealtimeAction::create(CCEaseInOut::create(CCScaleTo::create(0.2f, value ? 1 : 0, 1), 2)));
        bg->runAction(RealtimeAction::create(CCEaseInOut::create(CCScaleTo::create(0.2f, value ? 1 : 0, 1), 2)));
    }
}

void PaintControl::onChangeColour(CCObject* sender)
{
    ui = SetupColourConfigUI::create([this](ColourConfig conf)
    {
        this->colour = conf;
        this->ui = nullptr;
        Mod::get()->setSavedValue<matjson::Value>("paintcontrol_colour", colour.toJson());
    });
    ui->setStartConfig(colour);
    ui->show();
    ui->shouldVisit = false;
}

void PaintControl::onChangeTool(CCObject* sender)
{
    auto tool = (PaintTool)sender->getTag();

    if (tool == PaintTool::Clear)
        PaintNode::get()->init();
    else
        setTool(tool);

    utils::addCircleToNode(static_cast<CCNode*>(sender), 0.75f);
}

void PaintControl::addToolButton(PaintTool tool)
{
    PaintToolData data;
    data.tool = tool;

    std::string str = "";

    switch (tool)
    {
        case PaintTool::Brush:
            str = "paint-brush-tool.png"_spr;
            break;

        case PaintTool::Rubber:
            str = "paint-rubber-tool.png"_spr;
            break;

        case PaintTool::Clear:
            str = "paint-clear-tool.png"_spr;
            break;
    }

    data.sprite = CCSprite::create(str.c_str());
    data.sprite->setID(str);
    data.button = Button::create(data.sprite, this, menu_selector(PaintControl::onChangeTool));
    data.button->setTag((int)tool);
    data.button->setPosition(ccp(30 + 25 * (int)tool, 0));

    setToolSize(tool, Mod::get()->getSavedValue<int>(fmt::format("paintcontrol_{}_size", (int)tool), 5));

    menu->addChild(data.button);
    toolSprs[tool] = data;
}

void PaintControl::setTool(PaintTool tool)
{
    this->selectedTool = tool;

    for (auto tool : toolSprs)
    {
        tool.second.button->setEnabled(tool.first != selectedTool);
        tool.second.sprite->setOpacity(tool.first == selectedTool ? 255 : 150);
    }

    input->setString(fmt::format("{}", getSelectedSize()));
}

PaintTool PaintControl::getTool()
{
    return selectedTool;
}

void PaintControl::setToolSize(PaintTool tool, int size)
{
    toolSizes[tool] = size;

    Mod::get()->setSavedValue<int>(fmt::format("paintcontrol_{}_size", (int)tool), size);
}

int PaintControl::getSelectedSize()
{
    if (!toolSizes.contains(selectedTool))
        return 5;

    return toolSizes[selectedTool];
}

bool PaintControl::ccTouchBegan(qolmod::Touch* touch)
{
    if (ui)
        return false;

    if (!isActive())
        return false;

    auto ret = FloatingUIButton::ccTouchBegan(touch);
    isMenuSelected = 0;

    if (ret)
        return true;

    if (isExpanded)
    {        
        if (menu->ccTouchBegan(touch->fakeTouch, nullptr))
        {
            isMenuSelected = 1;
            return true;
        }

        if (input->ccTouchBegan(touch->fakeTouch, nullptr))
        {
            isMenuSelected = 2;
            return true;
        }

        isMenuSelected = 3;
        qolmod::PaintNode::get()->ccTouchBegan(touch->fakeTouch);
        return true;
    }

    return false;
}

void PaintControl::ccTouchMoved(qolmod::Touch* touch)
{
    if (isMenuSelected == 1)
        return menu->ccTouchMoved(touch->fakeTouch, nullptr);

    if (isMenuSelected == 2)
        return input->ccTouchMoved(touch->fakeTouch, nullptr);

    if (isMenuSelected == 3)
        return qolmod::PaintNode::get()->ccTouchMoved(touch->fakeTouch);

    FloatingUIButton::ccTouchMoved(touch);
}

void PaintControl::ccTouchEnded(qolmod::Touch* touch)
{
    if (isMenuSelected == 1)
        return menu->ccTouchEnded(touch->fakeTouch, nullptr);

    if (isMenuSelected == 2)
        return input->ccTouchEnded(touch->fakeTouch, nullptr);

    if (isMenuSelected == 3)
        return qolmod::PaintNode::get()->ccTouchEnded(touch->fakeTouch);

    FloatingUIButton::ccTouchEnded(touch);
}

void PaintControl::updatePosition(cocos2d::CCPoint point)
{
    FloatingUIButton::updatePosition(point);

    Mod::get()->setSavedValue<float>("paintcontrol_position.x", position.x);
    Mod::get()->setSavedValue<float>("paintcontrol_position.y", position.y);

    // updateOrientation();
}

void PaintControl::textChanged(CCTextInputNode* node)
{
    auto size = geode::utils::numFromString<int>(input->getString()).unwrapOr(getSelectedSize());

    setToolSize(selectedTool, size);
}

void PaintControl::preVisit()
{
    if (!isActive())
        return;

    PaintNode::get()->visit();
}

void PaintControl::visit()
{
    if (!isActive())
        return;

    FloatingUIButton::visit();

    if (ui)
    {
        if (!ui->getParent())
            CCScene::get()->addChild(ui);

        ui->shouldVisit = true;
        ui->visit();
        ui->shouldVisit = false;
    }
}

bool PaintControl::getExpanded()
{
    return isExpanded;
}

bool PaintControl::isActive()
{
    return PaintEnabled::get()->getRealEnabled();
}

$on_game(Loaded)
{
    qolmod::PaintControl::get();
}