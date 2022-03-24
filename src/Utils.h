#pragma once
class Utils
{
public:
    /*Tokenize a string_view into a vector of string_view.*/
    static std::vector<std::string_view> tokenize(const std::string_view str, const char delim = '|')
    {
        std::vector<std::string_view> result;

        int indexCommaToLeftOfColumn = 0;
        int indexCommaToRightOfColumn = -1;

        for (int i = 0; i < static_cast<int>(str.size()); i++)
        {
            if (str[i] == delim)
            {
                indexCommaToLeftOfColumn = indexCommaToRightOfColumn;
                indexCommaToRightOfColumn = i;
                int index = indexCommaToLeftOfColumn + 1;
                int length = indexCommaToRightOfColumn - index;
                std::string_view column(str.data() + index, length);
                result.push_back(column);
            }
        }
        const std::string_view finalColumn(str.data() + indexCommaToRightOfColumn + 1, str.size() - indexCommaToRightOfColumn - 1);
        result.push_back(finalColumn);
        return result;
    }

    /*Tokenize a string into vectors, separated by a common delimiter.
    @param s: string to be tokenized.
    @param delimiter: the delimiter based on which to tokenize the string.
    @return a vector containing all tokens.*/
    static std::vector<std::string> tokenize(std::string s, const char delimiter = '|')
    {
        size_t start = 0;
        size_t end = s.find_first_of(delimiter);

        std::vector<std::string> output;

        while (end <= std::string::npos)
        {
            std::string ss = s.substr(start, end - start);

            if (ss.size() != 0) { //if token has 0 size, skip it.
                output.emplace_back(ss);
            }
            

            if (end == std::string::npos)
                break;

            start = end + 1;
            end = s.find_first_of(delimiter, start);
        }

        return output;
    }

    /*Fetch brackted contents.
    sample input: ![a_float][a_string]
    assuming input is in correct format.*/
    static std::pair<float, std::string> fetchBracketed(std::string in) {
        size_t start = in.find_first_of('[');
        size_t end = in.find_first_of(']');
        float f = std::stof(in.substr(start, end - start));
        start = in.find_first_of('[', start);
        end = in.find_first_of(']', end);
        std::string s = in.substr(start, end);
        return std::pair{ f, s };
    }

    static void SGTM(float a_in) {
        static float* g_SGTM = (float*)REL::ID(511883).address();
        *g_SGTM = a_in;
        using func_t = decltype(SGTM);
        REL::Relocation<func_t> func{ REL::ID(66989) };
        return;
    }

    static bool ToInt(std::string str, int& value)
    {
        const char* strVal = str.c_str();
        char* endVal = NULL;
        long ret = strtol(strVal, &endVal, 0);
        if (ret == LONG_MAX || ret == LONG_MIN || (int)endVal != (int)strVal + strlen(strVal))
            return false;
        value = ret;
        return true;
    }

    static void damageav(RE::Actor* a, RE::ActorValue av, float val)
    {
        if (a) {
            a->As<RE::ActorValueOwner>()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, av, -val);
        }
    }

    static RE::NiPoint3 vectorMatrixMult(const RE::NiPoint3& a_vector, const RE::NiMatrix3& a_matrix)
    {
        return RE::NiPoint3(a_matrix.entry[0][0] * a_vector.x + a_matrix.entry[0][1] * a_vector.y + a_matrix.entry[0][2] * a_vector.z,
            a_matrix.entry[1][0] * a_vector.x + a_matrix.entry[1][1] * a_vector.y + a_matrix.entry[1][2] * a_vector.z,
            a_matrix.entry[2][0] * a_vector.x + a_matrix.entry[2][1] * a_vector.y + a_matrix.entry[2][2] * a_vector.z);
    }


};