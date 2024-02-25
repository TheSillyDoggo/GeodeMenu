#pragma once

#include <Geode/Geode.hpp>

#include <Geode/modify/CheckpointObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

#include <iostream>
#include <vector>
#include <optional>
#include "../Replay/json.hpp"

using namespace nlohmann;

namespace gdr {

    struct Bot {
        std::string name;
        std::string version;

        inline Bot(std::string const& name, std::string const& version)
            : name(name), version(version) {}
    };

    struct Level {
        uint32_t id;
        std::string name;

        Level() = default;

        inline Level(std::string const& name, uint32_t id = 0)
            : name(name), id(id) {}
    };

    class Input {
    protected:
        Input() = default;
        template <typename, typename>
        friend class Replay;
    public:
        uint32_t frame;
        int button;
        bool player2;
        bool down;

        inline virtual void parseExtension(json::object_t obj) {}
        inline virtual json::object_t saveExtension() const {
            return {};
        }

        inline Input(int frame, int button, bool player2, bool down)
            : frame(frame), button(button), player2(player2), down(down) {}


        inline static Input hold(int frame, int button, bool player2 = false) {
            return Input(frame, button, player2, true);
        }

        inline static Input release(int frame, int button, bool player2 = false) {
            return Input(frame, button, player2, false);
        }

        inline bool operator<(Input const& other) const {
            return frame < other.frame;
        }
    };

    template <typename S = void, typename T = Input>
    class Replay {
        Replay() = default;
    public:
        using InputType = T;
        using Self = std::conditional_t<std::is_same_v<S, void>, Replay<S, T>, S>;

        std::string author;
        std::string description;

        float duration;
        float gameVersion;
        float version = 1.0;

        int seed = 0;
        int coins = 0;

        bool ldm = false;

        Bot botInfo;
        Level levelInfo;

        std::vector<InputType> inputs;

        virtual void parseExtension(json::object_t obj) {}
        virtual json::object_t saveExtension() const {
            return {};
        }

        Replay(std::string const& botName, std::string const& botVersion)
            : botInfo(botName, botVersion) {}

        static Self importData(std::vector<uint8_t> const& data) {
            Self replay;
            json replayJson;

            try {
                replayJson = json::from_msgpack(data);
            } catch(std::exception& e) {
                replayJson = json::parse(data);
            }

            replay.gameVersion = replayJson["gameVersion"];
            replay.description = replayJson["description"];
            replay.version = replayJson["version"];
            replay.duration = replayJson["duration"];
            replay.botInfo.name = replayJson["bot"]["name"];
            replay.botInfo.version = replayJson["bot"]["version"];
            replay.levelInfo.id = replayJson["level"]["id"];
            replay.levelInfo.name = replayJson["level"]["name"];
            replay.author = replayJson["author"];
            replay.seed = replayJson["seed"];
            replay.coins = replayJson["coins"];
            replay.ldm = replayJson["ldm"];
            replay.parseExtension(replayJson.get<json::object_t>());

            for (json const& inputJson : replayJson["inputs"]) {
                InputType input;
                input.frame = inputJson["frame"];
                input.button = inputJson["btn"];
                input.player2 = inputJson["2p"];
                input.down = inputJson["down"];
                input.parseExtension(inputJson.get<json::object_t>());

                replay.inputs.push_back(input);
            }

            return replay;
        }

        static Self importDataS(std::string const& data) {
            Self replay;
            json replayJson;

            replayJson = json::parse(data);

            replay.gameVersion = replayJson["gameVersion"];
            replay.description = replayJson["description"];
            replay.version = replayJson["version"];
            replay.duration = replayJson["duration"];
            replay.botInfo.name = replayJson["bot"]["name"];
            replay.botInfo.version = replayJson["bot"]["version"];
            replay.levelInfo.id = replayJson["level"]["id"];
            replay.levelInfo.name = replayJson["level"]["name"];
            replay.author = replayJson["author"];
            replay.seed = replayJson["seed"];
            replay.coins = replayJson["coins"];
            replay.ldm = replayJson["ldm"];
            replay.parseExtension(replayJson.get<json::object_t>());

            for (json const& inputJson : replayJson["inputs"]) {
                InputType input;
                input.frame = inputJson["frame"];
                input.button = inputJson["btn"];
                input.player2 = inputJson["2p"];
                input.down = inputJson["down"];
                input.parseExtension(inputJson.get<json::object_t>());

                replay.inputs.push_back(input);
            }

            return replay;
        }

        std::vector<uint8_t> exportData(bool exportJson = false) {
            json replayJson = saveExtension();
            replayJson["gameVersion"] = gameVersion;
            replayJson["description"] = description;
            replayJson["version"] = version;
            replayJson["duration"] = inputs[inputs.size() - 1].frame;
            replayJson["bot"]["name"] = botInfo.name;
            replayJson["bot"]["version"] = botInfo.version;
            replayJson["level"]["id"] = levelInfo.id;
            replayJson["level"]["name"] = levelInfo.name;
            replayJson["author"] = author;
            replayJson["seed"] = seed;
            replayJson["coins"] = coins;
            replayJson["ldm"] = ldm;

            for (InputType const& input : inputs) {
                json inputJson = input.saveExtension();
                inputJson["frame"] = input.frame;
                inputJson["btn"] = input.button;
                inputJson["2p"] = input.player2;
                inputJson["down"] = input.down;

                replayJson["inputs"].push_back(inputJson);
            }

            if (exportJson) {
                std::string replayString = replayJson.dump();
                return std::vector<uint8_t>(replayString.begin(), replayString.end());
            } else {
                return json::to_msgpack(replayJson);
            }
        }
    };

} // namespace gdr


struct MyInput : gdr::Input {
	bool h = false;
	float xpos;
	float ypos;
	float dt;
	float xvelocity;
	float yvelocity;
	float rotation;

	MyInput() = default;

	MyInput(int frame, int button, bool player2, bool down, float xpos, float ypos, float dt, float xvelocity, float yvelocity, float rotation)
		: Input(frame, button, player2, down) {
            this->xpos = xpos;
            this->ypos = ypos;
            this->dt = dt;
            this->xvelocity = xvelocity;
            this->yvelocity = yvelocity;
            this->rotation = rotation;
        }

    void parseExtension(json::object_t obj) override {
		xpos = obj["xpos"];
		ypos = obj["ypos"];
		dt = obj["dt"];
		xvelocity = obj["xvelocity"];
        yvelocity = obj["yvelocity"];
        rotation = obj["rotation"];
	}

	json::object_t saveExtension() const override {
		return { {"xpos", xpos}, {"ypos", ypos}, {"dt", dt}, {"xvelocity", xvelocity}, {"yvelocity", yvelocity}, {"rotation", rotation} };
	}

};

struct MyReplay : gdr::Replay<MyReplay, MyInput> {
	int attempts = -1;

	MyReplay() : Replay("QOLBot", "1.0") {}
};

class GJReplayManager // roberta topertla momento
{
    public:
        static inline bool playing = false;
        static inline bool recording = false;
        static inline float dt;
        static inline int frame;
        static inline MyReplay replay = MyReplay();
};