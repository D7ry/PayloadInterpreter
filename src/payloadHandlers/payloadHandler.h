#pragma once
#include "Utils.h"
class payloadHandler {
public:
	/*process the parameters of a payload funcion.*/
	static void process(RE::Actor* actor, std::vector<std::string_view> param) {
		//parse the information in the payload command and execute commands.
	};

private:
	/*check if the amount of parameter in the vector is correct. If not, gives an error message.*/
	static inline bool checkParamCt(std::vector<std::string> param, int ct) {
		if (param.size() != ct) {
			ERROR("Error: Incorrect number of parameters passed in");
		}
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
	static void process(RE::Actor* actor, std::vector<std::string_view> param, GRAPHVARIABLETYPE graphVariableType);

};

class globalTimeHandler : public payloadHandler {
public:
	/*process the parameters of a payload funcion.*/
	//SGTM takes 1 param.
	static void process(std::vector<std::string_view> param);

};

/*Deal with spell casting.*/
class spellCastHandler : public payloadHandler {
public:
	static void process(RE::Actor* actor, std::vector<std::string_view> param);
};