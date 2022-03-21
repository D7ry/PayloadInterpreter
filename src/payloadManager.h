#include "payloadHandlers/payloadHandler.h"
#include "Utils.h"
#include "lib/robin_hood.h"
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
	/*Mapping of pre-defined instructions to actual tokenized instructions.*/
	static inline robin_hood::unordered_map<std::string, std::vector<std::string>> preDefinedInstructions;
public:
	/*Dedicate payload to corresponding handlers.*/
	static void preProcessPayload(RE::Actor* actor, std::vector<std::string> tokens) {
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
		case "@PLAYPARTICLE"_h:
			particleHandler::process(actor, tokens); break;
		default:
			payloadHandler::printErrMsg(tokens, "Invalid instruction.");
		}
	};

	static void matchDefinedPayload(RE::Actor* actor, std::string payload) {
		auto tokenizedPayload = preDefinedInstructions.find(payload);
		if (tokenizedPayload != preDefinedInstructions.end()) {
			preProcessPayload(actor, tokenizedPayload->second);
		}
	}

private:
	

};
