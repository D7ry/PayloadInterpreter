#include "events.h"
#include "payloadManager.h"

	/*all the payloads must begin with "$" to be recognized.*/
	static inline auto head = boost::regex("$\\*+");

RE::BSEventNotifyControl animEventHandler::HookedProcessEvent(RE::BSAnimationGraphEvent& a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* src) {
    
    FnProcessEvent fn = fnHash.at(*(uint64_t*)this);
	if (!a_event.holder) {
		return fn ? (this->*fn)(a_event, src) : RE::BSEventNotifyControl::kContinue;
	}
	if (a_event.payload.length() == 0) {
		return fn ? (this->*fn)(a_event, src) : RE::BSEventNotifyControl::kContinue; //0 length payload gets returned.
	}
	std::string payload = static_cast<std::string>(a_event.payload);
	if (boost::regex_match(payload, head)) { //matches the payload with the payload head.
		DEBUG("detected correct payload head, pre-processing payload");
		payloadManager::preProcessPayload(a_event.holder->As<RE::Actor>(), payload.substr(1));
	}

    return fn ? (this->*fn)(a_event, src) : RE::BSEventNotifyControl::kContinue;
}

std::unordered_map<uint64_t, animEventHandler::FnProcessEvent> animEventHandler::fnHash;