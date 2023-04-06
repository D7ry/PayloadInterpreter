#include "PayloadInterpreter.h"

#include "Utils.h"
#include "settings.h"

#include "payloadHandlers/payloadHandler.h"

void PayloadInterpreter::LoadPreDefinedPayloads()
{
	logger::info("Loading predefined payload instructions...");
	if (std::filesystem::is_directory(configDir)) {
		for (const auto& entry : std::filesystem::directory_iterator(configDir)) {  //iterates through all .ini files
			std::string pathStr = entry.path().string();
			logger::info("Loading from {}", pathStr);
			const char* cstr = pathStr.c_str();
			ReadSingleIni(cstr);
		}
	}

	logger::info("Predefined instructions loaded.");
}

void PayloadInterpreter::ReadSingleIni(const char* a_iniPath)
{
	using namespace std;
	CSimpleIniA ini;
	ini.SetMultiKey(true);
	ini.LoadFile(a_iniPath);
	std::list<CSimpleIniA::Entry> sections;
	ini.GetAllSections(sections);
	for (const CSimpleIniA::Entry& s_it0 : sections) {  //iterate thru all sections
		CSimpleIniA::TNamesDepend keys;
		const char* section = s_it0.pItem;
		//logger::info("Loading section {}...", a_section);
		ini.GetAllKeys(section, keys);
		for (CSimpleIniA::TNamesDepend::iterator s_it1 = keys.begin(); s_it1 != keys.end(); s_it1++) {  //iterate through a section's all keys
			const char* key = s_it1->pItem;
			//logger::info("Loading payload mapping for key {} :", a_key);
			if (key[0] != '$') {
				const char* msg = "Error: invalid key. Mapping entries must begin with $";
				if (settings::bWarnAgainstInvalidConfigs) {
					logger::error("{}", msg);
				}
				continue;
			}
			std::list<CSimpleIniA::Entry> ls;
			if (ini.GetAllValues(section, key, ls) && ls.size() != 0) {
				for (const CSimpleIniA::Entry& e : ls) {  //iterate through a key's all values
					string instruction{ e.pItem };
					auto it = instructionMap.find(key);
					if (it == instructionMap.end()) {
						std::vector<std::string> vec;
						vec.push_back(instruction);
						instructionMap.emplace(key, vec);
					} else {
						it->second.push_back(instruction);
					}
					logger::info("{}", instruction);
				}
			}
			logger::info("mapping loaded for {}", key);
		}
	}
}

void PayloadInterpreter::Process(RE::TESObjectREFR* a_holder, const std::string_view& a_payload, RE::BShkbAnimationGraph* a_animationGraph)
{
	RE::Actor* actor = const_cast<RE::Actor*>(a_holder->As<RE::Actor>());

	if (actor) {
		PreProcess(actor, a_payload);
	}
}

void PayloadInterpreter::PreProcess(RE::Actor* a_actor, const std::string_view& a_payload)
{
	switch (a_payload[0]) {
	case '@':
		DelegateNative(a_actor, a_payload);
		break;
	case '$':
		DelegateCustom(a_actor, a_payload);
		break;
	case '!':
		DelegateAsync(a_actor, a_payload);
		break;
	default:
		break;
	}
}

void PayloadInterpreter::DelegateNative(RE::Actor* a_actor, const std::string_view& a_payload)
{
	using namespace Utils::literals;

	std::vector<std::string_view> tokens;
	Utils::splitSV(tokens, a_payload, '|');

	switch (Utils::hash(tokens[0].data(), tokens[0].size())) {
	case "@SGVB"_h:
		graphVariableHandler::process(a_actor, &tokens, graphVariableHandler::GRAPHVARIABLETYPE::Bool);
		break;
	case "@SGVF"_h:
		graphVariableHandler::process(a_actor, &tokens, graphVariableHandler::GRAPHVARIABLETYPE::Float);
		break;
	case "@SGVI"_h:
		graphVariableHandler::process(a_actor, &tokens, graphVariableHandler::GRAPHVARIABLETYPE::Int);
		break;
	case "@CAST"_h:
	case "@CASTSPELL"_h:
		spellCastHandler::process(a_actor, &tokens);
		break;
	case "@APPLYSPELL"_h:
		spellApplyHandler::process(a_actor, &tokens, spellApplyHandler::OPERATION::add);
		break;
	case "@UNAPPLYSPELL"_h:
		spellApplyHandler::process(a_actor, &tokens, spellApplyHandler::OPERATION::remove);
		break;
	case "@CAMSHAKE"_h:
		cameraHandler::process(a_actor, &tokens, cameraHandler::CAMOPTYPE::screenShake);
		break;
	case "@SETGHOST"_h:
		setGhostHandler::process(a_actor, &tokens);
		break;
	case "@PLAYPARTICLE"_h:
		particleHandler::process(a_actor, &tokens);
		break;
	default:
		break;
		payloadHandler::printErrMsg(&tokens, "Invalid instruction.");
	}
};

void PayloadInterpreter::DelegateCustom(RE::Actor* a_actor, const std::string_view& a_payload)
{
	auto i = instructionMap.find(a_payload.data());
	if (i != instructionMap.end()) {
		for (const std::string& instruction : i->second) {
			PreProcess(a_actor, instruction);
		}
	}
}

void PayloadInterpreter::DelegateAsync(RE::Actor* a_actor, const std::string_view& a_payload)
{
	size_t delayStartPos = a_payload.find('[');
	size_t delayEndPos = a_payload.find(']');
	
	if (delayStartPos == std::string::npos || delayEndPos == std::string::npos) {
		logger::info("Error: invalid async payload input: {}", a_payload);
		return;
	}
	delayStartPos++;

	std::string_view delayStr = a_payload.substr(delayStartPos, delayEndPos - delayStartPos);

	float delay;
	if (std::from_chars(delayStr.data(), delayStr.data() + delayStr.size(), delay).ec == std::errc::invalid_argument)
	{
		return;
	}

	//DEBUG("time: {}", a_time);
	std::string payloadRest{ a_payload.substr(delayEndPos + 1) };  //make a copy, since a_payload gets popped off of the stack

	auto ThreadFunc = [this](float a_time, RE::Actor* a_actor, const std::string& a_payload) -> void {
		AsyncThreadFunc(a_time, a_actor, a_payload);
	};

	std::jthread thread(ThreadFunc, delay, a_actor, payloadRest);
	thread.detach();
}

void PayloadInterpreter::AsyncThreadFunc(float a_delay, RE::Actor* a_actor, const std::string& a_payload)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(a_delay * 1000)));
	const auto taskIface = SKSE::GetTaskInterface();

	if (taskIface != nullptr) {
		if (a_actor && a_actor->GetActorRuntimeData().currentProcess  //a_actor must be in high process
			&& a_actor->GetActorRuntimeData().currentProcess->InHighProcess()) {
			taskIface->AddTask([this, a_actor, a_payload]() -> void {
				PreProcess(a_actor, a_payload);
			});
		}
	}
}
