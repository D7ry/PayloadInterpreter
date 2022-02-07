#pragma once
#include "payloadHandler.h"
#include "Utils.h"
inline void SGTM(float a_in) {
    static float* g_SGTM = (float*)REL::ID(511883).address();
    *g_SGTM = a_in;
    using func_t = decltype(SGTM);
    REL::Relocation<func_t> func{ REL::ID(66989) };
    return;
}

class globalTimeHandler : public payloadHandler {
public:
	/*process the parameters of a payload funcion.*/
	//SGTM takes 1 param.
	static void process(std::vector<std::string> param) {
		DEBUG("SGTM to {}", param[1]);
		SGTM(std::stof(param[1]));
	};

};

