#include "payloadHandlers/payloadHandler.h"
void spellApplyHandler::process(RE::Actor* a_actor, std::vector<std::string> v, spellApplyHandler::OPERATION op) {
	//@ADDSPELL|FormID|.esp
	if (checkParamCt(v, 2)) {
		return;
	}
	int spellForm = 0;
	if (!Utils::ToInt(v[1], spellForm)
		|| spellForm == 0) {
		printErrMsg(v, "invalid spell formID");
		return;
	}
	
	auto spell = RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(spellForm, v[2]);
	if (!spell) {
		printErrMsg(v, "spell not found");
		return;
	}

	switch (op) {
	case OPERATION::add:a_actor->AddSpell(spell);
	case OPERATION::remove:a_actor->RemoveSpell(spell);
	}
	
}