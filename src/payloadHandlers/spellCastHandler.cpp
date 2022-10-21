#pragma once
#include "payloadHandler.h"
void spellCastHandler::process(RE::Actor* actor, std::vector<std::string_view>* arr){
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


	RE::SpellItem* spell = nullptr;
	
	if (!Utils::lookup(arr->at(1), arr->at(2), spell)) {
		printErrMsg(arr, "Invalid spell formId or plugin name.");
		return;
	}

	/*Can you believe this github copilot wrote all those annoying boilerplates*/
	float effectiveness, magnitude, healthReq, healthCost, staminaReq, staminaCost, magickaReq, magickaCost;
	bool selfTargeting;
	if (!Utils::string_view::to_float(arr->at(3), effectiveness) 
		|| !Utils::string_view::to_float(arr->at(4), magnitude) 
		|| !Utils::string_view::to_bool(arr->at(5), selfTargeting) 
		|| !Utils::string_view::to_float(arr->at(6), healthReq) 
		|| !Utils::string_view::to_float(arr->at(7), healthCost) 
		|| !Utils::string_view::to_float(arr->at(8), staminaReq) 
		|| !Utils::string_view::to_float(arr->at(9), staminaCost) 
		|| !Utils::string_view::to_float(arr->at(10), magickaReq) 
		|| !Utils::string_view::to_float(arr->at(11), magickaCost)) {
		printErrMsg(arr, "Invalid argument for spell cast.");
		return;
	}
	//Check actor value requirements.
	if (actor->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth) < healthReq 
		|| actor->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStamina) < staminaReq
		||actor->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMagicka) < magickaReq
		) {
		return;
	}

	//Check if the spell is self-targeting.
	RE::Actor* target = nullptr;
	if (selfTargeting) {
		target = actor;
	} else {
		target = nullptr;
	}
	
	//Cast the spell
	actor->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
		->CastSpellImmediate(spell, false, target, effectiveness, false, magnitude, actor);

	//Damage actor value
	if (healthCost != 0) {
		Utils::damageav(actor, RE::ActorValue::kHealth, healthCost);
	}
	if (staminaCost != 0) {
		Utils::damageav(actor, RE::ActorValue::kStamina, staminaCost);
	}
	if (magickaCost != 0) {
		Utils::damageav(actor, RE::ActorValue::kMagicka, magickaCost);
	}
};
