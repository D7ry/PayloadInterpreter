#pragma once
#include "payloadHandler.h"
void globalTimeHandler::process(std::vector<std::string_view>* param) {
	float arg1;
	if (Utils::string_view::to_float(param->at(1), arg1)) {
		Utils::SGTM(arg1);
	}
};


