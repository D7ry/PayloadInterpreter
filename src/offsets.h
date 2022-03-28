#pragma once
class offsets
{
public:

	typedef void(_fastcall* _shakeCamera)(float strength, RE::NiPoint3 source, float duration);
	typedef void(_fastcall* _setIsGhost)(RE::Actor* actor, bool isGhost);

#if ANNIVERSARY_EDITION
	static inline REL::Relocation<_shakeCamera> shakeCamera{ REL::ID(33012) };					//50E5F0
	static inline REL::Relocation<_setIsGhost> setIsGhost{ REL::ID(37276) };						//5F6C60
	static inline float* g_deltaTime = (float*)REL::ID(410199).address();                         // 30064C8
	static inline float* g_deltaTimeRealTime = (float*)REL::ID(410200).address();                 // 30064CC
#else 
	static inline REL::Relocation<_shakeCamera> shakeCamera{ REL::ID(32275) };
	static inline REL::Relocation<_setIsGhost> setIsGhost{ REL::ID(36287) };
	static inline float* g_deltaTime = (float*)REL::ID(523660).address();                            // 2F6B948
	static inline float* g_deltaTimeRealTime = (float*)REL::ID(523661).address();                  // 2F6B94C
#endif
	

	


};