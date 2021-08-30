/*
* @Moudule     : Converter
* @Date        : 2021/07/14
* @Author      : JomiXedYu
* @Description : This file is part of JxCode.CoreLib : https://github.com/JomiXedYu/JxCode.CoreLib
* @StdRequired : c++11
*/

#pragma once

#include <string>
#include <cstdint>
#include <algorithm>

namespace std
{
    template<typename T>
    concept is_iteratable = requires { typename T::iterator; };

    template<is_iteratable T>
    inline string to_string(const T& arr)
    {
        string s;
        s.append("[");
        for (auto it = arr.begin(); it != arr.end(); it++)
        {
            s.append(to_string(*it));
            if (it != --arr.end())
            {
                s.append(", ");
            }
        }
        s.append("]");
        return s;
    }

}

namespace cvt
{
    using std::to_string;

    int32_t to_int32(const std::string& str);
    float to_float32(const std::string& str);
    double to_double64(const std::string& str);
    bool to_bool(const std::string& str);


    //inline
    inline int32_t to_int32(const std::string& str)
    {
        return std::atoi(str.c_str());
    }
    inline float to_float32(const std::string& str)
    {
        return (float)std::atof(str.c_str());
    }
    inline double to_double64(const std::string& str)
    {
        return std::atof(str.c_str());
    }
    inline bool to_bool(const std::string& str)
    {
        std::string nstr;
        std::transform(str.begin(), str.end(), nstr.begin(), std::tolower);
        return nstr == "true";
    }
}

namespace JxCoreLib
{
    using namespace cvt;
}
