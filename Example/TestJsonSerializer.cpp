#include <CoreLib.Serializer/JsonSerializer.h>
#include <CoreLib/DebugTool.hpp>
#include <iostream>
#include <format>
#include <map>

using namespace JxCoreLib;
using namespace JxCoreLib::Serializer;
using namespace std;


class PersonInfo : public Object
{
    CORELIB_DEF_TYPE(PersonInfo, Object);
public:
    CORELIB_REFL_DECL_FIELD(name);
    string name;
    CORELIB_REFL_DECL_FIELD(age);
    int age;
    virtual string ToString() const override
    {
        return std::format("name: {}, age: {}", name, age);
    }
};

class StudentInfo : public Object
{
    CORELIB_DEF_TYPE(StudentInfo, Object);
public:

    CORELIB_REFL_DECL_FIELD(id);
    int id;
    CORELIB_REFL_DECL_FIELD(president);
    bool president;
    CORELIB_REFL_DECL_FIELD(person_info);
    PersonInfo* person_info;

    //CORELIB_REFL_DECL_FIELD(score);
    static inline struct __corelib_refl_score
    {
        template<typename T> using _Detected = decltype(std::declval<T&>().score);
        __corelib_refl_score()
        {
            //using _Ty = decltype(std::declval<__corelib_curclass&>().score);
            //using _Fuldecay = fulldecay<_Ty>::type;
            //using cc = std::remove_cv<_Ty>::type;
            //using dd = get_cltype<cc>::type;
            //using _CTy = get_cltype<std::remove_cv<_Ty>::type>::type;
            //using _TyOncePtr = get_cltype<_Fuldecay>::type*;
            //ReflectionBuilder::CreateFieldInfo<__corelib_curclass, _Ty>(
            //    "score", false, JxCoreLib::is_detected<_Detected, __corelib_curclass>::value,
            //    [](Object* p) -> Object* {
            //        return get_object_pointer<_Fuldecay>::get(((__corelib_curclass*)p)->score);
            //    },
            //    [](Object* p, Object* value) {
            //        auto dp = const_cast<std::remove_cv<_Ty>::type*>(&((__corelib_curclass*)p)->score);
            //        _TyOncePtr obj = static_cast<_TyOncePtr>(value);
            //        *dp = *find_pointer_if<_CTy, !std::is_pointer<_Ty>::value>::get(&obj);
            //    });
        }
    } __corelib_refl_score_;

    //ManagedMap<String*, Integer32*>* score;

    virtual string ToString() const override
    {
        return std::format("id: {}, president: {}, person_info: {{{}}}", id, president, person_info->ToString());
    }
};

void TestJsonSerializer()
{
    StudentInfo* student = new StudentInfo;
    student->id = 33;
    student->president = true;

    student->person_info = new PersonInfo;
    student->person_info->name = "jx";
    student->person_info->age = 12;
    //student->score->insert(new String{"math"}, new Integer32{ 50 });

    string json_str = JsonSerializer::Serialize(student, true);
    cout << json_str << endl;

    //StudentInfo* newstudent = JsonSerializer::Deserialize<StudentInfo>(json_str);
    //cout << newstudent->ToString() << endl;

}
