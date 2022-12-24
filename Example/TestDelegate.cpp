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

static int g_delegate_check = -1;

void TestDelegate()
{
    //raw function
    {
        auto deleg = FunctionDelegate<int>::FromRaw([]() -> int { return 1; });
        int invoke_ret = deleg->Invoke();
        assert(invoke_ret == 1);

        auto dyninvoke_ret = deleg->DynamicInvoke({ });
        assert(UnboxUtil::Unbox<int>(dyninvoke_ret) == 1);
    }
    {
        auto deleg = FunctionDelegate<void, int>::FromRaw([](int i) { g_delegate_check = i; });
        deleg->Invoke(3);
        assert(g_delegate_check == 3);
        g_delegate_check = -1;

        auto dyninvoke_ret = deleg->DynamicInvoke({ mkbox(3) });
        assert(g_delegate_check == 3);
        g_delegate_check = -1;
    }

    //member
    {

    }
    //auto inst = mksptr(new TestDelgClass);
    //sptr<GetIntDelegate> deleg2 = GetIntDelegate::FromMember(inst, &TestDelgClass::Get);
    //int result2 = deleg2->Invoke(3);
    //assert(result2 == 6);


}