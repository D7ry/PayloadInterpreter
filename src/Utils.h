#pragma once
namespace Utils
{
	namespace string_view
	{
		bool hex_str_to_int(std::string_view a_in, int& a_out);
		bool to_int(std::string_view input, int& out);
		bool to_float(std::string_view input, float& out);
		bool to_bool(std::string_view input, bool& out);
		bool to_uint(std::string_view input, uint32_t& out);
	}

	constexpr uint32_t hash(const char* data, size_t const size) noexcept
	{
		uint32_t hash = 5381;

		for (const char* c = data; c < data + size; ++c) {
			hash = ((hash << 5) + hash) + (unsigned char)*c;
		}

		return hash;
	}

	namespace literals
	{
		constexpr uint32_t operator"" _h(const char* str, size_t size) noexcept
		{
			return hash(str, size);
		}
	}

    /*Tokenize a string_view into a vector of string_view.*/
	void splitSV(std::vector<std::string_view>& ret, std::string_view strv, const char delim = '|');

    void SGTM(float a_in);
	
    void damageav(RE::Actor* a, RE::ActorValue av, float val);

    RE::NiPoint3 vectorMatrixMult(const RE::NiPoint3& a_vector, const RE::NiMatrix3& a_matrix);
	
	template <class formType>
	bool lookup(std::string_view a_formIDInHex, std::string_view a_modname, formType*& a_out)
	{
		auto data = RE::TESDataHandler::GetSingleton();
		if (!data) {
			return false;
		}

		int formID = 0;
		
		if (!string_view::hex_str_to_int(a_formIDInHex, formID)) {
			return false;
		}
		
		a_out = data->LookupForm<formType>(formID, a_modname);

		return a_out != nullptr;
	}

};
