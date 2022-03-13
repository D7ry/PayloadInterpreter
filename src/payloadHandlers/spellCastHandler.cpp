#pragma once
#include "payloadHandler.h"
void spellCastHandler::process(RE::Actor* actor, std::vector<std::string_view> arr){
	//@CAST|FormID|.esp|Effectiveness|Magnitude|Self-Targeting(0/1)|Health Requirement|Health Cost|Stamina Requirement|Stamina Cost|Magicka Requirement|Magicka Cost
	DEBUG("Casting spell!");

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
	int spellForm = 0;
	if (!Utils::ToInt(arr[1].data(), spellForm)
		|| !spellForm == 0) {
		DEBUG("Error: invalid spell formID");
		return;
	}
	RE::SpellItem* spell = RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(spellForm, arr[2]);
	if (!spell) {
		ERROR("Error: spell does not exist");
		return;
	}

	//Check actor value requirements.
	if (actor->GetActorValue(RE::ActorValue::kHealth) < std::stof(arr[6].data())
		||actor->GetActorValue(RE::ActorValue::kStamina) < std::stof(arr[8].data())
		||actor->GetActorValue(RE::ActorValue::kMagicka) < std::stof(arr[10].data())
		) {
		return;
	}

	//Check if the spell is self-targeting.
	RE::Actor* target = nullptr;
	switch (std::stoi(arr[5].data())) {
	case 0: target = nullptr;
	case 1: target = actor;
	default: ERROR("Error: Wrong payload input!");
	}

	//Cast the spell
	actor->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
		->InstantCast(spell, false, target, std::stof(arr[3].data()), false, std::stof(arr[4].data()), actor);

	//Damage actor value
	
	if (arr[7] != 0) {
		Utils::damageav(actor, RE::ActorValue::kHealth, std::stof(arr[7].data()));
	}
	if (arr[9] != 0) {
		Utils::damageav(actor, RE::ActorValue::kStamina, std::stof(arr[9].data()));
	}
	if (arr[11] != 0) {
		Utils::damageav(actor, RE::ActorValue::kMagicka, std::stof(arr[11].data()));
	}
};