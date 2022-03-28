#include "payloadManager.h"
#include "SimpleIni.h"
#include "offsets.h"
void payloadManager::update() {
	if (asyncTaskQueue.size() == 0) {
		//DEBUG("async queue empty, turning off update");
		hasAsyncTask = false; //flip switch if no more async task.
		return;
	}

	auto it_actor = asyncTaskQueue.begin(); //iterator for the queue
	while (it_actor != asyncTaskQueue.end()) {
		if (it_actor->second.size() == 0) {// actor has no more queued task
			//DEBUG("{} has no more async task, erasing actor!", it_actor->first->GetName());
			it_actor = asyncTaskQueue.erase(it_actor);
			continue;
		}
		//do work
#pragma region TaskIterator
		auto it_task = it_actor->second.begin();//iterator for all tasks of a single actor.
		while (it_task != it_actor->second.end()) {
			if (it_task->first <= 0) {
				//DEBUG("Initiating task: ", it_task->second);
				//submit this single task.
				const auto task = SKSE::GetTaskInterface();
				if (task != nullptr) {
					auto a_actor = it_actor->first;
					if (a_actor && a_actor->currentProcess //actor must be in high process
						&& a_actor->currentProcess->InHighProcess()) {
						auto a_instruction = it_task->second;
						task->AddTask([a_actor, a_instruction]() {
							preProcess(a_actor, a_instruction);
							});
					}
				}
				//erase this task.
				it_task = it_actor->second.erase(it_task);
				continue;
			}
			//or decrement timer for this task.
			it_task->first -= *offsets::g_deltaTimeRealTime;
			it_task++;
		}
#pragma endregion
		it_actor++;
	}
}
void payloadManager::preProcess(RE::Actor* actor, std::string a_payload) {
	DEBUG(a_payload);
	switch (a_payload.at(0)) {
	case '@': payloadManager::delegateNative(actor, a_payload); break;
	case '$': payloadManager::delegateCustom(actor, a_payload); break;
	case '!': payloadManager::delegateAsync(actor, a_payload); break;
	//default: INFO("Error: invalid payload tag: ", a_payload); break;
	}
}


void payloadManager::delegateNative(RE::Actor* actor, std::string a_payload) {
	auto tokens = Utils::tokenize(a_payload, '|');
	switch (hash(tokens[0].data(), tokens[0].size())) {
	case "@SGVB"_h:
		graphVariableHandler::process(actor, tokens, graphVariableHandler::GRAPHVARIABLETYPE::Bool); break;
		break;
	case "@SGVF"_h:
		graphVariableHandler::process(actor, tokens, graphVariableHandler::GRAPHVARIABLETYPE::Float); break;
	case "@SGVI"_h:
		graphVariableHandler::process(actor, tokens, graphVariableHandler::GRAPHVARIABLETYPE::Int); break;
	case "@CAST"_h:
	case "@CASTSPELL"_h:
		spellCastHandler::process(actor, tokens); break;
	case "@APPLYSPELL"_h:
		spellApplyHandler::process(actor, tokens, spellApplyHandler::OPERATION::add); break;
	case "@UNAPPLYSPELL"_h:
		spellApplyHandler::process(actor, tokens, spellApplyHandler::OPERATION::remove); break;
	case "@CAMSHAKE"_h:
		cameraHandler::process(actor, tokens, cameraHandler::CAMOPTYPE::screenShake); break;
	case "@SETGHOST"_h:
		setGhostHandler::process(actor, tokens); break;
	case "@PLAYPARTICLE"_h:
		particleHandler::process(actor, tokens); break;
	default:
		break;
		//payloadHandler::printErrMsg(tokens, "Invalid instruction.");
	}
};

void payloadManager::delegateCustom(RE::Actor* actor, std::string payload) {
	auto i = preDefinedInstructions.find(payload);
	if (i != preDefinedInstructions.end()) {
		for (auto instruction : i->second) {
			preProcess(actor, instruction);
		}
	}
	else {
		INFO("Error: payload mapping not found for {}.", payload);
	}
}

void payloadManager::delegateAsync(RE::Actor* actor, std::string a_payload) {
	//![Time]Actual_Payload
	DEBUG("processing async payload: " + a_payload);
	size_t start = a_payload.find_first_of('[');
	size_t end = a_payload.find_first_of(']');
	if (start == std::string::npos
		|| end == std::string::npos) {
		INFO("Error: invalid payload input: " + a_payload);
	}
	float a_time = std::stof(a_payload.substr(start + 1, end - start - 1));
	//DEBUG("time: {}", a_time);
	std::string a_instruction = a_payload.substr(end + 1);
	//DEBUG("instruction: {}", a_instruction);
	auto it = asyncTaskQueue.find(actor);
	if (it != asyncTaskQueue.end()) { //actor has other async tasks
		//DEBUG("initializing async task");
		it->second.push_back(std::pair<float, std::string>{a_time, a_instruction});
	}
	else { //actor only has one async task
		asyncTaskQueue.emplace(actor, 
			std::vector<std::pair<float, std::string>>
			{ 
				{a_time, a_instruction}//insert one task
			}
		);
	}
	hasAsyncTask = true; //flip the switch
	//DEBUG("finished porcessing");
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
		INFO("Loading section {}...", a_section);
		ini.GetAllKeys(a_section, keys);
		for (CSimpleIniA::TNamesDepend::iterator s_it1 = keys.begin(); s_it1 != keys.end(); s_it1++) { //iterate through a section's all keys
			const char* a_key = s_it1->pItem; 
			INFO("Loading payload mapping for key: {}", a_key);
			if (a_key[0] == '$') {
				std::list<CSimpleIniA::Entry> ls;
				if (ini.GetAllValues(a_section, a_key, ls)
					&& ls.size() != 0) {
					for (CSimpleIniA::Entry e : ls) { //iterate through a key's all values
						string a_instruction = e.pItem;
;						auto it = preDefinedInstructions.find(a_key);
						if (it == preDefinedInstructions.end()) {
							preDefinedInstructions.emplace(a_key, vector<string>{a_instruction});
						}
						else {
							it->second.push_back(a_instruction);
						}
						INFO("mapped to instruction: {}", a_instruction);
					}
				}
			}
			else {
				INFO("Error: invalid key. Key should begin with $.");
			}
		}
	}
}
void payloadManager::loadPreDefinedPayload() {
	INFO("Loading predefined payload instructions...");
	for (const auto& entry : std::filesystem::directory_iterator(configDir)) { //iterates through all .ini files
		std::string pathStr = entry.path().string();
		INFO("Loading from {}", pathStr);
		const char* cstr = pathStr.c_str();
		readSingleIni(cstr);
	}
	INFO("Predefined instructions loaded.");
}
#pragma endregion