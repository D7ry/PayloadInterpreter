#include "payloadHandlers/payloadHandler.h"
#include "Utils.h"
#pragma once


/*class pre-processing all payloads and filter out unwanted ones.*/
class payloadManager
{
	static inline const std::string configDir = "Data/SKSE/PayloadInterpreter/Config";
	/*Mapping of pre-defined instructions to actual tokenized instructions.*/
	static inline std::unordered_map<std::string, std::vector<std::string*>*> instructionMap;

public:
	//static void update(); //called once per frame
	/*preProcess different types of instructions.*/
	static void preProcess(RE::Actor* actor, std::string* a_payload);
	static void preProcess(RE::Actor* actor, std::string a_payload);
	/*Delegate a parsed native instruction to corresponding handlers.*/
	static void delegateNative(RE::Actor* actor, std::string* a_payload);
	/*Try to match pre-defined payload to their pre-defined definitions in .ini and do work.*/
	static void delegateCustom(RE::Actor* actor, std::string* payload);
	/*Push a async payload task.*/
	static void delegateAsync(RE::Actor* actor, std::string* a_payload);
	/*Function run in an async thread.
	@param time: time until the payload instruction fires.*/
	static void asyncThreadFunc(float time, RE::Actor* actor, std::string a_payload);
	/*Load pre-defined payload from .ini file.*/
	static void loadPreDefinedPayload();
private:
	static void readSingleIni(const char* ini_path);

	/// <summary>
	/// Write log to the logs file regarding the actors current animation.
	/// </summary>
	/// <param name="a_actor"></param>
	static void logCurrentAnim(RE::Actor* a_actor);

};


/* Temp class for CPR*/
class CPR : public payloadHandler
{
public:
	static void delegateNative(RE::Actor* actor, std::string* a_payload);
};
