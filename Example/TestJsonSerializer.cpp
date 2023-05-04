#include <CoreLib.Serialization/JsonSerializer.h>
#include "Assembly.h"
#include <iostream>
//#include <format>
#include <map>
#include <CoreLib/Converter.hpp>
#include <CoreLib/Enum.h>
#include <CoreLib/Guid.h>
#include <CoreLib.Math/Math.h>

using namespace jxcorlib;
using namespace jxcorlib::ser;
using namespace jxcorlib::math;
using namespace std;


class PersonInfo : public Object
{
    CORELIB_DEF_TYPE(AssemblyObject_Test, PersonInfo, Object);
public:

    CORELIB_REFL_DECL_FIELD(name);
    string name;
    CORELIB_REFL_DECL_FIELD(age);
    int age;
    virtual string ToString() const override
    {
        return StringUtil::Concat("name: ", name, ", age: ", to_string(age));
        //return std::format("name: {}, age: {}", name, age);
    }
};

CORELIB_DEF_ENUM(AssemblyObject_Test, , 
    StudentLevel,
    A, B, C
    );

CORELIB_DECL_BOXING_ENUM(StudentLevel);

class StudentInfo : public Object
{
    CORELIB_DEF_TYPE(AssemblyObject_Test, StudentInfo, Object);
public:

    CORELIB_REFL_DECL_FIELD(id);
    int id;

    CORELIB_REFL_DECL_FIELD(is_exist);
    bool is_exist;

    CORELIB_REFL_DECL_FIELD(level);
    StudentLevel level;

    CORELIB_REFL_DECL_FIELD(person_info);
    sptr<PersonInfo> person_info;

    CORELIB_REFL_DECL_FIELD(score);
    List_sp<int> score;

    CORELIB_REFL_DECL_FIELD(guid)
    guid_t guid;

    CORELIB_REFL_DECL_FIELD(vec3);
    Vector3f vec3;

    virtual string ToString() const override
    {
        //return std::format("id: {}, exist: {}, level: {}, guid: {}, person_info: {{{}}}, score: {}, vec3: {}", id, is_exist, BoxingStudentLevel::StaticFindName(level), guid.to_string(), person_info->ToString(), jxcvt::to_string(*score), to_string(vec3));
        return {};
    }
};

void TestJsonSerializer()
{

    StudentInfo* student = new StudentInfo;
    student->id = 33;
    student->is_exist = true;
    student->level = StudentLevel::B;
    student->guid = guid_t::create_new();
    student->score = mksptr(new List<int>());
    student->score->push_back(3);
    student->score->push_back(4);
    student->vec3 = { 2,3,8 };

    student->person_info = mksptr(new PersonInfo);
    student->person_info->name = "jx";
    student->person_info->age = 12;

    string json_str = JsonSerializer::Serialize(student, {4, false});
    cout << json_str << endl;

    sptr<StudentInfo> newstudent = JsonSerializer::Deserialize<StudentInfo>(json_str);
    cout << newstudent->ToString() << endl;

    
}
