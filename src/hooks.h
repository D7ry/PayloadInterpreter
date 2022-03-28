#pragma once
#include "payloadManager.h"

class Hook_MainUpdate
{
public:

	static void install() {
		auto& trampoline = SKSE::GetTrampoline();
#if ANNIVERSARY_EDITION
		REL::Relocation<uintptr_t> hook{ REL::ID(36544) };  // 5D29F0, main loop
		_Update = trampoline.write_call<5>(hook.address() + 0x160, Update);
#else
		REL::Relocation<uintptr_t> hook{ REL::ID(35551) };  // 5AF3D0, main loop
		_Update = trampoline.write_call<5>(hook.address() + 0x11F, Update);
#endif

		
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

class Hook_PlayerUpdate
{
public:
	static void install() {
		REL::Relocation<std::uintptr_t> PlayerCharacterVtbl{ RE::Offset::PlayerCharacter::Vtbl };
		_Update = PlayerCharacterVtbl.write_vfunc(0xAD, Update);
		INFO("Player update hook installed");
	}
private:
	
	static void Update(RE::PlayerCharacter* a_this, float a_delta) {
		if (payloadManager::hasAsyncTask) {
			payloadManager::update();
		}
		_Update(a_this, a_delta);
	}
	static inline REL::Relocation<decltype(Update)> _Update;
};
class Hooks {
public:

	static void install() {
		SKSE::AllocTrampoline(1 << 5);
		//Hook_MainUpdate::install();
		Hook_PlayerUpdate::install();
	}
};