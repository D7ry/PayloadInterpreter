#include "payloadHandlers/spellCastHandler.h"
#include "payloadHandlers/actorValueHandler.h"
#include "payloadHandlers/graphVariableHandler.h"
#include "payloadHandlers/globalTimeHandler.h"
#include "Utils.h"
#pragma once

/*bunch of regex to match payload input.*/
namespace PF_Regex
{
	/*standard payload begin with "#".*/
	namespace std
	{
		using namespace boost;
		static const boost::regex head = boost::regex("\\#.+");

		static const boost::regex setGraphVariable = boost::regex("\\#SGV.+");
		static const boost::regex modGraphVariable = boost::regex("\\#MGV.+");

		static const boost::regex setActorVariable = boost::regex("\\#SAV.+");
		static const boost::regex modActorVariable = boost::regex("\\#MAV.+");

		static const boost::regex castSpell = boost::regex("\\#CAS.+");

		static const boost::regex setGlobalTimeMultiplier = boost::regex("\\#SGT\\|(\\d|\\.)+\\|");

		static const boost::regex screenShake = boost::regex("\\#SHK.+");
	};

	/*convenience payloads begin with "$". Convenience payloads are for more specific operations,
	such as enabling i-frame.*/
	namespace convenience
	{
		static const boost::regex head = boost::regex("\\$.+");

		static const boost::regex enableIframe = boost::regex("\\$enableIframe"); //i-frame will be disabled as soon as one exits the animation.
		static const boost::regex disableIframe = boost::regex("\\$disableIframe");
	}
};

/*class pre-processing all payloads and filter out unwanted ones.*/
class payloadManager
{
public:
	/*In charge of pre-processing all payloads:
1. pattern match payload commands and filter out unwanted commands.
2. for matched commands, extract their parameters as a vector and pass them into
	corresponding payload handlers for furutre parsing and processsing.*/
	static void preProcessPayload(RE::Actor* actor, std::string payload) {
		DEBUG("processing {} for {}", payload, actor->GetName());
		if (boost::regex_match(payload, PF_Regex::std::head)) {
			standardPayload(actor, payload);
		}
		else if (boost::regex_match(payload, PF_Regex::convenience::head)) {
			conveniencePayload(actor, payload);
		}
	};

	/*revert all changes caused by payload.*/
	static void revertChanges(RE::Actor* actor) {

	}
private:
	inline static void standardPayload(RE::Actor* actor, std::string payload) {
		if (regex_match(payload, PF_Regex::std::setGraphVariable)) {
			DEBUG("matched setGv");
		}
		else if (regex_match(payload, PF_Regex::std::modGraphVariable)) {
			DEBUG("matched modGv");
		}
		else if (regex_match(payload, PF_Regex::std::setActorVariable)) {
			DEBUG("matched setAv");
		}
		else if (regex_match(payload, PF_Regex::std::modActorVariable)) {
			DEBUG("matched modAv");
		}
		else if (regex_match(payload, PF_Regex::std::castSpell)) {
			DEBUG("matched cast spell");
		}
		else if (regex_match(payload, PF_Regex::std::setGlobalTimeMultiplier)) {
			DEBUG("matched SGTM");
			if (actor->IsPlayerRef()) {
				//SGTM only triggers on player payload
				globalTimeHandler::process(Utils::splitString(payload, '|'));
			}
		}
		else if (regex_match(payload, PF_Regex::std::screenShake)) {
			DEBUG("matched screen shake");
			if (actor->IsPlayerRef()) {

			}
		}
	}

	inline static void conveniencePayload(RE::Actor* actor, std::string payload) {
		if (regex_match(payload, PF_Regex::convenience::enableIframe)) {
			DEBUG("matched enableIframe");

		}
		else if (regex_match(payload, PF_Regex::convenience::disableIframe)) {
			DEBUG("matched disableIframe");
		}
	}
};


//SGS() _set game settings

//SGV(i/f/b|"string of graph variable"|true/false in 0/1|keepChanges(true/false in 1/0)) _set graph variable
//MGV(i/f|"string of graph variable"|+/- float/int depending on the choice|keepChanges(true/false in 1/0)) _modify graph variable
//SAV(av(in the form of int)|magnitude as float, -+|keepChanges(true/false in 1/0)) _set actor value
//MAV(av(in the form of int)|magnitude as float, -+|keepChanges(true/false in 1/0)) _modify Actor value



//CAS(spell form|plugin name|effectiveness|magnitude|magickaCost|StaminaCost) _cast a spell
//FOV(desiredFOV) (only works for player) _modify FOV, changes will be automatically reverted
//SHK(magnitude) (only works for player) _shake screen, changes will be automatically reverted
//SGT(magnitude(must be positive)) _set global time multiplier. change will be automatically reverted