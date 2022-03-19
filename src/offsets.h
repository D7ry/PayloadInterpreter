#pragma once
class offsets
{
public:

	typedef void(_fastcall* _shakeCamera)(float strength, RE::NiPoint3 source, float duration);
	static inline REL::Relocation<_shakeCamera> shakeCamera{ REL::ID(32275) };

	typedef void(_fastcall* _setIsGhost)(RE::Actor* actor, bool isGhost);
	static inline REL::Relocation<_setIsGhost> setIsGhost{ REL::ID(36287) };
};