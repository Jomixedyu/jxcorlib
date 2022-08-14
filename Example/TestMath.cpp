#include <CoreLib.Math/Math.h>
#include <cassert>
#include <iostream>

using namespace JxCoreLib::Math;
using namespace std;

void TestMath()
{
    Vector3f v{ 3,3,3 };
    Quat4f(Vector3f{});
    cout << to_string(Quat4f(-1, 0, 0.023, -0.008).GetEuler()) << endl;
    cout << to_string(Vector3f(0.020, -2.591, 0.864)) << endl;

    assert(Quat4f(-1, 0, 0.023, -0.008).GetEuler() == Vector3f(0.020, -2.591, 0.864));
}