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
	static inline const std::string configDir = "Data/SKSE/PayloadInterpreter/Config";
	/*Mapping of pre-defined instructions to actual tokenized instructions.*/
	static inline robin_hood::unordered_map
		<
		std::string, //instruction name that are defined
		std::vector<std::vector<std::string>> //vector containing all tokenized instructions that match instruction name
		> preDefinedInstructions;
public:
	/*Dedicate payload to corresponding handlers.*/
	static void preProcessPayload(RE::Actor* actor, std::vector<std::string> tokens);
	/*Try to match pre-defined payload to their definitions and process.*/
	static void matchPreDefinedPayload(RE::Actor* actor, std::string payload);
	/*Load pre-defined payload from .ini file.*/
	static void loadPreDefinedPayload();
private:
	static void readSingleIni(const char* ini_path);

};
