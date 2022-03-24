#include "events.h"
#include "payloadManager.h"
#include "hooks.h"
void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kDataLoaded:
		//dataHandler::testInsertWeaponArt();
		//((animEventHandler*)((uintptr_t)RE::PlayerCharacter::GetSingleton() + 0x30))->HookSink();
		INFO("Hooksink anim events...");
		REL::Relocation<uintptr_t> npcPtr{ REL::ID(261399) };
		REL::Relocation<uintptr_t> pcPtr{ REL::ID(261918) };
		animEventHandler::HookSink(pcPtr.address());
		animEventHandler::HookSink(npcPtr.address());
		INFO("Anim event hook sinked");
		payloadManager::loadPreDefinedPayload();
		break;
	}
}
#if ANNIVERSARY_EDITION

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []()
{
	SKSE::PluginVersionData data{};

	data.PluginVersion(Version::MAJOR);
	data.PluginName(Version::NAME);
	data.AuthorName("dTry"sv);

	data.CompatibleVersions({ SKSE::RUNTIME_LATEST });
	data.UsesAddressLibrary(true);

	return data;
}();

#else

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	DKUtil::Logger::Init(Version::PROJECT, Version::NAME);

	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = Version::PROJECT.data();
	a_info->version = Version::MAJOR;

	if (a_skse->IsEditor()) {
		ERROR("Loaded in editor, marking as incompatible"sv);
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (ver < SKSE::RUNTIME_1_5_39) {
		ERROR("Unable to load this plugin, incompatible runtime version!\nExpected: Newer than 1-5-39-0 (A.K.A Special Edition)\nDetected: {}", ver.string());
		return false;
	}

	return true;
}

#endif


extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
#if ANNIVERSARY_EDITION

	DKUtil::Logger::Init(Version::PROJECT, Version::NAME);

	if (REL::Module::get().version() < SKSE::RUNTIME_1_6_317) {
		ERROR("Unable to load this plugin, incompatible runtime version!\nExpected: Newer than 1-6-317-0 (A.K.A Anniversary Edition)\nDetected: {}", REL::Module::get().version().string());
		return false;
	}

#endif

	INFO("{} v{} loaded", Version::PROJECT, Version::NAME);

	SKSE::Init(a_skse);
	auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener("SKSE", MessageHandler)) {
		return false;
	}
	Hooks::install();
	return true;
}
