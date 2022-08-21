#pragma once
#include <string>
#include <string_view>
#include <cstdint>

namespace JxCoreLib::Platform
{
    namespace Window
    {
        
        enum class MessageBoxMode : uint8_t
        {
            YesNo,
            Ok,
            OkCancel,
            YesNoCancel
        };
        enum class MessageBoxResult : uint8_t
        {
            Ok,
            Cancel,
            Yes,
            No,
        };
        MessageBoxResult MessageBox(std::string_view text, std::string_view title, MessageBoxMode mode);
    }
}