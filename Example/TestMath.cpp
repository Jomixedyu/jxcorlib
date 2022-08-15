#include <CoreLib.Math/Math.h>
#include <cassert>
#include <iostream>

using namespace JxCoreLib::Math;
using namespace std;

void TestMath()
{
    Vector3f v{ 3,3,3 };
    Quat4f(Vector3f{});
    cout << to_string(Quat4f(1, 0, 0, 0)) << endl;
    cout << to_string(Quat4f(1, 0, 0, 0).GetEuler()) << endl;

    auto q = Quat4f(0.224, 0.827, -0.514, -0.038);
    auto qe = q.GetEuler();
    auto q2 = Quat4f(qe);
    cout << to_string(q ) << endl;
    cout << to_string(qe ) << endl;
    cout << to_string(q2 ) << endl;

    //cout << to_string(Quat4f(0.023, 0.003, 0.948, -0.316).GetEuler()) << endl;

    //assert(Quat4f(-1, 0, 0.023, -0.008).GetEuler() == Vector3f(0.020, -2.591, 0.864));
}