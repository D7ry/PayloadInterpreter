#pragma once
#include "payloadHandler.h"
void nifHandler::process(RE::Actor* actor, std::vector<std::string> v) {
	//0 instruction
	//1 nif model path
	//2 biped body index
	if (!checkParamCt(v, 4)) {
		return;
	}
}