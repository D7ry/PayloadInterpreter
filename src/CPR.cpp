#include "CPR.h"
#include "Utils.h"

#include "payloadHandlers/payloadHandler.h"

namespace CPR
{
	void PayloadInterpreter::Process(RE::TESObjectREFR* a_holder, const std::string_view& a_payload, RE::BShkbAnimationGraph* a_animationGraph)
	{
		using namespace Utils::literals;

		RE::Actor* actor = const_cast<RE::Actor*>(a_holder->As<RE::Actor>());

		if (actor) {
			logger::info("CPR instruction triggered for {}; instruction: {}", actor->GetName(), a_payload);
			std::vector<std::string_view> tokens;
			Utils::splitSV(tokens, a_payload, '|');
			switch (Utils::hash(tokens[0].data(), tokens[0].size())) {
			case "EnableAdvance"_h:  // CPR.EnableAdvance|111|222|333|444|555|666
				CPRHandler::process(actor, &tokens, CPRHandler::FUNCTION::EnableAdvance);
				break;
			case "EnableBackoff"_h:  // CPR.EnableBackoff|11|22
				CPRHandler::process(actor, &tokens, CPRHandler::FUNCTION::EnableBackoff);
				break;
			case "EnableCircling"_h:  // CPR.EnableCircling|33|44
				CPRHandler::process(actor, &tokens, CPRHandler::FUNCTION::EnableCircling);
				break;
			case "EnableSurround"_h:  // CPR.EnableSurround|...
				CPRHandler::process(actor, &tokens, CPRHandler::FUNCTION::EnableSurround);
				break;
			case "EnableFallback"_h:  // CPR.EnableFallback|...
				CPRHandler::process(actor, &tokens, CPRHandler::FUNCTION::EnableFallback);
				break;
			}
		}
	}
}
