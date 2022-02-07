#pragma once
class payloadHandler {
public:
	/*process the parameters of a payload funcion.*/
	static void process(RE::Actor* actor, std::vector<std::string> param) {
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