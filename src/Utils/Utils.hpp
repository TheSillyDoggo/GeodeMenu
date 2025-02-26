#define MEMBERBYOFFSET(type, class, offset) *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(class) + offset)
#define MBO MEMBERBYOFFSET

#define public_cast(value, member) [](auto* v) { \
	class FriendClass__; \
	using T = std::remove_pointer<decltype(v)>::type; \
	class FriendeeClass__: public T { \
	protected: \
		friend FriendClass__; \
	}; \
	class FriendClass__ { \
	public: \
		auto& get(FriendeeClass__* v) { return v->member; } \
	} c; \
	return c.get(reinterpret_cast<FriendeeClass__*>(v)); \
}(value)

#define QOLMOD_MOD_HOOK(_modid, _hookname) \
static void onModify(auto& self) { \
    auto hook = self.getHook(_hookname); \
    Loader::get()->queueInMainThread([hook] { \
        auto modu = Client::GetModule(_modid); \
        modu->addHookRaw(hook); \
    }); \
}

#define QOLMOD_MOD_ALL_HOOKS(_modid) \
static void onModify(auto& self) { \
	std::vector<geode::Hook*> hooks; \
	if (self.m_hooks.empty()) \
		return; \
	for (auto hook : self.m_hooks) \
	{ \
		hooks.push_back(hook.second.get()); \
	} \
	Loader::get()->queueInMainThread([hooks] \
	{ \
		auto modu = Client::GetModule(_modid); \
		for (auto hook : hooks) \
		{ \
			if (hook) \
			{ \
				modu->addHook(hook); \
			} \
		} \
	}); \
}

#ifdef GEODE_IS_WINDOWS
    #ifdef QOLMOD_EXPORTING
        #define SILLY_DLL __declspec(dllexport)
    #else
        #define SILLY_DLL __declspec(dllimport)
    #endif
#else
    #define SILLY_DLL __attribute__((visibility("default")))
#endif

float roundUpToMultipleOf2(float num);
float scaleFloat(float v, float min, float max);
float unscaleFloat(float v, float min, float max);
geode::Patch* createPatchSafe(void *address, const geode::ByteVector &data);
cocos2d::CCRect getScreenSafeArea();
cocos2d::CCRect getScreenSafeAreaiOS();

#include <imgui-cocos.hpp>

ImVec4 ccc4ToVec(cocos2d::ccColor4B col);
cocos2d::ccColor4B vecToCCC4(ImVec4 vec);

std::string applyRTL(const std::string& str);
std::string applyRTLFix(const std::string& str);

namespace ImGuiExt
{
	void colouredText(std::string label);
};