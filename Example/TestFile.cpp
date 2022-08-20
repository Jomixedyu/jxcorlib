#include <CoreLib/File.h>
#include <cassert>
#include <string>
#include <array>
#include <CoreLib.Serialization/DataSerializer.h>

using namespace JxCoreLib;
using namespace JxCoreLib::Serialization;
using namespace std;

void TestFile()
{
    string filename = "C:\\a\\b/c.ext";

    assert(PathUtil::GetFilename(filename) == "c.ext");

    assert(PathUtil::GetFilenameWithoutExt(filename) == "c");

    assert(PathUtil::GetFilenameExt(filename) == ".ext");

    {
        FileStream fs{ "D:/a.txt", FileOpenMode::OpenOrCreate };

        bool is_ser = true;

        int32_t i32 = 257;
        ReadWriteStream(fs, is_ser, i32);

        string str = "jomi";
        ReadWriteStream(fs, is_ser, str);

        std::vector<int> arr = { 2,3,4,8 };
        ReadWriteStream(fs, is_ser, arr);

        std::array<int, 3> ar = { 1 };
        ReadWriteStream(fs, is_ser, ar);

        int32_t oi32;
        string ostr;
        fs.set_position(0);
        ReadWriteStream(fs, !is_ser, oi32);
        ReadWriteStream(fs, !is_ser, ostr);

        std::vector<int> oarr;
        ReadWriteStream(fs, !is_ser, oarr);

        std::array<int, 3> oar;
        ReadWriteStream(fs, !is_ser, oar);

        assert(oi32 == i32);
        assert(str == ostr);
        assert(arr == oarr);
        assert(!memcmp(ar.data(), oar.data(), ar.size()));
    }
    {
        FileStream fs{ "D:/a.txt", FileOpenMode::Read };

        int32_t i32 = 257;
        ReadWriteStream(fs, false, i32);

        assert(257 == i32);
    }

}