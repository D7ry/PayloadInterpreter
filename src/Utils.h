#pragma once
namespace Utils
{
    /*Tokenize a string_view into a vector of string_view.*/
    inline std::vector<std::string_view> tokenize(const std::string_view str, const char delim = '|')
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

    inline std::vector<std::string> splitString(std::string s, const char delimiter)
    {
        size_t start = 0;
        size_t end = s.find_first_of(delimiter);

        std::vector<std::string> output;

        while (end <= std::string::npos)
        {
            output.emplace_back(s.substr(start, end - start));

            if (end == std::string::npos)
                break;

            start = end + 1;
            end = s.find_first_of(delimiter, start);
        }

        return output;
    }

    inline void SGTM(float a_in) {
        static float* g_SGTM = (float*)REL::ID(511883).address();
        *g_SGTM = a_in;
        using func_t = decltype(SGTM);
        REL::Relocation<func_t> func{ REL::ID(66989) };
        return;
    }

    inline bool ToInt(std::string str, int& value)
    {
        const char* strVal = str.c_str();
        char* endVal = NULL;
        long ret = strtol(strVal, &endVal, 0);
        if (ret == LONG_MAX || ret == LONG_MIN || (int)endVal != (int)strVal + strlen(strVal))
            return false;
        value = ret;
        return true;
    }

    inline void damageav(RE::Actor* a, RE::ActorValue av, float val)
    {
        if (a) {
            a->As<RE::ActorValueOwner>()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, av, -val);
        }
        DEBUG("{}'s {} damaged to {}",
            a->GetName(),
            av,
            a->GetActorValue(av));
    }

}