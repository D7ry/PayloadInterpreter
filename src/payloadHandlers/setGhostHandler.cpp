#pragma once
#include "payloadHandler.h"
#include "offsets.h"
void setGhostHandler::process(RE::Actor* actor, std::vector<std::string> v) {
	if (!checkParamCt(v, 1)) {
		return;
	}

	bool isGhost = false;
	switch (std::stoi(v[1])) {
	case 0: break;
	case 1: isGhost = true; break;
	default: printErrMsg(v, "invalid parameter for self-targeting spell."); return;
	}

	if (actor) {
		offsets::setIsGhost(actor, isGhost);
	}
}
