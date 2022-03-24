#pragma once
#include "payloadHandler.h"
#include "offsets.h"
inline void shakeScreen(RE::Actor* actor, std::vector<std::string> arr) {
	// 1 = cam shake strength
	// 2 = cam shake duration
	offsets::shakeCamera(std::stof(arr[1]), actor->GetPosition(), std::stof(arr[2]));
}
void cameraHandler::process(RE::Actor* actor, std::vector<std::string> arr, CAMOPTYPE op) {
	if (!actor->IsPlayerRef()) {
		return;
	}
	if (!checkParamCt(arr, 2)) {
		return;
	};
	switch (op) {
	case CAMOPTYPE::screenShake: shakeScreen(actor, arr); break;
	}
};


