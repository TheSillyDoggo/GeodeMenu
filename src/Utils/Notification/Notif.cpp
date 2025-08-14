#include "Notif.hpp"

NotifLayer* NotifLayer::createAndNotify(Notif notif)
{
    auto pRet = create(notif);
    pRet->notify();
    return pRet;
}

NotifLayer* NotifLayer::create(Notif notif)
{
    auto pRet = new NotifLayer();

    if (pRet && pRet->init(notif))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void NotifLayer::notify()
{
    // https://www.youtube.com/watch?v=cwVoiduekyA
    if (notif.playSound)
        FMODAudioEngine::get()->playEffect("notification.mp3"_spr);

    if (notif.duration != -1)
        this->scheduleOnce(schedule_selector(NotifLayer::onTimeUp), notif.duration);

    CCScene::get()->addChild(this);
}

void NotifLayer::onTimeUp(float dt)
{
    if (queue.size() > 0)
    {
        std::vector<Notif> queue2 = {};

        auto noti = NotifLayer::createAndNotify(queue[0]);

        for (size_t i = 1; i < queue.size(); i++)
        {
            noti->addToQueue(queue[i]);
        }
    }

    this->removeFromParentAndCleanup(true);
}

void NotifLayer::setOnFinish(std::function<void()> func)
{
    this->onFinish = func;
}

void NotifLayer::addToQueue(Notif notif)
{
    queue.push_back(notif);
}

bool NotifLayer::init(Notif notif)
{
    if (!CCLayer::init())
        return false;

    instance = this;

#define NOTIF_INSET 8
#define LABEL_INSET 3

    this->notif = notif;
    this->setAnchorPoint(ccp(0, 0));
    // fix for qolmod menu appearing after this is run
    Loader::get()->queueInMainThread([this]
    {
        this->setZOrder(CCScene::get()->getHighestChildZ() + 1);
    });

    bg = CCLayerColor::create();
    bg->setColor(ccc3(0, 0, 0));
    bg->setOpacity(150);
    bg->ignoreAnchorPointForPosition(false);
    bg->setAnchorPoint(ccp(0, 1));

    label = CCLabelBMFont::create(notif.text.c_str(), "bigFont.fnt");
    label->setAnchorPoint(ccp(0, 1));
    label->setScale(0.3f);

    bg->setContentWidth(label->getScaledContentWidth() + LABEL_INSET * 2);
    bg->setContentHeight(label->getScaledContentHeight() + LABEL_INSET * 2);

    this->addChildAtPosition(bg, Anchor::TopLeft, ccp(NOTIF_INSET, -NOTIF_INSET));
    this->addChildAtPosition(label, Anchor::TopLeft, ccp(NOTIF_INSET + LABEL_INSET, -NOTIF_INSET + -LABEL_INSET));
    return true;
}

NotifLayer::~NotifLayer()
{
    if (instance == this)
        instance = nullptr;
}

void NotifLayer::closeCurrent()
{
    if (instance)
        instance->removeFromParent();
}

// Help notif

#include "../../GUI/AndroidUI.hpp"

class UIOpenFilter : public EventFilter<QOLModUIOpenEvent>
{
    public:
        UIOpenFilter(CCNode* target)
        {

        }
};

class $modify (MenuLayer)
{
    bool init()
    {
        if (!MenuLayer::init())
            return false;

        if (std::filesystem::exists(Mod::get()->getSaveDir() / "saved.json"))
            return true;

        // TODO: write help

        static bool hasShownNotif = false;

        if (!hasShownNotif)
        {
            hasShownNotif = true;

            Loader::get()->queueInMainThread([this]
            {
                #ifdef GEODE_IS_MOBILE
                auto noti = Notif({"Press the (>_) button on the screen\nto open the QOLMod menu.", true, -1});
                #else
                auto noti = Notif({"Press the [TAB] button on the keyboard\nto open the QOLMod menu.", true, -1});
                #endif

                auto notif = NotifLayer::createAndNotify(noti);

                notif->addEventListener<UIOpenFilter>([notif](QOLModUIOpenEvent* ev)
                {
                    notif->onTimeUp(0);

                    return ListenerResult::Propagate;
                });



                GameManager::get()->addEventListener<UIOpenFilter>([this](QOLModUIOpenEvent* ev)
                {
                    static bool hasShownNotif2 = false;

                    if (!hasShownNotif2)
                    {
                        hasShownNotif2 = true;

                        auto notif = NotifLayer::create({"Mods can be toggled with the\ncheckbox (  ) next to the name."});
                        notif->addToQueue({"Some mods may have additional settings,\nthese can be found with the plus (  ) button."});
                        notif->notify();
                    }

                    return ListenerResult::Propagate;
                });
            });
        }

        return true;
    }
};