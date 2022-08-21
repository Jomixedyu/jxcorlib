#include "../../Window.h"
#include <Windows.h>

namespace JxCoreLib::Platform::Window
{
    using std::string;

    static UINT _MappingMsgMode(MessageBoxMode mode)
    {
        switch (mode)
        {
        case JxCoreLib::Platform::Window::MessageBoxMode::YesNo: return MB_YESNO;
        case JxCoreLib::Platform::Window::MessageBoxMode::Ok: return MB_OK;
        case JxCoreLib::Platform::Window::MessageBoxMode::OkCancel: return MB_OKCANCEL;
        case JxCoreLib::Platform::Window::MessageBoxMode::YesNoCancel: return MB_YESNOCANCEL;
        default:
            break;
        }
        return MB_OK;
    }
    static string Utf8ToGbk(const char* src_str)
    {
        int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
        wchar_t* wszGBK = new wchar_t[len + 1];
        memset(wszGBK, 0, len * 2 + 2);
        MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
        len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
        char* szGBK = new char[len + 1];
        memset(szGBK, 0, len + 1);
        WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
        string strTemp(szGBK);
        if (wszGBK) delete[] wszGBK;
        if (szGBK) delete[] szGBK;
        return strTemp;
    }
    static MessageBoxResult _MappingMsgResult(int ret)
    {
        switch (ret)
        {
        case IDCANCEL: return MessageBoxResult::Cancel;
        case IDNO: return MessageBoxResult::No;
        case IDOK: return MessageBoxResult::Ok;
        case IDYES: return MessageBoxResult::Yes;
        default:
            break;
        }
        return MessageBoxResult::Cancel;
    }

    static MessageBoxResult _MessageBoxImpl(std::string_view text, std::string_view title, MessageBoxMode mode)
    {
        auto ret = MessageBox(nullptr, Utf8ToGbk(text.data()).data(), Utf8ToGbk(title.data()).data(), _MappingMsgMode(mode));
        return _MappingMsgResult(ret);
    }

#undef MessageBox



    MessageBoxResult MessageBox(std::string_view text, std::string_view title, MessageBoxMode mode)
    {
        return _MessageBoxImpl(text, title, mode);
    }
}