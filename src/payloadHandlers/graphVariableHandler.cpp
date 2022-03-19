#pragma once
#include "payloadHandler.h"
inline void setGraphVariableBool(RE::Actor* actor, std::vector<std::string> in) {
	bool b;
	switch (std::stoi(in[2])) {
	case 0: b = false; break;
	case 1: b = true; break;
	default: ERROR("Error: Wrong payload input!");
	}
	actor->SetGraphVariableBool(in[1], b);
	//DEBUG("set {}'s graph variable bool {} to {}", actor->GetName(), in[1], in[2]);
}

inline void setGraphVariableFloat(RE::Actor* actor, std::vector<std::string> in) {
	actor->SetGraphVariableFloat(in[1].data(), std::stof(in[2]));
	//DEBUG("set {}'s graph variable float {} to {}", actor->GetName(), in[1], in[2]);
}

inline void setGraphVariableInt(RE::Actor* actor, std::vector<std::string> in) {
	//DEBUG("HOE DISTAR 4");
	actor->SetGraphVariableInt(in[1], std::stoi(in[2]));
	//DEBUG("set {}'s graph variable int {} to {}", actor->GetName(), in[1], std::stoi(in[2]));
}


void graphVariableHandler::process(RE::Actor* actor, std::vector<std::string> param, GRAPHVARIABLETYPE graphVariableType) {
	if (!checkParamCt(param, 2)) {
		return;
	}
	switch (graphVariableType) {
	case GRAPHVARIABLETYPE::Bool: setGraphVariableBool(actor, param); break;
	case GRAPHVARIABLETYPE::Float: setGraphVariableFloat(actor, param); break;
	case GRAPHVARIABLETYPE::Int: setGraphVariableInt(actor, param); break;
	}
}


/*
@SGVB|"string of graph variable"|0(False)/1(True)| - set graph variable bool
@SGVF|"string of graph variable"|any float value| - set graph variable float
@SGVI|"string of graph variable"|any int value| - set graph variable int
*/