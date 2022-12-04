#include "payloadHandler.h"

void CPRHandler::process(RE::Actor* actor, std::vector<std::string_view>* v, FUNCTION f)
{
	switch (f) {
	case FUNCTION::EnableAdvance:
		enableAdvance(actor, v);
		break;
	case FUNCTION::EnableBackoff:
		enableBackoff(actor, v);
		break;
	case FUNCTION::EnableCircling:
		enableCircling(actor, v);
		break;
	case FUNCTION::EnableSurround:
		enableSurround(actor, v);
		break;
	case FUNCTION::EnableFallback:
		enableFallback(actor, v);
		break;
	}
}

void CPRHandler::enableAdvance(RE::Actor* a_actor, std::vector<std::string_view>* v)
{
	if (!checkParamCt(v, 6)) {
		return;
	}
	float CPR_InnerRadiusMin, CPR_InnerRadiusMid, CPR_InnerRadiusMax, CPR_OuterRadiusMin, CPR_OuterRadiusMid, CPR_OuterRadiusMax;
	if (!Utils::string_view::to_float(v->at(1), CPR_InnerRadiusMin) 
		|| !Utils::string_view::to_float(v->at(2), CPR_InnerRadiusMid) 
		|| !Utils::string_view::to_float(v->at(3), CPR_InnerRadiusMax) 
		|| !Utils::string_view::to_float(v->at(4), CPR_OuterRadiusMin) 
		|| !Utils::string_view::to_float(v->at(5), CPR_OuterRadiusMid) 
		|| !Utils::string_view::to_float(v->at(6), CPR_OuterRadiusMax)) {
		printErrMsg(v, "Invalid argument for CPR_EnableAdvance.");
		return;
	}
	logger::info("CPR:EnableAdvance for {}", a_actor->GetName());
	a_actor->SetGraphVariableBool("CPR_EnableAdvanceRadius", true);

	//Set innerRadius value
	a_actor->SetGraphVariableFloat("CPR_InnerRadiusMin", CPR_InnerRadiusMin);
	a_actor->SetGraphVariableFloat("CPR_InnerRadiusMid", CPR_InnerRadiusMid);
	a_actor->SetGraphVariableFloat("CPR_InnerRadiusMax", CPR_InnerRadiusMax);

	//Set OuterRadius value
	a_actor->SetGraphVariableFloat("CPR_OuterRadiusMin", CPR_OuterRadiusMin);
	a_actor->SetGraphVariableFloat("CPR_OuterRadiusMid", CPR_OuterRadiusMid);
	a_actor->SetGraphVariableFloat("CPR_OuterRadiusMax", CPR_OuterRadiusMax);
}

void CPRHandler::enableBackoff(RE::Actor* a_actor, std::vector<std::string_view>* v)
{
	if (!checkParamCt(v, 2)) {
		return;
	}
	float MinDistMult, BackoffChance;
	if (!Utils::string_view::to_float(v->at(1), MinDistMult) || !Utils::string_view::to_float(v->at(2), BackoffChance)) {
		printErrMsg(v, "Invalid argument for CPR_EnableBackoff.");
		return;
	}
	logger::info("CPR:EnableBackoff for {}", a_actor->GetName());
	//Enable data override on vanilla Backoff data.
	a_actor->SetGraphVariableBool("CPR_EnableBackoff", true);

	//Set Minimum Backoff Distance Multiple.
	a_actor->SetGraphVariableFloat("CPR_BackoffMinDistMult", MinDistMult);

	//Set Backoff Chance.
	a_actor->SetGraphVariableFloat("CPR_BackoffChance", BackoffChance);
	
}

void CPRHandler::enableCircling(RE::Actor* a_actor, std::vector<std::string_view>* v)
{
	if (!checkParamCt(v, 2)) {
		return;
	}
	float CirclingDistMin, CirclingDistMax;
	if (!Utils::string_view::to_float(v->at(1), CirclingDistMin) || !Utils::string_view::to_float(v->at(2), CirclingDistMax)) {
		printErrMsg(v, "Invalid argument for CPR_EnableCircling.");
		return;
	}
	logger::info("CPR:EnableCircling for {}", a_actor->GetName());
	//Enable data override on vanilla Circling data.
	a_actor->SetGraphVariableBool("CPR_EnableCircling", true);

	//Set Minimum Circling Distance.
	a_actor->SetGraphVariableFloat("CPR_CirclingDistMin", CirclingDistMin);

	//Set Maximum Circling Distance.
	a_actor->SetGraphVariableFloat("CPR_CirclingDistMax", CirclingDistMax);
}

void CPRHandler::enableSurround(RE::Actor* a_actor, std::vector<std::string_view>* v)
{
	if (!checkParamCt(v, 2)) {
		return;
	}
	float SurroundDistMin, SurroundDistMax;
	if (!Utils::string_view::to_float(v->at(1), SurroundDistMin) || !Utils::string_view::to_float(v->at(2), SurroundDistMax)) {
		printErrMsg(v, "Invalid argument for CPR_EnableSurround.");
		return;
	}
	logger::info("CPR:EnableSurround for {}", a_actor->GetName());
	//Enable data override on vanilla Surround data.
	a_actor->SetGraphVariableBool("CPR_EnableSurround", true);

	//Set Minimum Surround Distance.
	a_actor->SetGraphVariableFloat("CPR_SurroundDistMin", SurroundDistMin);

	//Set Maximum Surround Distance.
	a_actor->SetGraphVariableFloat("CPR_SurroundDistMax", SurroundDistMax);
}

void CPRHandler::enableFallback(RE::Actor* a_actor, std::vector<std::string_view>* v)
{
	if (!checkParamCt(v, 2)) {
		return;
	}

	float FallbackDistMin, FallbackDistMax;
	if (!Utils::string_view::to_float(v->at(1), FallbackDistMin) || !Utils::string_view::to_float(v->at(2), FallbackDistMax)) {
		printErrMsg(v, "Invalid argument for CPR_EnableFallback.");
		return;
	}
	logger::info("CPR:EnableFallback for {}", a_actor->GetName());
	//Enable data override on vanilla Fallback data.
	a_actor->SetGraphVariableBool("CPR_EnableFallback", true);

	//Set Minimum Fallback Distance.
	a_actor->SetGraphVariableFloat("CPR_FallbackDistMin", FallbackDistMin);

	//Set Maximum Fallback Distance.
	a_actor->SetGraphVariableFloat("CPR_FallbackDistMax", FallbackDistMax);
}
