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
		std::vector<std::string> //vector containing all instructions that matches the payload.
		> 
		preDefinedInstructions;

	/*Task queue for async payload operations.*/
	static inline robin_hood::unordered_map
		<
		RE::Actor*,
		std::vector
		<
		std::pair<float, std::string> //Waiting time, unparsed instruction.
		>
		>
		asyncTaskQueue;
public:
	static inline bool hasAsyncTask; //switch for async tasking, hooked to main update.
	static void update(); //called once per frame
	/*preProcess different types of instructions.*/
	static void preProcess(RE::Actor* actor, std::string a_payload);
	/*Delegate a parsed native instruction to corresponding handlers.*/
	static void delegateNative(RE::Actor* actor, std::string a_payload);
	/*Try to match pre-defined payload to their pre-defined definitions in .ini and do work.*/
	static void delegateCustom(RE::Actor* actor, std::string a_payload);
	/*Push a async payload task.*/
	static void delegateAsync(RE::Actor* actor, std::string a_payload);

	/*Load pre-defined payload from .ini file.*/
	static void loadPreDefinedPayload();
private:
	static void readSingleIni(const char* ini_path);

};
