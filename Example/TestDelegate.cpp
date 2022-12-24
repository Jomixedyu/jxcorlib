#include <CoreLib/Delegate.h>
#include <CoreLib/Object.h>
#include <cassert>
#include <iostream>

using namespace JxCoreLib;

class TestDelgClass : public Object
{
    CORELIB_DEF_TYPE(AssemblyObject_JxCoreLib, TestDelgClass, Object);

public:
    int Get(int i)
    {
        return i * 2;
    }
};

using GetIntDelegate = FunctionDelegate<int, int>;

using Tos = FunctionDelegate<void, int>;

void TestDelegate()
{
    sptr<Tos> deleg0 = Tos::FromRaw([](int i) { std::cout << "delegate test1" << std::endl; });
    deleg0->Invoke(0);

    sptr<GetIntDelegate> deleg = GetIntDelegate::FromRaw([](int i)->int { return i + 1; });
    int result = deleg->Invoke(3);
    assert(result == 4);

    auto inst = mksptr(new TestDelgClass);
    sptr<GetIntDelegate> deleg2 = GetIntDelegate::FromMember(inst, &TestDelgClass::Get);
    int result2 = deleg2->Invoke(3);
    assert(result2 == 6);

    //Object_sp result2 = deleg->DynamicInvoke({ BoxUtil::Box(3) });
    //assert(result2->GetType() == cltypeof<Integer32>());
    //auto result2v = sptr_cast<Integer32>(result2)->get_unboxing_value();
    //assert(result2v == 4);
}