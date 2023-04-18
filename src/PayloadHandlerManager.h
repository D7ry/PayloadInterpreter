#pragma once

#include "API.h"

using PayloadHandler = payloadinterpreter::PayloadHandler;

class PayloadHandlerManager : public payloadinterpreter::API::PayloadHandlerCollector
{
public:
	static PayloadHandlerManager* GetSingleton()
	{
		static PayloadHandlerManager singleton;

		return &singleton;
	}

	void RegisterPayloadHandler(const char* a_eventTag, PayloadHandler* a_payloadHandler) final
	{
		std::string eventTag{ a_eventTag };

		// Convert to uppercase so it can be checked case insensitive
		// (some animators are using "Pie" instead of "PIE" for the event tag)
		for (char& c : eventTag) c = static_cast<char>(std::toupper(c));

		handlers.emplace(eventTag, a_payloadHandler);
	}

	void CollectExternPayloadHandlers()
	{
		payloadinterpreter::API::Message msg{ this };

		SKSE::GetMessagingInterface()->Dispatch(0, &msg, sizeof(msg), nullptr);
	}
	
	PayloadHandler* GetPayloadHandlerFor(std::string a_eventTag) const
	{
		// Case insensitive search of handler
		for (char& c : a_eventTag) c = static_cast<char>(std::toupper(c));

		if (handlers.contains(a_eventTag)) {
			return handlers.at(a_eventTag);
		} else {
			return nullptr;
		}
	}

private:
	PayloadHandlerManager() = default;

	std::unordered_map<std::string, PayloadHandler*> handlers;
};
