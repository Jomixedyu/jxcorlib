
#include "CoreLib/CoreLib.h"
#include "CoreLib/DebugTool.hpp"
#include <format>
#include <array>
#include <iostream>
#include <string>
#include <cassert>
#include <CoreLib/UString.h>

using namespace std;
using namespace JxCoreLib;

extern void TestOOP();
extern void TestProperty();
extern void TestString();
extern void TestEvents();
extern void TestReflection();
extern void TestJsonSerializer();
extern void TestDynCreateInst();
extern void TestFormater();
extern void TestFile();



#define CORELIB_DEF_ENUM(Assembly, FullName, Name, ...) \
enum class Name : uint32_t \
{ \
 __VA_ARGS__ \
}; \
inline struct __corelib_enum_##Assembly##_##FullName { \
__corelib_enum_##Assembly##_##FullName() { ::JxCoreLib::Enum::StaticRegisterEnum(#__VA_ARGS__); } \
} __corelib_enum_##Assembly##_##FullName##_; \
template<> struct get_boxing_type<Mode> { using type = ::JxCoreLib::Enum; };



enum class Mode { A = 1, B = 2 };

class BoxingMode final : public Enum
{
    CORELIB_DEF_TYPE(AssemblyObject_JxCoreLib, BoxingMode, Enum);

    static inline DataMap* definitions = nullptr;
    static inline const char* string_definitions = "A = 1, B = 2";
    static void init_defs() { InitDefinitions(string_definitions, &definitions); }
public:
    using unboxing_type = Mode;
    static const DataMap* StaticGetDefinitions() {
        init_defs();
        return definitions;
    }
    static string StaticFindName(Mode value) {
        init_defs();
        for (auto& [name, enum_value] : *definitions) {
            if (enum_value == static_cast<uint32_t>(value)) return name;
        }
        return {};
    }
    string GetName() const override {
        return StaticFindName(static_cast<Mode>(this->value_));
    }
    BoxingMode& operator=(Mode value) {
        this->value_ = static_cast<uint32_t>(value);
    }
    BoxingMode(Mode value) : base(static_cast<uint32_t>(value)) { }
    BoxingMode() : base() {  }
};

int main()
{
    using namespace std;

    TestOOP();
    TestProperty();
    TestString();
    TestEvents();
    TestReflection();
    TestJsonSerializer();
    TestDynCreateInst();
    TestFormater();
    //TestFile();

    BoxingMode m = Mode::B;
    cout << m.GetName() << endl;;

    sptr<List<String_sp>> intlist = mksptr(new List<String_sp>);
    //interface
    auto list = sptr_cast<IList>(intlist);

    list->Add(BoxUtil<string>::Box("aaa"));

    for (auto item : *intlist)
    {
        cout << item << endl;
    }

    cout << PathUtil::Combine("asd/", "/vasf") << endl;

    assert(intlist->GetType()->IsImplementedInterface(cltypeof<IList>()));

    return 0;
}
