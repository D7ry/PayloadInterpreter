#pragma once
#include "events.h"
#include "payloadManager.h"

RE::BSEventNotifyControl animEventHandler::HookedProcessEvent(RE::BSAnimationGraphEvent& a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* src) {
    
    FnProcessEvent fn = fnHash.at(*(uint64_t*)this);
	if (!a_event.holder) {
		return fn ? (this->*fn)(a_event, src) : RE::BSEventNotifyControl::kContinue;
	}
	if (a_event.payload.length() == 0) {//events without payload won't get processed
		return fn ? (this->*fn)(a_event, src) : RE::BSEventNotifyControl::kContinue;
	}
	std::string payLoad = a_event.payload.data();
	if (payLoad.at(0) == '@') { //only process valid payloads
		//DEBUG("matched custom payload");
		//DEBUG(payLoad);
		payloadManager::preProcessPayload(a_event.holder->As<RE::Actor>(), Utils::tokenize(payLoad, '|'));
	} 
	else if (payLoad.at(0) == '$') {
		DEBUG("matched pre-defined payload");
		payloadManager::matchDefinedPayload(a_event.holder->As<RE::Actor>(), payLoad);
	}



    return fn ? (this->*fn)(a_event, src) : RE::BSEventNotifyControl::kContinue;
}

std::unordered_map<uint64_t, animEventHandler::FnProcessEvent> animEventHandler::fnHash;