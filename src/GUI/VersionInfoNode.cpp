#include "VersionInfoNode.hpp"
#include "../Localisation/LocalisationManager.hpp"

VersionInfoNode* VersionInfoNode::create(VersionInfoType type)
{
    auto pRet = new VersionInfoNode();

    if (pRet && pRet->init(type))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool VersionInfoNode::init(VersionInfoType type)
{
    if (!CCNodeRGBA::init())
        return false;

    this->type = type;

    if (type == VersionInfoType::QOLMod)
        checkForUpdate();

    label = AdvLabelBMFont::createWithString("", "lexend.fnt"_spr);
    label->setAnchorPoint(ccp(0, 0));
    circle = LoadingCircleSprite::create(1);
    circle->setScale(0.22f);

    this->addChild(circle);
    this->addChild(label);
    updateLabel();
    return true;
}

void VersionInfoNode::updateLabel()
{
    switch (type)
    {
        case VersionInfoType::GD:
            label->setString(fmt::format("GD v{}", Loader::get()->getGameVersion()).c_str());
            break;

        case VersionInfoType::Geode:
            label->setString(fmt::format("Geode {}", Loader::get()->getVersion().toVString()).c_str());
            break;

        case VersionInfoType::GeodeAndGD:
            label->setString(fmt::format("Geode {}  |  GD v{}", Loader::get()->getVersion().toVString(), Loader::get()->getGameVersion()).c_str());
            break;

        case VersionInfoType::QOLMod:
            if (isUpdateAvailable())
            {
                label->setString(fmt::format("{}  |  QOLMod {}", LocalisationManager::get()->getLocalisedString("ui/update-available"), Mod::get()->getVersion().toVString()).c_str());
            }
            else
            {
                label->setString(fmt::format("QOLMod {}", Mod::get()->getVersion().toVString()).c_str());
            }

            break;
    }

    circle->setVisible(isChecking() && type == VersionInfoType::QOLMod);
    this->setContentSize(label->getScaledContentSize());
    circle->setPosition(ccp(-17, getContentHeight() / 2 + -0.3f));
}

bool VersionInfoNode::isUpdateAvailable()
{
    return updateAvailable;
}

bool VersionInfoNode::isChecking()
{
    return !updateChecksFinished;
}

void VersionInfoNode::checkForUpdate()
{
    static bool hasCheckedForUpdates = false;

    if (hasCheckedForUpdates)
    {
        updateChecksFinished = true;
        return;
    }

    updateListener.bind([this](Mod::CheckUpdatesTask::Event* event)
    {
        if (auto value = event->getValue())
        {
            if (value->isOk())
            {
                this->updateAvailable = value->unwrap().has_value();
            }
            
            updateChecksFinished = true;
            hasCheckedForUpdates = true;
            updateLabel();
        }
        else if (event->isCancelled())
        {
            updateChecksFinished = false;
            updateLabel();
        }
        else
        {
            hasCheckedForUpdates = true;
            updateChecksFinished = true;
            updateLabel();
        }
    });

    updateListener.setFilter(Mod::get()->checkUpdates());
}

void VersionInfoNode::setColor(const ccColor3B& color)
{
    CCNodeRGBA::setColor(color);
    circle->setColor(color);
    label->setColor(color);
}

void VersionInfoNode::setOpacity(GLubyte opacity)
{
    CCNodeRGBA::setOpacity(opacity);
    circle->setOpacity(opacity);
    label->setOpacity(opacity);
}