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
    sptr<PersonInfo> person_info;


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

    student->person_info = mksptr(new PersonInfo);
    student->person_info->name = "jx";
    student->person_info->age = 12;
    //student->score->insert(new String{"math"}, new Integer32{ 50 });

    string json_str = JsonSerializer::Serialize(student, true);
    cout << json_str << endl;

    //StudentInfo* newstudent = JsonSerializer::Deserialize<StudentInfo>(json_str);
    //cout << newstudent->ToString() << endl;

}
