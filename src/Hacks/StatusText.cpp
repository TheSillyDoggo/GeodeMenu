#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

#include "Noclip.cpp"

using namespace geode::prelude;

std::string floatToString(float num) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << num;
    return ss.str();
}

bool isStringConvertibleToFloat(const std::string& str) {
    std::istringstream iss(str);
    float f;
    iss >> std::noskipws >> f; // Disable skipping whitespaces
    return iss.eof() && !iss.fail();
}

float safeStringToFloat(const std::string& floatString) {
    if (isStringConvertibleToFloat(floatString)) {
        return std::stof(floatString);
    } else {
        return 0.9f;
    }
}

class StatusNode : public CCNode
{
    public:
        static StatusNode* create() {
            auto ret = new (std::nothrow) StatusNode;
            if (ret && ret->init()) {
                ret->autorelease();
                return ret;
            }
            delete ret;
            return nullptr;
        }

        CCLabelBMFont* tl;
        CCLabelBMFont* tr;

        bool init()
        {
            this->setID("status-node");
            this->scheduleUpdate();

            return true;
        }

        void updateVis()
        {
            float op = 0.9f, scale = 1.0f;

            op = safeStringToFloat(StatusOpacity::instance->text);
            scale = safeStringToFloat(StatusScale::instance->text);

            op = clamp<float>(op, 0.0f, 1.0f);

            tl->setOpacity((int)round(255 * op));
            tl->setScale(0.5f * scale);
            tl->setString("");

            tr->setOpacity((int)round(255 * op));
            tr->setScale(0.5f * scale);
            tr->setString("");
        }

        void update(float dt)
        {
            updateVis();

            if (Client::GetModuleEnabled("status-testmode") && PlayLayer::get()->m_isTestMode)
                WriteText("Testmode", "", Mod::get()->getSavedValue<int>("testmode_side", 0));

            //if (Client::GetModuleEnabled("status-attempt"))
                //WriteText("Attempt %", std::to_string(PlayLayer::get()->));

            if (Client::GetModuleEnabled("status-fps"))
                WriteText("FPS: %", std::to_string((int)round(1.0f / dt)), Mod::get()->getSavedValue<int>("fps_side", 0));

            if (Client::GetModuleEnabled("noclip"))
            {
                auto v = as<NoclipLayer*>(PlayLayer::get());

                float acc = (((1 - ((v->m_fields->t * 1.0f) / (v->m_gameState.m_unk1f8 * 1.0f))) * 100.0f));

                if (Client::GetModuleEnabled("status-accuracy"))
                    WriteText("Accuracy: %%", floatToString(acc), Mod::get()->getSavedValue<int>("accuracy_side", 0));

                if (Client::GetModuleEnabled("status-death"))
                    WriteText("Deaths: %", std::to_string((int)v->m_fields->t), Mod::get()->getSavedValue<int>("death_side", 0));
            }
        }

        void WriteText(std::string text, std::string f, int side = 0)
        {
            std::stringstream s;

            bool a = false;
            
            for (size_t i = 0; i < text.size(); i++)
            {
                if (text[i] == '%' && !a)
                {
                    s << f;
                    a = true;
                }
                else
                {
                    s << text[i];
                }
            }

            std::stringstream ss;

            if (side == 0)
                ss << tl->getString();
            else// if (side == 1);
                ss << tr->getString();

            ss << s.str();
            ss << "\n";

            if (side == 0)
                tl->setString(ss.str().c_str());
            else// if (side == 1)
                tr->setString(ss.str().c_str());
        }
};

class $modify (PlayLayer)
{
    bool init(GJGameLevel* p0, bool p1, bool p2)
    {
        if (!PlayLayer::init(p0, p1, p2))
            return false;

        auto stn = StatusNode::create();

        auto menu = CCMenu::create();
        menu->setPosition(ccp(0, 0));
        menu->setContentSize(CCDirector::get()->getWinSize());
        menu->setAnchorPoint(ccp(0, 0));
        menu->ignoreAnchorPointForPosition(false);

        auto tl = CCLabelBMFont::create("TL", "bigFont.fnt");
        tl->setAnchorPoint(ccp(0, 1));
        tl->setPosition(ccp(2, CCDirector::get()->getWinSize().height));

        auto tr = CCLabelBMFont::create("TR", "bigFont.fnt");
        tr->setAnchorPoint(ccp(1, 1));
        tr->setAlignment(CCTextAlignment::kCCTextAlignmentRight);
        tr->setPosition(ccp(CCDirector::get()->getWinSize().width - 2, CCDirector::get()->getWinSize().height));

        menu->addChild(tl);
        menu->addChild(tr);

        stn->tl = tl;
        stn->tr = tr;

        menu->addChild(stn);
        this->addChild(menu, 69420);
        return true;
    }
};