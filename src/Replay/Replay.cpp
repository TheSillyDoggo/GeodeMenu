#include "Macro.h"

using namespace gdr;

struct MyInput : Input {
	MyInput() = default;

	MyInput(int frame, int button, bool player2, bool down, float xpos)
		: Input(frame, button, player2, down) {}
};

struct MyReplay : Replay<MyReplay, MyInput> {
	int attempts = -1;

	MyReplay() : Replay("QOLMod Bot", "1.0") {}
};

/*class $modify (PlayerObject)
{
    virtual void update(float p0)
    {
        PlayerObject::update(p0);

        //log::info("i hope this is tick instead of cocos update");

        /*MyReplay r;

		r.author = "camila314";
		r.description = "we testing up in here";
		r.attempts = 50;

		r.inputs.push_back(MyInput(100, 1, false, true, 30.23));
		r.inputs.push_back(MyInput(130, 1, false, false, 100.35));

		std::vector<uint8_t> output = r.exportData(true);

		std::string str(output.begin(), output.end());

		log::info("replay data: {}", str);
    }
};*/

class $modify (PauseLayer)
{
    virtual TodoReturn customSetup()
    {
        PauseLayer::customSetup();

		return;

		auto s = CCSprite::create("replay.png"_spr);
		s->setScale(CCDirector::get()->getContentScaleFactor() * 0.5f);

		auto btn = CCMenuItemSpriteExtra::create(s, this, nullptr);

        auto m = CCMenu::create();
		m->setContentSize(ccp(0, 0));
		m->setScale(0.25f);
		m->addChild(btn);
        this->addChild(m);
	}
};