#pragma once
#include "payloadManager.h"
namespace Hook
{
	class hook_animationEvent
	{
	public:
		static void install()
		{
			logger::info("Installing animation event hook...");
			REL::Relocation<uintptr_t> AnimEventVtbl_NPC{ RE::VTABLE_Character[2] };
			REL::Relocation<uintptr_t> AnimEventVtbl_PC{ RE::VTABLE_PlayerCharacter[2] };

			_ProcessEvent_NPC = AnimEventVtbl_NPC.write_vfunc(0x1, ProcessEvent_NPC);
			_ProcessEvent_PC = AnimEventVtbl_PC.write_vfunc(0x1, ProcessEvent_PC);
		}

	private:
		static inline void ProcessEvent(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource) 
		{
			if (a_event->tag != "PIE") {
				return;
			}
			RE::TESObjectREFR* holder = const_cast<RE::TESObjectREFR*>(a_event->holder);

			if (!holder) {
				return;
			}

			payloadManager::preProcess(holder->As<RE::Actor>(), a_event->payload.data());

		}
		using EventResult = RE::BSEventNotifyControl;
		static EventResult ProcessEvent_NPC(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource) 
		{
			ProcessEvent(a_sink, a_event, a_eventSource);
			return _ProcessEvent_NPC(a_sink, a_event, a_eventSource);
		}
		static EventResult ProcessEvent_PC(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource) 
		{
			ProcessEvent(a_sink, a_event, a_eventSource);
			return _ProcessEvent_PC(a_sink, a_event, a_eventSource);
		}

		static inline REL::Relocation<decltype(ProcessEvent_NPC)> _ProcessEvent_NPC;
		static inline REL::Relocation<decltype(ProcessEvent_NPC)> _ProcessEvent_PC;
	};
	
}
