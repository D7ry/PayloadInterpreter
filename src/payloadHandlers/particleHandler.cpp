#pragma once
#include "payloadHandler.h"
void particleHandler::process(RE::Actor* actor, std::vector<std::string> v) {
	//0 instruction
	//1 (string)nif model path
	//2 (int)biped body index
	//3 (float)nif scale
	//4 (float)play time
	//5 (uint) flags
	//6 X offset
	//7 Y offset
	//8 Z offset
	if (!checkParamCt(v, 8)) {
		return;
	}
	if (!actor) {
		return;
	}
	if (!actor->currentProcess || !actor->currentProcess->high) {
		printErrMsg(v, "actor not in high process");
		return;
	}
	auto cell = actor->GetParentCell();
	if (!cell) {
		printErrMsg(v, "cell not found.");
		return;
	}
	auto actorNode = actor->GetCurrentBiped()->objects[static_cast<RE::BIPED_OBJECT>(std::stoi(v[2]))].partClone;
	if (!actorNode) {
		printErrMsg(v, "actor biped node not found.");
		return;
	}
	auto actorNodeObj = actorNode.get();
	if (!actorNodeObj) {
		printErrMsg(v, "actor biped node object not found.");
		return;
	}
	//DEBUG("all checks passed");

	//obtain and offset nif position
	auto vfxNode = actorNode->worldBound.center + Utils::vectorMatrixMult({std::stof(v[6]), std::stof(v[7]), std::stof(v[8]) },
			actorNode->world.rotate);


	cell->PlaceParticleEffect(
		std::stof(v[4]),
		v[1].c_str(),
		actorNode->world.rotate,
		vfxNode,
		std::stof(v[3]),
		std::stoul(v[5], nullptr, 10),
		actorNodeObj
	);


}