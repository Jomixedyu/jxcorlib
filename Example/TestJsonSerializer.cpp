#include <CoreLib.Extension/JsonSerializer.h>
#include <CoreLib//DebugTool.h>
#include <iostream>
#include <format>

using namespace JxCoreLib;
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

    string json_str = JsonSerializer::Serialize(student);
    cout << json_str << endl;

    StudentInfo* newstudent = JsonSerializer::Deserialize<StudentInfo>(json_str);
    cout << newstudent->ToString() << endl;

}
