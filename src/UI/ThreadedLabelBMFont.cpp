#include "ThreadedLabelBMFont.hpp"

bool ThreadedLabelBMFont::init(std::string text, std::string font, MiniFunction<void(ThreadedLabelBMFont*)> callback)
{
    if (!CCNode::init())
        return false;

    this->text = text;
    this->font = font;
    this->callback = callback;

    if (CCTextureCache::get()->textureForKey(utils::string::replace(font, ".fnt", ".png").c_str()))
    {
        addLabel();
    }
    else
    {
        labels.push_back(this);
        this->retain();

        if (labels.size() - 1 == 0)
        {
            Loader::get()->queueInMainThread([this]
            {
                queueStep();
            });
        }
    }

    return true;
}

void ThreadedLabelBMFont::queueStep()
{
    if (labels.size() == 0)
        return;

    if (auto lbl = as<Ref<ThreadedLabelBMFont>>(labels[0]))
    {
        auto conf = FNTConfigLoadFile(lbl->font.c_str());

        CCTextureCache::get()->addImage(conf->getAtlasName(), false);

        auto fnt = lbl->font;

        for (auto lbl : labels)
        {
            if (lbl->font == fnt)
            {
                lbl->addLabel();
                lbl->release();
            }
        }

        labels.erase(std::remove_if(labels.begin(), labels.end(), [fnt](ThreadedLabelBMFont* obj)
        {
            return obj->getFont() == fnt;
        }));

        if (labels.size() != 0)
        {
            Loader::get()->queueInMainThread([this]
            {
                queueStep();
            });
        }
    }
}

void ThreadedLabelBMFont::addLabel()
{
    label = CCLabelBMFont::create(text.c_str(), font.c_str());
    this->addChild(label);

    if (callback)
        callback(this);

    this->setContentSize(label->getScaledContentSize());
    label->setAnchorPoint(ccp(0, 0));
}

CCLabelBMFont* ThreadedLabelBMFont::getLabel()
{
    return label;
}

std::string ThreadedLabelBMFont::getFont()
{
    return font;
}

ThreadedLabelBMFont* ThreadedLabelBMFont::create(std::string text, std::string font, MiniFunction<void(ThreadedLabelBMFont*)> callback)
{
    auto pRet = new ThreadedLabelBMFont();

    if (pRet && pRet->init(text, font, callback))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}