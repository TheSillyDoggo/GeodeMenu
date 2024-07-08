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

float roundUpToMultipleOf2(float num);
float scaleFloat(float v, float min, float max);