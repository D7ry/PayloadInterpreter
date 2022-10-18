#pragma once
#include "payloadHandler.h"
#include "offsets.h"
void setGhostHandler::process(RE::Actor* actor, std::vector<std::string_view>* v) {
	if (!checkParamCt(v, 1)) {
		return;
	}

	if (!actor) {
		return;
	}

	bool isGhost = false;
	if (!Utils::string_view::to_bool(v->at(1), isGhost)) {
		printErrMsg(v, "Invalid argument for ghost.");
		return;
	}

	offsets::setIsGhost(actor, isGhost);
}
