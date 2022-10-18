#pragma once
#include "payloadHandler.h"
#include "offsets.h"
inline void shakeScreen(RE::Actor* actor, std::vector<std::string_view>* arr) {
	// 1 = cam shake strength
	// 2 = cam shake duration
	float arg1, arg2;
	if (Utils::string_view::to_float(arr->at(1), arg1) && Utils::string_view::to_float(arr->at(2), arg2)) {
		offsets::shakeCamera(arg1, actor->GetPosition(), arg2);
	}
}
void cameraHandler::process(RE::Actor* actor, std::vector<std::string_view>* arr, CAMOPTYPE op) {
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


