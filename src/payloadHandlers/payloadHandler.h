#pragma once
#include "Utils.h"
class payloadHandler {
public:
	/*process the parameters of a payload funcion.*/
	static void process(RE::Actor* actor, std::vector<std::string> param) {
		//parse the information in the payload command and execute commands.
	};

	/*Prints out an error message and the command errored on.
	@param params: tokenized instruction.
	@param errMsg: Error message to print.*/
	static void printErrMsg(std::vector<std::string> params, std::string errMsg) {
		std::string orgCmd = std::accumulate(params.begin(), params.end(), std::string{}); //Concatenate the params to obtain original command
		INFO("Error: " + errMsg + " "
			+ "Errored instruction: " + orgCmd);
	}
	/*check if the amount of parameter in the vector is correct. If not, prints an error message.
	@param param: vector containing all parameters.
	@param ct: expected i.e. correct number of parameters.
	@return whether the number of tokens in param matches ct.*/
	static inline bool checkParamCt(std::vector<std::string> param, int ct) {
		if (param.size() != ct) {
			printErrMsg(param,
				"incorrect number of parameters passed in, expected: "
				+ std::to_string(ct) + "received: " + std::to_string(param.size())
			);
			return false;
		}
		return true;
	}


};
/*Deal with animation graph variables.*/
class graphVariableHandler : public payloadHandler {
public:
	/*type of graph variable*/
	enum GRAPHVARIABLETYPE {
		Float = 0,
		Int,
		Bool
	};
	static void process(RE::Actor* actor, std::vector<std::string> param, GRAPHVARIABLETYPE graphVariableType);

};

class globalTimeHandler : public payloadHandler {
public:
	/*process the parameters of a payload funcion.*/
	//SGTM takes 1 param.
	static void process(std::vector<std::string> param);

};

/*Deal with spell casting.*/
class spellCastHandler : public payloadHandler {
public:
	static void process(RE::Actor* actor, std::vector<std::string> param);
};

/*Deal with screenshake&FOV changes*/
class cameraHandler : public payloadHandler {
public:
	/*Types of camera operation*/
	enum CAMOPTYPE {
		screenShake = 0,
		FOV,
	};
	/*Shakes the player's camera.*/
	static void process(RE::Actor* actor, std::vector<std::string> param, CAMOPTYPE op);
};