#pragma once
namespace Utils
{

    inline auto strViewSplit(std::string_view in, char sep) {
        std::vector<std::string_view> r;
        //r.reserve(std::count(in.begin(), in.end(), sep) + 1); // optional
        for (auto p = in.begin();; ++p) {
            auto q = p;
            p = std::find(p, in.end(), sep);
            r.emplace_back(q, p);
            if (p == in.end())
                return r;
        }
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
}