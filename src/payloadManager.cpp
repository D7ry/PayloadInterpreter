#include "payloadManager.h"
#include "offsets.h"
#include "settings.h"

using namespace Utils;
using namespace Utils::literals;

/**
* Looks at the 1st character of a payload and returns the corresponding payloadHandler.
* Note: A_PAYLOAD may be freed by the caller once this function returns, so it's the callee's responsibility to copy it if needed(for async stuff).
*/
void payloadManager::preProcess(RE::Actor* actor, std::string* a_payload)
{
	switch (a_payload->front()) {
	case '@':
		payloadManager::delegateNative(actor, a_payload);
		break;
	case '$':
		payloadManager::delegateCustom(actor, a_payload);
		break;
	case '!':
		payloadManager::delegateAsync(actor, a_payload);
		break;
	default:
		break;
		////logger::info("Error: invalid payload prefix: {} for payload {}.", a_payload->front(), a_payload);
	}
}

void payloadManager::preProcess(RE::Actor* actor, std::string a_payload) 
{
	preProcess(actor, &a_payload);
}


void payloadManager::delegateNative(RE::Actor* actor, std::string* a_payload) {
	std::vector<std::string_view> tokens;
	Utils::splitSV(tokens, std::string_view(a_payload->c_str()), '|');
	switch (hash(tokens[0].data(), tokens[0].size())) {
	case "@SGVB"_h:
		graphVariableHandler::process(actor, &tokens, graphVariableHandler::GRAPHVARIABLETYPE::Bool); break;
		break;
	case "@SGVF"_h:
		graphVariableHandler::process(actor, &tokens, graphVariableHandler::GRAPHVARIABLETYPE::Float); break;
	case "@SGVI"_h:
		graphVariableHandler::process(actor, &tokens, graphVariableHandler::GRAPHVARIABLETYPE::Int); break;
	case "@CAST"_h:
	case "@CASTSPELL"_h:
		spellCastHandler::process(actor, &tokens); break;
	case "@APPLYSPELL"_h:
		spellApplyHandler::process(actor, &tokens, spellApplyHandler::OPERATION::add); break;
	case "@UNAPPLYSPELL"_h:
		spellApplyHandler::process(actor, &tokens, spellApplyHandler::OPERATION::remove); break;
	case "@CAMSHAKE"_h:
		cameraHandler::process(actor, &tokens, cameraHandler::CAMOPTYPE::screenShake); break;
	case "@SETGHOST"_h:
		setGhostHandler::process(actor, &tokens); break;
	case "@PLAYPARTICLE"_h:
		particleHandler::process(actor, &tokens); break;
	default:
		break;
		payloadHandler::printErrMsg(&tokens, "Invalid instruction.");
	}
};

void payloadManager::delegateCustom(RE::Actor* actor, std::string* payload) {
	auto i = instructionMap.find(payload->data());
	if (i != instructionMap.end()) {
		for (std::string* instruction : *i->second) {
			preProcess(actor, instruction);
		}
	}

}

void payloadManager::delegateAsync(RE::Actor* a_actor, std::string* a_payload) {
	size_t start = a_payload->find_first_of('[');
	size_t end = a_payload->find_first_of(']');
	if (start == std::string::npos
		|| end == std::string::npos) {
		logger::info("Error: invalid async payload input: {}", *a_payload);
	}
	float time = std::stof(a_payload->substr(start + 1, end - start - 1));
	//DEBUG("time: {}", a_time);
	std::string rest(a_payload->substr(end + 1));  //make a copy, since A_PAYLOAD gets popped off of the stack
	std::jthread thread(asyncThreadFunc, time, a_actor, rest);
	thread.detach();
}

/*Perform a timer and then preprocess the payload instruction. Payload needs to be freed after proprocess.*/
void payloadManager::asyncThreadFunc(float time, RE::Actor* a_actor, std::string a_payload)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(time * 1000)));
	const auto task = SKSE::GetTaskInterface();
	
	if (task != nullptr) {
		if (a_actor && a_actor->GetActorRuntimeData().currentProcess //actor must be in high process
			&& a_actor->GetActorRuntimeData().currentProcess->InHighProcess()) {
			task->AddTask([a_actor, a_payload]() {
				preProcess(a_actor, a_payload);
				});
		}
	}
}
#pragma region data
/*Read everything from a single ini.*/
void payloadManager::readSingleIni(const char* ini_path) {
	using namespace std;
	CSimpleIniA ini;
	ini.SetMultiKey(true);
	ini.LoadFile(ini_path);
	std::list<CSimpleIniA::Entry> sections;
	ini.GetAllSections(sections);
	for (CSimpleIniA::Entry s_it0 : sections) { //iterate thru all sections
		CSimpleIniA::TNamesDepend keys;
		const char* a_section = s_it0.pItem;
		//logger::info("Loading section {}...", a_section);
		ini.GetAllKeys(a_section, keys);
		for (CSimpleIniA::TNamesDepend::iterator s_it1 = keys.begin(); s_it1 != keys.end(); s_it1++) { //iterate through a section's all keys
			const char* a_key = s_it1->pItem;
			//logger::info("Loading payload mapping for key {} :", a_key);
			if (a_key[0] != '$') {
				const char* msg = "Error: invalid key. Mapping entries must begin with $";
				if (settings::bWarnAgainstInvalidConfigs) {
					logger::error("{}",msg);
				}
				continue;
			}
			std::list<CSimpleIniA::Entry> ls;
			if (ini.GetAllValues(a_section, a_key, ls)
				&& ls.size() != 0) {
				for (CSimpleIniA::Entry e : ls) { //iterate through a key's all values
					string* instruction = new std::string(e.pItem);
;						auto it = instructionMap.find(a_key);
					if (it == instructionMap.end()) {
						std::vector<std::string*>* vec = new std::vector<std::string*>();
						vec->push_back(instruction);
						instructionMap.emplace(a_key, vec);
					}
					else {
						it->second->push_back(instruction);
					}
					logger::info("{}", *instruction);
				}
			}
			logger::info("mapping loaded for {}", a_key);

		}
	}
}

void payloadManager::loadPreDefinedPayload() {
	logger::info("Loading predefined payload instructions...");
	if (std::filesystem::is_directory(configDir)) {
		for (const auto& entry : std::filesystem::directory_iterator(configDir)) { //iterates through all .ini files
			std::string pathStr = entry.path().string();
			logger::info("Loading from {}", pathStr);
			const char* cstr = pathStr.c_str();
			readSingleIni(cstr);
		}
	}

	logger::info("Predefined instructions loaded.");
}
#pragma endregion

void CPR::delegateNative(RE::Actor* actor, std::string* a_payload)
{
	logger::info("CPR instruction triggered for {}; instruction: {}", actor->GetName(), a_payload->c_str());
	std::vector<std::string_view> tokens;
	Utils::splitSV(tokens, std::string_view(a_payload->c_str()), '|');
	switch (hash(tokens[0].data(), tokens[0].size())) {
	case "EnableAdvance"_h: // CPR.EnableAdvance|111|222|333|444|555|666
		CPRHandler::process(actor, &tokens, CPRHandler::FUNCTION::EnableAdvance);
		break;
	case "EnableBackoff"_h: // CPR.EnableBackoff|11|22
		CPRHandler::process(actor, &tokens, CPRHandler::FUNCTION::EnableBackoff);
		break;
	case "EnableCircling"_h: // CPR.EnableCircling|33|44
		CPRHandler::process(actor, &tokens, CPRHandler::FUNCTION::EnableCircling);
		break;
	case "EnableSurround"_h: // CPR.EnableSurround|...
		CPRHandler::process(actor, &tokens, CPRHandler::FUNCTION::EnableSurround);
		break;
	case "EnableFallback"_h: // CPR.EnableFallback|...
		CPRHandler::process(actor, &tokens, CPRHandler::FUNCTION::EnableFallback);
		break;
	}
}
