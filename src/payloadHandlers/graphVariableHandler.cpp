#pragma once
#include "payloadHandler.h"

void graphVariableHandler::setGraphVariableBool(RE::Actor* actor, std::vector<std::string_view>* in)
{
	bool b;
	if (!Utils::string_view::to_bool(in->at(2), b)) {
		printErrMsg(in, "Invalid argument for setGraphVariableBool.");
		return;
	}
	
	actor->SetGraphVariableBool(std::string(in->at(1)), b);
}

void graphVariableHandler::setGraphVariableFloat(RE::Actor* actor, std::vector<std::string_view>* in)
{
	float f;
	if (!Utils::string_view::to_float(in->at(2), f)) {
		printErrMsg(in, "Invalid argument for setGraphVariableFloat.");
		return;
	}
	actor->SetGraphVariableFloat(std::string(in->at(1)), f);
}

void graphVariableHandler::setGraphVariableInt(RE::Actor* actor, std::vector<std::string_view>* in)
{
	int i;
	if (!Utils::string_view::to_int(in->at(2), i)) {
		printErrMsg(in, "Invalid argument for setGraphVariableInt.");
		return;
	}
	actor->SetGraphVariableInt(std::string(in->at(1)), i);
}


void graphVariableHandler::process(RE::Actor* actor, std::vector<std::string_view>* param, GRAPHVARIABLETYPE graphVariableType)
{
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
