#pragma once
#include "payloadHandler.h"
void spellCastHandler::process(RE::Actor* actor, std::vector<std::string> arr){
	//@CAST|FormID|.esp|Effectiveness|Magnitude|Self-Targeting(0/1)|Health Requirement|Health Cost|Stamina Requirement|Stamina Cost|Magicka Requirement|Magicka Cost
	//DEBUG("Processing spell payload");
	// 1 = formID (hex
	// 2 = esp plugin name (string
	// 3 = Effectiveness (float
	// 4 = Magnitude (float
	// 5 = if self targeting (bool
	// 6 = Health requirement (float
	// 7 = Healdh cost (float
	// 8 = Stamina requirement (float
	// 9 = Stamina cost (float
	// 10 = Magicka requirement (float
	// 11 = Magicka cost (float
	//Read spell
	if (!checkParamCt(arr, 11)) {
		return;
	}
	int spellForm = 0;
	if (!Utils::ToInt(arr[1], spellForm)
		|| spellForm == 0) {
		printErrMsg(arr, "invalid spell formID");
		return;
	}

	RE::SpellItem* spell = RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(spellForm, arr[2]);
	if (!spell) {
		printErrMsg(arr, "failed to look up spell");
		return;
	}

	//Check actor value requirements.
	if (actor->GetActorValue(RE::ActorValue::kHealth) < std::stof(arr[6])
		||actor->GetActorValue(RE::ActorValue::kStamina) < std::stof(arr[8])
		||actor->GetActorValue(RE::ActorValue::kMagicka) < std::stof(arr[10])
		) {
		//DEBUG("Actor value requirement not met.");
		return;
	}

	//Check if the spell is self-targeting.
	RE::Actor* target = nullptr;
	switch (std::stoi(arr[5])) {
	case 0: target = nullptr; break;
	case 1: target = actor; break;
	default: printErrMsg(arr, "invalid parameter for self-targeting spell."); return;
	}

	//Cast the spell
	actor->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
		->InstantCast(spell, false, target, std::stof(arr[3]), false, std::stof(arr[4]), actor);

	//Damage actor value
	spell->CalculateMagickaCost(actor);
	if (std::stof(arr[7]) != 0) {
		Utils::damageav(actor, RE::ActorValue::kHealth, std::stof(arr[7]));
	}
	if (std::stof(arr[9]) != 0) {
		Utils::damageav(actor, RE::ActorValue::kStamina, std::stof(arr[9]));
	}
	if (std::stof(arr[11]) != 0) {
		Utils::damageav(actor, RE::ActorValue::kMagicka, std::stof(arr[11]));
	}
};