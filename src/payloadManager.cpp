#include "payloadManager.h"
#include "SimpleIni.h"
void payloadManager::preProcessPayload(RE::Actor* actor, std::vector<std::string> tokens) {
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

void payloadManager::matchPreDefinedPayload(RE::Actor* actor, std::string payload) {
	auto i = preDefinedInstructions.find(payload);
	if (i != preDefinedInstructions.end()) {
		for (std::vector<std::string> v : i->second) {
			preProcessPayload(actor, v);
		}
	}
	else {
		INFO("Error: payload mapping not found for {}.", payload);
	}
}
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
					&& ls.size() != 0
					) {
					for (CSimpleIniA::Entry e : ls) { //iterate through a key's all values
						string a_instruction = e.pItem;
						vector<string> instructions = Utils::tokenize(a_instruction);
						auto it = preDefinedInstructions.find(a_key);
						if (it == preDefinedInstructions.end()) {
							preDefinedInstructions.emplace(a_key, vector<vector<string>>{instructions});
						}
						else {
							it->second.push_back(instructions);
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