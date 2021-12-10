#include <CoreLib/File.h>
#include <cassert>
#include <string>

using namespace JxCoreLib;
using namespace std;

void TestFile()
{
    string filename = "C:\\a\\b/c.ext";

    assert(PathUtil::GetFilename(filename) == "c.ext");

    assert(PathUtil::GetFilenameWithoutExt(filename) == "c");

    assert(PathUtil::GetFilenameExt(filename) == ".ext");
}