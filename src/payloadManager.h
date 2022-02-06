#pragma once
#include "spellCastHandler.h"
using namespace std;
/*bunch of regex to match payload input.*/
namespace apf_regex
{
	static const regex setGraphVariable = regex("SGV\\*+");
	static const regex modGraphVariable = regex("MGV\\*+");

	static const regex setActorVariable = regex("SAV\\*+");
	static const regex modActorVariable = regex("MAV\\*+");

	static const regex castSpell = regex("CAS\\*+");
};


class payloadManager
{
public:
	/*pre-process the payload and delegate task to corresponding handlers, who will further parse the payload and process.
	@param a_actor the actor whose animation triggers the payload.
	@param a_payload the payload body. */
	static void preProcessPayload(RE::Actor* actor, string payload) {
		DEBUG("processing payload {} for {}", payload, actor->GetName());
		if (regex_match(payload, apf_regex::setGraphVariable)) {
			DEBUG("matched setGv");
		}
		else if (regex_match(payload, apf_regex::modGraphVariable)) {
			DEBUG("matched modGv");
		}
		else if (regex_match(payload, apf_regex::setActorVariable)) {
			//e.g.:SAV(
			DEBUG("matched setAv");
		}
		else if (regex_match(payload, apf_regex::modActorVariable)) {
			DEBUG("matched modAv");
		}
		else if (regex_match(payload, apf_regex::castSpell)) {
			DEBUG("matched cast spell");
		}
	};
};
//SGV(i/f/b|"string of graph variable"|true/false in 0/1|keepChanges(true/false in 1/0)) _set graph variable
//MGV(i/f|"string of graph variable"|+/- float/int depending on the choice|keepChanges(true/false in 1/0)) _modify graph variable
//SAV(av(in the form of int)|magnitude as float, -+|keepChanges(true/false in 1/0)) _set actor value
//MAV(av(in the form of int)|magnitude as float, -+|keepChanges(true/false in 1/0)) _modify Actor value
//CAS(spell form|plugin name|effectiveness|magnitude|magickaCost|StaminaCost) _cast a spell
//FOV(desiredFOV) (only works for player) _modify FOV, changes will be automatically reverted
//SHK(magnitude) (only works for player) _shake screen, changes will be automatically reverted