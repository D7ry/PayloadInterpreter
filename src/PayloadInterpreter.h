#pragma once

#include "PayloadHandlerManager.h"

class PayloadInterpreter : public PayloadHandler
{
public:
	static PayloadInterpreter* GetSingleton()
	{
		static PayloadInterpreter singleton;

		return &singleton;
	}

	// Load pre-defined payloads from .ini files
	void LoadPreDefinedPayloads();

private:
	PayloadInterpreter() = default;

	// override (PayloadHandler)
	void Process(RE::TESObjectREFR* a_holder, const std::string_view& a_payload, RE::BShkbAnimationGraph* a_animationGraph) final;

	// Pre-process different types of instructions
	void PreProcess(RE::Actor* a_actor, const std::string_view& a_payload);

	// Delegate a parsed native instruction to corresponding handlers
	void DelegateNative(RE::Actor* a_actor, const std::string_view& a_payload);

	// Try to match pre-defined payload to their pre-defined definitions in .ini and do work
	void DelegateCustom(RE::Actor* a_actor, const std::string_view& a_payload);

	// Push an async payload task
	void DelegateAsync(RE::Actor* a_actor, const std::string_view& a_payload);

	// Perform a timer and then preprocess the payload instruction
	void AsyncThreadFunc(float a_delay, RE::Actor* a_actor, const std::string& a_payload);

	// Read everything from a single ini
	void ReadSingleIni(const char* a_iniPath);

	// members
	static constexpr std::string_view configDir = "Data/SKSE/PayloadInterpreter/Config";

	// Mapping of pre-defined instructions to actual tokenized instructions
	std::unordered_map<std::string, std::vector<std::string>> instructionMap;
};
