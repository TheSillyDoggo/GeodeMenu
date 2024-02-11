/*
class $modify (PlayerObject)
{
    virtual void update(float p0)
    {
		if (PlayLayer::get() || LevelEditorLayer::get())
		{
			GJBaseGameLayer* l = base_cast<GJBaseGameLayer*>(PlayLayer::get());

			if (!l)
				l = base_cast<GJBaseGameLayer*>(LevelEditorLayer::get());

			if (!l)
				return;

			for (size_t i = 0; i < replay.inputs.size(); i++) // todo: make better
			{
				//log::info("penis: {}", l->m_gameState.m_unk1f8);

				if (replay.inputs[i].frame == l->m_gameState.m_unk1f8 && !replay.inputs[i].h)
				{
					log::info("action: {}", replay.inputs[i].button);
					replay.inputs[i].h = true;

					l->handleButton(replay.inputs[i].down, replay.inputs[i].button, replay.inputs[i].player2);

					//replay.inputs.erase(replay.inputs.begin() + i);

					break;
				}
			}
			
			//log::info("frames: {}", l->m_gameState.m_unk1f8);
		}

        PlayerObject::update(p0);
    }
};

class $modify (GJBaseGameLayer)
{
	TodoReturn handleButton(bool p0, int p1, bool p2)
	{
		GJBaseGameLayer::handleButton(p0, p1, p2);

		//replay.inputs.push_back(MyInput(this->m_gameState.m_unk1f8, p1, !p2, !p0, ((p2 ? m_player2 : m_player1)->m_position.x)));
	}
};*/

/*

struct MyInput : Input {
	bool h = false;
	float xpos;

	MyInput() = default;

	MyInput(int frame, int button, bool player2, bool down, float xpos)
		: Input(frame, button, player2, down) {}

};

struct MyReplay : Replay<MyReplay, MyInput> {
	int attempts = -1;

	MyReplay() : Replay("QOLMod Bot", "1.0") {}
};

MyReplay replay;

bool asdf;
std::vector<MyInput> actions;

class $modify (ReplayPauseLayer, PauseLayer)
{
	void asd(CCObject*)
	{
		asdf = !asdf;

		if (!asdf)
			actions.clear();
	}

	void onS(CCObject*)
	{
		replay.author = "camila314";
		replay.description = "we testing up in here";
		replay.attempts = 50;

		//replay.inputs.push_back(MyInput(100, (int)PlayerButton::Right, false, true, 30.23));
		//replay.inputs.push_back(MyInput(2400, (int)PlayerButton::Right, false, false, 100.35));

		std::vector<uint8_t> output = replay.exportData(true);
		std::string str(output.begin(), output.end());

		utils::file::writeString(Mod::get()->getConfigDir() / "macros" / "Replay.gdr", /*outputstr.c_str());

		//

		//log::info("replay data: {}", str);
	}

	void onL(CCObject*)
	{
		auto v = utils::file::readString(Mod::get()->getConfigDir() / "macros" / "Replay.gdr").value();
		log::info("value: {}", v);

		//ReplayUtils::loadFromJson(&replay, v);

		replay = MyReplay::importDataS(v);

		log::info("a: {}", replay.inputs.size());

		//std::vector<uint8_t> output = replay.exportData(true);

		//std::string str(output.begin(), output.end());

		//log::info("replay data: {}", str);
	}

    virtual TodoReturn customSetup()
    {
        PauseLayer::customSetup();

		//return;

		auto s = CCSprite::create("replay.png"_spr);
		s->setScale(CCDirector::get()->getContentScaleFactor() * 0.5f);

		auto btn = CCMenuItemSpriteExtra::create(s, this, menu_selector(ReplayPauseLayer::asd ));

        auto m = CCMenu::create();
		m->setContentSize(ccp(0, 0));
		m->setScale(0.25f);
		m->addChild(btn);
        this->addChild(m);
	}
};

int actionsIndex = 0;
float dta;

class $modify(PlayLayer) {

    void postUpdate(float dt) { //until GJBaseGameLayer::update
        PlayLayer::postUpdate(dt);

        dta += dt;

        if (asdf) {
            if (actions[actionsIndex].frame <= (dta * 240.f)) {
                handleButton(actions[actionsIndex].down, actions[actionsIndex].button, actions[actionsIndex].player2);
                actionsIndex++;
            }
        }
    }

    void resetLevel() {
		dta = 0;
        PlayLayer::resetLevel();

        actionsIndex = 0;
    }
};

class $modify(GJBaseGameLayer) {
    void handleButton(bool push, int button, bool player1) {
        GJBaseGameLayer::handleButton(push, button, player1);

        if (!asdf) actions.push_back(MyInput(m_gameState.m_unk1f8, button, !player1, push, 0));
    }
};*/