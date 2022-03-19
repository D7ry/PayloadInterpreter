#include "payloadHandlers/payloadHandler.h"
#include "Utils.h"
#pragma once
constexpr uint32_t hash(const char* data, size_t const size) noexcept
{
	uint32_t hash = 5381;

	for (const char* c = data; c < data + size; ++c) {
		hash = ((hash << 5) + hash) + (unsigned char)*c;
	}

	return hash;
}

constexpr uint32_t operator"" _h(const char* str, size_t size) noexcept
{
	return hash(str, size);
}


/*class pre-processing all payloads and filter out unwanted ones.*/
class payloadManager
{
public:
	/*Tokenize the payload and dedicate it to corresponding handlers.*/
	static void preProcessPayload(RE::Actor* actor, std::string payload) {
		//DEBUG("processing {} for {}", payload, actor->GetName());
		std::vector<std::string> tokens = Utils::tokenize(payload, '|');
		//DEBUG(tokens[0]);
		switch (hash(tokens[0].data(), tokens[0].size())) {
		case "@SGVB"_h:
			graphVariableHandler::process(actor, tokens, graphVariableHandler::GRAPHVARIABLETYPE::Bool); break;
			break;
		case "@SGVF"_h:
			graphVariableHandler::process(actor, tokens, graphVariableHandler::GRAPHVARIABLETYPE::Float); break;
		case "@SGVI"_h:
			graphVariableHandler::process(actor, tokens, graphVariableHandler::GRAPHVARIABLETYPE::Int); break;
		case "@CAST"_h:
			spellCastHandler::process(actor, tokens); break;
		case "@CAMSHAKE"_h:
			cameraHandler::process(actor, tokens, cameraHandler::CAMOPTYPE::screenShake); break;
		case "@SETGHOST"_h:
			setGhostHandler::process(actor, tokens); break;
		case "PLAYNIF"_h:
			nifHandler::process(actor, tokens); break;
		default:
			INFO("Error: invalid instruction: " + payload);
		}
		


	};

	/*revert all changes caused by payload.*/
	static void revertChanges(RE::Actor* actor) {

	}

};
