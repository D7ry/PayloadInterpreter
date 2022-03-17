#pragma once
#include "payloadHandler.h"
void globalTimeHandler::process(std::vector<std::string> param) {
	DEBUG("SGTM to {}", param[1]);
	Utils::SGTM(std::stof(param[1].data()));
};


