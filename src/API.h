#pragma once

namespace RE
{
	class BShkbAnimationGraph;
	class TESObjectREFR;
}

namespace payloadinterpreter
{
	class PayloadHandler
	{
	public:
		virtual ~PayloadHandler() = default;
		virtual void Process(RE::TESObjectREFR* a_holder, const std::string_view& a_payload, RE::BShkbAnimationGraph* a_animationGraph) = 0;
	};

	namespace API
	{		
		class PayloadHandlerCollector
		{
		public:
			virtual void RegisterPayloadHandler(const char* a_eventTag, PayloadHandler* a_payloadHandler) = 0;
		};

		struct Message
		{
			PayloadHandlerCollector* payloadHandlerCollector;
		};
	}
}

/* API INSTRUCTIONS */
// 
//	1.	Create a class that inherits from `payloadinterpreter::PayloadHandler' to provide a custom implementation of payload handling
//		by defining the `Process' function.
// 
//		class ExamplePayloadHandler : public payloadinterpreter::PayloadHandler
//		{
//		public:
//			static ExamplePayloadHandler* GetSingleton()
//			{
//				static ExamplePayloadHandler singleton;
//				return &singleton;
//			}
// 
//		private:
//			void Process(RE::TESObjectREFR * a_holder, const std::string_view& a_payload, RE::BShkbAnimationGraph* a_animationGraph) final
//			{
//				// ... do stuff with the payload string
//			}
//		};
//
//
//	2.	After calling `SKSE::Init' in `SKSEPlugin_Load', register an SKSE messages listener function. In this function, register a
//		Payload Interpreter messages listener after that plugin is loaded (upon receiving a post load message from SKSE).
//		If Payload Interpreter is not detected, registering the listener will fail on game launch.
//  
//		In `SKSEPlugin_Load':
// 
//		...
//			if (!SKSE::GetMessagingInterface()->RegisterListener("SKSE", SKSEMessageListener))
//			{
//				// ... failure handling
//			}
//		...
// 
//		Also define the SKSE message listener function elsewhere:
// 
//		void SKSEMessageListener(SKSE::MessagingInterface::Message* a_msg)
//		{
//			// If all plugins have been loaded
//			if (a_msg->type == SKSE::MessagingInterface::kPostLoad) {
//				if (SKSE::GetMessagingInterface()->RegisterListener("PayloadInterpreter", PayloadInterpreterMessageListener)) {
//					// ... success handling
//				} else {
//					// ... failure handling
//				}
//			}
//		}
//
//
//	3.	Define the listener function for payload interpreter messages. The received `SKSE::MessagingInterface::Message'
//		will contain a `payloadinterpreter::API::Message' in its "data" field. Cast to access the payload handler
//		collector to register your own payload handler.
//
//		void PayloadInterpreterMessageListener(SKSE::MessagingInterface::Message* a_msg)
//		{
//			if (!a_msg || std::string_view(a_msg->sender) != "PayloadInterpreter") {
//				return;
//			}
//
//			auto payloadInterpreterMsg = static_cast<payloadinterpreter::API::Message*>(a_msg->data);
//
//			if (payloadInterpreterMsg) {
//				payloadInterpreterMsg->payloadHandlerCollector->RegisterPayloadHandler("Example", ExamplePayloadHandler::GetSingleton());
//			}
//		}
