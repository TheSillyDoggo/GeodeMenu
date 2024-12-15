#include "TransAlertLayer.hpp"
#include "TransLabelBMFont.hpp"
#include "../Utils/TranslationManager.hpp"

bool TransAlertLayer::init(char const* title, const gd::string& desc, char const* btn)
{
    std::string desc2 = TranslationManager::get()->getTranslatedString(desc);

    if (!FLAlertLayer::init(nullptr, title, desc2, btn, nullptr, 300.0f, false, 320, 1.0f))
        return false;

    auto label = CCLabelBMFont::create("", "bigFont.fnt");

    bool useTtf = false;

    for (auto letter : desc2)
    {
        if (!label->getConfiguration()->getCharacterSet()->contains(as<int>(letter)))
        {
            useTtf = true;
            break;
        }
    }

    auto titleL = TransLabelBMFont::create(std::string(title), "goldFont.fnt");
    titleL->setAnchorPoint(ccp(0.5f, 1));
    titleL->setPosition(m_mainLayer->getChildByType<CCLabelBMFont>(0)->getPosition());
    titleL->setScale(m_mainLayer->getChildByType<CCLabelBMFont>(0)->getScale());

    m_mainLayer->getChildByType<CCLabelBMFont>(0)->setVisible(false);

    m_mainLayer->addChild(titleL, 420);

    if (useTtf)
    {
        m_mainLayer->getChildByType<TextArea>(0)->setVisible(false);

        auto node = CCNode::create();
        node->setPosition(CCDirector::get()->getWinSize() / 2);
        node->setAnchorPoint(ccp(0.5f, 0.5f));
        node->setContentWidth(260);

        for (auto lb : CCArrayExt<CCLabelBMFont*>(m_mainLayer->getChildByType<TextArea>(0)->getChildByType<MultilineBitmapFont>(0)->getChildren()))
        {
            auto lbl = TransLabelBMFont::create(fmt::format("{} ", lb->getString()), "chatFont.fnt");
            lbl->setColor(lb->getColor());

            node->addChild(lbl);
            log::info("str: {}", lb->getString());
        }

        node->setLayout(AxisLayout::create()->setAutoScale(false)->setGrowCrossAxis(true)->setGap(0));

        m_mainLayer->addChild(node, 69);
    }

    return true;
}

TransAlertLayer* TransAlertLayer::create(char const* title, const gd::string& desc, char const* btn)
{
    auto pRet = new TransAlertLayer();

    if (pRet && pRet->init(title, desc, btn))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}