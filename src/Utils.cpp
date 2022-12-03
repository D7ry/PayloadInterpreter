#include "Utils.h"


/*Tokenize a string_view into a vector of string_view.*/
void Utils::splitSV(std::vector<std::string_view>& ret, std::string_view strv, const char delim)
{
	size_t first = 0;

	while (first < strv.size()) {
		const auto second = strv.find_first_of(delim, first);

		if (first != second)
			ret.emplace_back(strv.substr(first, second - first));

		if (second == std::string_view::npos)
			break;

		first = second + 1;
	}

}

void Utils::SGTM(float a_in)
{
	static float* g_SGTM = (float*)RELOCATION_ID(511883, 388443).address();
	*g_SGTM = a_in;
	using func_t = decltype(SGTM);
	REL::Relocation<func_t> func{ RELOCATION_ID(66989, 0) };
	return;
}

	
/*Converts a string view into an int, safe for none null-terminated string views. Returns false if conversion isn't successful.*/
bool Utils::string_view::to_int(std::string_view input, int& out)
{
	logger::info("to_int for {}", input);
	const std::from_chars_result res = std::from_chars(input.data(), input.data() + input.size(), out);
	if (res.ec == std::errc::invalid_argument || res.ec == std::errc::result_out_of_range) {
		return false;
	}
	return true;
}

inline long long hextoint(char x)
{
	int b = uint8_t(x);
	int maskLetter = (('9' - b) >> 31);
	int maskSmall = (('Z' - b) >> 31);
	int offset = '0' + (maskLetter & int('A' - '0' - 10)) + (maskSmall & int('a' - 'A'));
	return b - offset;
}

/*Convert a hex value beginning with "0x" to int. Works for none-null terminated string views.*/
bool Utils::string_view::hex_str_to_int(std::string_view a_in, int& a_out)
{
	if (a_in[0] != '0' ||(a_in[1] != 'x' && a_in[1] != 'X')) {
		return false;
	}
	int i = 1;
	//iterate through the string view from right to left, adding the corresponding hex values
	for (int j = a_in.length() - 1; j > 1; j--) {
		a_out += hextoint(a_in[j]) * i;
		i *= 16;
	}
	return true;
}

bool Utils::string_view::to_float(std::string_view input, float& out)
{
	const std::from_chars_result res = std::from_chars(input.data(), input.data() + input.size(), out);

	if (res.ec == std::errc::invalid_argument || res.ec == std::errc::result_out_of_range) {
		return false;
	}
	return true;
}

bool Utils::string_view::to_bool(std::string_view input, bool& out)
{
	switch (input[0]) {
	case '1':
		out = true;
		return true;
	case '0':
		out = false;
		return true;
	default:
		return false;
	}
}

bool Utils::string_view::to_uint(std::string_view input, uint32_t& out)
{
	const std::from_chars_result res = std::from_chars(input.data(), input.data() + input.size(), out);
	if (res.ec == std::errc::invalid_argument || res.ec == std::errc::result_out_of_range) {
		return false;
	}
	return true;
}

void Utils::damageav(RE::Actor* a, RE::ActorValue av, float val)
{
	if (a) {
		a->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, av, -val);
	}
}

RE::NiPoint3 Utils::vectorMatrixMult(const RE::NiPoint3& a_vector, const RE::NiMatrix3& a_matrix)
{
	return RE::NiPoint3(a_matrix.entry[0][0] * a_vector.x + a_matrix.entry[0][1] * a_vector.y + a_matrix.entry[0][2] * a_vector.z,
		a_matrix.entry[1][0] * a_vector.x + a_matrix.entry[1][1] * a_vector.y + a_matrix.entry[1][2] * a_vector.z,
		a_matrix.entry[2][0] * a_vector.x + a_matrix.entry[2][1] * a_vector.y + a_matrix.entry[2][2] * a_vector.z);
}
