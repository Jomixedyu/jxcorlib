/*
* @Moudule     : Formater
* @Date        : 2021/07/14
* @Author      : JomiXedYu
* @Description : This file is part of JxCode.CoreLib : https://github.com/JomiXedYu/JxCode.CoreLib
* @StdRequired : c++11
*/

#ifndef _CORELIB_FORMATER_H
#define _CORELIB_FORMATER_H

#include <string>

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

#endif