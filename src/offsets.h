#pragma once
class offsets
{
public:

	typedef void(_fastcall* _shakeCamera)(float strength, RE::NiPoint3 source, float duration);
	typedef void(_fastcall* _setIsGhost)(RE::Actor* actor, bool isGhost);

#if ANNIVERSARY_EDITION
	
#else 
	static inline REL::Relocation<_shakeCamera> shakeCamera{ REL::ID(32275) };
	static inline REL::Relocation<_setIsGhost> setIsGhost{ REL::ID(36287) };
	static inline float* g_deltaTime = (float*)REL::ID(523660).address();                            // 2F6B948
	static inline float* g_deltaTimeRealTime = (float*)REL::ID(523661).address();                  // 2F6B94C
#endif
	

	


};