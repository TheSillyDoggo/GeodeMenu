#define MODULE_ALL_HOOKS(func) \
static void onModify(auto& self) \
{ \
	if (self.m_hooks.empty()) \
		return; \
	for (auto hook : self.m_hooks) \
	{ \
        func::get()->addHook(hook.second.get()); \
	} \
}