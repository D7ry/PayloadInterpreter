#pragma once
#include "payloadManager.h"

class Hook_MainUpdate
{
public:
	static void install() {
		auto& trampoline = SKSE::GetTrampoline();

		REL::Relocation<uintptr_t> hook{ REL::ID(35551) };  // 5AF3D0, main loop

		_Update = trampoline.write_call<5>(hook.address() + 0x11F, Update);
		INFO("Main Update hook installed.");
	}

private:
	static void Update(RE::Main* a_this, float a2) {
		if (payloadManager::hasAsyncTask) {
			payloadManager::update();
		}
		_Update(a_this, a2);
	}
	static inline REL::Relocation<decltype(Update)> _Update;

};
class Hooks {
public:

	static void install() {
		SKSE::AllocTrampoline(1 << 4);
		Hook_MainUpdate::install();
	}
};