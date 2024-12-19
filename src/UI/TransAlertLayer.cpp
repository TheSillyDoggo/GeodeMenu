#include "TransAlertLayer.hpp"
#include "TransLabelBMFont.hpp"
#include "../Utils/TranslationManager.hpp"
#include <regex>

bool TransAlertLayer::init(char const* title, const gd::string& desc, char const* btn)
{
    std::string desc2 = TranslationManager::get()->getTranslatedString(desc);

    auto label = CCLabelBMFont::create("l", "chatFont.fnt");

    bool useTtf = false;

    useTtf = !std::regex_match(desc2, std::regex("^[a-zA-Z0-9\\s\\W]*$"));

    if (useTtf)
    {
        desc2 = utils::string::replace(desc2, "</c>", "");
        desc2 = utils::string::replace(desc2, "<cr>", "");
        desc2 = utils::string::replace(desc2, "<cg>", "");
        desc2 = utils::string::replace(desc2, "<cl>", "");
        desc2 = utils::string::replace(desc2, "<cp>", "");
        desc2 = utils::string::replace(desc2, "<co>", "");
        desc2 = utils::string::replace(desc2, "<cy>", "");
        desc2 = utils::string::replace(desc2, "<cj>", "");
        desc2 = utils::string::replace(desc2, "<cf>", "");
        desc2 = utils::string::replace(desc2, "<cd>", "");
        desc2 = utils::string::replace(desc2, "<cs>", "");
        desc2 = utils::string::replace(desc2, "<ca>", "");
        desc2 = utils::string::replace(desc2, "<cc>", "");
        desc2 = utils::string::replace(desc2, "<cb>", "");

        auto node = CCNode::create();
        node->setPosition(CCDirector::get()->getWinSize() / 2 + ccp(0, 5));
        node->setAnchorPoint(ccp(0.5f, 0.5f));
        node->setContentWidth(260);

        for (auto line : utils::string::split(desc2, "\n"))
        {
            auto node2 = CCNode::create();
            node2->setPosition(CCDirector::get()->getWinSize() / 2);
            node2->setAnchorPoint(ccp(0.5f, 0.5f));
            node2->setContentWidth(260);

            for (auto word : utils::string::split(line, " "))
            {
                auto lbl = TransLabelBMFont::create(word, "chatFont.fnt");
                lbl->setForceTTF(true);

                node2->addChild(lbl);
            }

            node->addChild(node2);
            node2->setLayout(AxisLayout::create()->setAutoScale(false)->setGrowCrossAxis(true)->setGap(6));
        }

        node->setLayout(AxisLayout::create()->setAutoScale(false)->setGrowCrossAxis(true)->setGap(6));

        std::string ss;

        for (auto child : CCArrayExt<CCNode*>(node->getChildren()))
        {
            float lastY = -42069;

            for (auto child2 : CCArrayExt<CCNode*>(child->getChildren()))
            {
                if (child2->getPositionY() != lastY)
                {
                    lastY = child2->getPositionY();
                    ss += "\n";
                }
            }

            ss += "\n";
        }

        ss = ss.substr(0, ss.size() - 1);

        FLAlertLayer::init(nullptr, title, ss, btn, nullptr, 300.0f, false, 320, 1.0f);
        m_mainLayer->getChildByType<TextArea>(0)->setVisible(false);

        m_mainLayer->addChild(node, 69);
    }
    else
    {
        FLAlertLayer::init(nullptr, title, desc2, btn, nullptr, 300.0f, false, 320, 1.0f);
    }

    auto titleL = TransLabelBMFont::create(std::string(title), "goldFont.fnt");
    titleL->setAnchorPoint(ccp(0.5f, 1));
    titleL->setPosition(m_mainLayer->getChildByType<CCLabelBMFont>(0)->getPosition());
    titleL->limitLabelWidth(260, m_mainLayer->getChildByType<CCLabelBMFont>(0)->getScale(), 0);;

    m_mainLayer->getChildByType<CCLabelBMFont>(0)->setVisible(false);

    m_mainLayer->addChild(titleL, 420);

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