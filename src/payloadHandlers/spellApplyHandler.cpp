#include "payloadHandlers/payloadHandler.h"
void spellApplyHandler::process(RE::Actor* a_actor, std::vector<std::string_view>* v, spellApplyHandler::OPERATION op) {
	//@ADDSPELL|FormID|.esp
	if (!checkParamCt(v, 2)) {
		return;
	}
	int spellForm;
	RE::SpellItem* spell = nullptr;

	if (!Utils::lookup(v->at(1), v->at(2), spell)) {
		printErrMsg(v, "Invalid spell formId or plugin name.");
		return;
	}


	switch (op) {
	case OPERATION::add:a_actor->AddSpell(spell); break;
	case OPERATION::remove:a_actor->RemoveSpell(spell); break;
	}
	
}
