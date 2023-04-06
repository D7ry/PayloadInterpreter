#pragma once

#include "PayloadHandlerManager.h"

namespace CPR
{
	class PayloadInterpreter : public PayloadHandler
	{
	public:
		static PayloadInterpreter* GetSingleton()
		{
			static PayloadInterpreter singleton;

			return &singleton;
		}

	private:
		PayloadInterpreter() = default;

		// override (PayloadHandler)
		void Process(RE::TESObjectREFR* a_holder, const std::string_view& a_payload, RE::BShkbAnimationGraph* a_animationGraph) final;
	};
}
