#pragma once
class offsets
{
public:

	typedef void(_fastcall* _shakeCamera)(float strength, RE::NiPoint3 source, float duration);
	typedef void(_fastcall* _setIsGhost)(RE::Actor* actor, bool isGhost);

	static inline REL::Relocation<_shakeCamera> shakeCamera{ RELOCATION_ID(32275,33012) };
	static inline REL::Relocation<_setIsGhost> setIsGhost{ RELOCATION_ID(36287,37276) };
	static inline float* g_deltaTime = (float*)RELOCATION_ID(523660,410199).address();                            // 2F6B948
	static inline float* g_deltaTimeRealTime = (float*)RELOCATION_ID(523661,410200).address();                  // 2F6B94C





};