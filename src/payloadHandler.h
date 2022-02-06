#pragma once
using namespace std;
/*bunch of regex to match payload input.*/
struct apf_regex
{
	//GV == graph variable
	inline static const regex setGVBool = regex("");
	inline static const regex setGVFloat = regex("");
	inline static const regex setGVInt = regex("");

	inline static const regex castSpell = regex("");
};
class payloadHandler
{
public:
	/*process a payload message. Does not include the identifier.
	@param a_actor the actor whose animation triggers the payload.
	@param a_payload the payload body. */
	static void processPayload(RE::Actor* actor, string payload) {
	};
};