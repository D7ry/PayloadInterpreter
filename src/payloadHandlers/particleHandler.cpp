#pragma once
#include "payloadHandler.h"
void particleHandler::process(RE::Actor* actor, std::vector<std::string_view>* v) {
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
	if (!actor->GetActorRuntimeData().currentProcess || !actor->GetActorRuntimeData().currentProcess->high) {
		printErrMsg(v, "actor not in high process");
		return;
	}
	auto cell = actor->GetParentCell();
	if (!cell) {
		printErrMsg(v, "cell not found.");
		return;
	}

	int actorNodeIndex;
	if (!Utils::string_view::to_int(v->at(2), actorNodeIndex)) {
		printErrMsg(v, "invalid actor node index.");
		return;
	}
	auto actorNode = actor->GetCurrentBiped()->objects[static_cast<RE::BIPED_OBJECT>(actorNodeIndex)].partClone;
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
	float xOffset, yOffset, zOffset;
	if (!Utils::string_view::to_float(v->at(6), xOffset)) {
		printErrMsg(v, "invalid x offset.");
		return;
	}
	if (!Utils::string_view::to_float(v->at(7), yOffset)) {
		printErrMsg(v, "invalid y offset.");
		return;
	}
	if (!Utils::string_view::to_float(v->at(8), zOffset)) {
		printErrMsg(v, "invalid z offset.");
		return;
	}

	//obtain and offset nif position
	auto vfxNode = actorNode->worldBound.center + Utils::vectorMatrixMult({ xOffset, yOffset, zOffset }, actorNode->world.rotate);
	
	float lifeTime;
	if (!Utils::string_view::to_float(v->at(4), lifeTime)) {
		printErrMsg(v, "invalid life time.");
		return;
	}

	float scale;
	if (!Utils::string_view::to_float(v->at(3), scale)) {
		printErrMsg(v, "invalid scale.");
		return;
	}

	uint32_t flags;
	if (!Utils::string_view::to_uint(v->at(5), flags)) {
		printErrMsg(v, "invalid flags.");
		return;
	}
	RE::BSTempEffectParticle::Spawn(
		cell,
		lifeTime,
		std::string(v->at(1)).data(),
		actorNode->world.rotate,
		vfxNode,
		scale,
		flags,
		actorNodeObj);
}
