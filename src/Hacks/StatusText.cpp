#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../Client/Client.h"

using namespace geode::prelude;

class StatusNode : public CCNode
{
    public:
        static inline PlayLayer* pl = nullptr;

        static StatusNode* create() {
            auto ret = new (std::nothrow) StatusNode;
            if (ret && ret->init()) {
                ret->autorelease();
                return ret;
            }
            delete ret;
            return nullptr;
        }

        bool testmode = false;

        CCLabelBMFont* tl;
        CCLabelBMFont* tr;

        bool init()
        {
            this->setID("status-node");
            this->scheduleUpdate();

            return true;
        }

        void update(float dt)
        {
            float op = 0.9f, scale = 1.0f;
            
            try
            {
                op = std::stof(StatusOpacity::instance->text);
            }
            catch(const std::exception& e)
            {
                op = 0.9f;
            }

            try
            {
                scale = std::stof(StatusScale::instance->text);
            }
            catch(const std::exception& e)
            {
                scale = 1.0f;
            }

            op = clamp<float>(op, 0.0f, 1.0f);

            tl->setOpacity((int)round(255 * op));
            tl->setScale(0.5f * scale);
            tl->setString("");

            tr->setOpacity((int)round(255 * op));
            tr->setScale(0.5f * scale);
            tr->setString("");

            if (Client::GetModuleEnabled("status-testmode") && testmode)
                WriteText("Testmode", "");

            if (Client::GetModuleEnabled("status-fps"))
                WriteText("FPS: %", std::to_string((int)round(1.0f / dt)));

            //if (Client::GetModuleEnabled("noclip"))
            if (false)
            {
                if (Client::GetModuleEnabled("status-death"))
                    WriteText("Deaths: %", std::to_string((int)1));
            }

            //log::info("attempt {}", );
            //if (Client::GetModuleEnabled("status-attempts"))
                //WriteText("%", attempt->getString());
        }

        void WriteText(std::string text, std::string f)
        {
            std::stringstream s;
            
            for (size_t i = 0; i < text.size(); i++)
            {
                if (text[i] == '%')
                {
                    s << f;
                }
                else
                {
                    s << text[i];
                }
            }

            std::stringstream ss;

            ss << tl->getString();
            ss << s.str();
            ss << "\n";

            tl->setString(ss.str().c_str());
        }
};

class $modify (PlayLayer)
{
    bool init(GJGameLevel* p0, bool p1, bool p2)
    {
        if (!PlayLayer::init(p0, p1, p2))
            return false;


        StatusNode::pl = this;
        auto stn = StatusNode::create();

        if (this->getChildrenCount() > 10)
        {
            reinterpret_cast<CCNode*>(getChildren()->objectAtIndex(10))->setVisible(false);
            stn->testmode = true;
        }

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