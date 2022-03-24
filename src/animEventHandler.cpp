#pragma once
#include "events.h"
#include "payloadManager.h"

RE::BSEventNotifyControl animEventHandler::HookedProcessEvent(RE::BSAnimationGraphEvent& a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* src) {
    
    FnProcessEvent fn = fnHash.at(*(uint64_t*)this);
	if (!a_event.holder) {
		return fn ? (this->*fn)(a_event, src) : RE::BSEventNotifyControl::kContinue;
	}
	if (a_event.payload.length() != 0) {//events without payload won't get processed
		payloadManager::preProcess(a_event.holder->As<RE::Actor>(), a_event.payload.data());
	}

    return fn ? (this->*fn)(a_event, src) : RE::BSEventNotifyControl::kContinue;
}

std::unordered_map<uint64_t, animEventHandler::FnProcessEvent> animEventHandler::fnHash;