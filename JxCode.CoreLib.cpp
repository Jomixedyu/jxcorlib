
#include "CoreLib/CoreLib.h"
#include "CoreLib/DebugTool.hpp"
#include <format>
#include <array>
#include <iostream>
#include <string>
#include <cassert>

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

namespace JxCoreLib
{

    class Enum : public Object
    {
        CORELIB_DEF_TYPE(AssemblyObject_JxCoreLib, JxCoreLib::Enum, Object);
    protected:
        using DataMap = std::map<string, uint32_t>;
        static void InitDefinitions(const char* datas, DataMap* defs)
        {
            if (defs) return;
            defs = new DataMap;
            auto enums = StringUtil::Split(datas, ",");
            for (string& item : enums) {
                auto kv = StringUtil::Split(item, "=");
                string& name_ = kv[0];
                string& enum_ = kv[1];
                StringUtil::TrimSelf(name_);
                StringUtil::TrimSelf(enum_);
                defs->insert({ name_ , (uint32_t)std::stoul(enum_) });
            }
        }
    public:

        uint32_t GetValue() { return this->value_; }
        virtual string GetName() const = 0;

        static uint32_t Parse(Type* type, string_view name);

    protected:
        uint32_t value_;
    };
}

#define CORELIB_DEF_ENUM(Assembly, FullName, Name, ...) \
enum class Name : uint32_t \
{ \
 __VA_ARGS__ \
}; \
inline struct __corelib_enum_##Assembly##_##FullName { \
__corelib_enum_##Assembly##_##FullName() { ::JxCoreLib::Enum::StaticRegisterEnum(#__VA_ARGS__); } \
} __corelib_enum_##Assembly##_##FullName##_; \
template<> struct get_boxing_type<Mode> { using type = ::JxCoreLib::Enum; };



enum class Mode { A };

class BoxingMode final : public Enum
{
    CORELIB_DEF_TYPE(AssemblyObject_JxCoreLib, Mode, Enum);

    static inline DataMap* definitions = nullptr;
    static inline const char* string_definitions ;
    static void init_defs() { InitDefinitions(string_definitions, definitions); }
public:
    static string GetName(Mode value) {
        init_defs();
        for (auto& [name, enum_value] : *definitions) {
            if (enum_value == static_cast<uint32_t>(value)) return name;
        }
    }

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
