#include <vector>
#include <map>
#include "Object.h"
#include "Type.h"

namespace JxCoreLib
{

    static Object* CreateInstance(const ParameterPackage&)
    {
        return new Object;
    }

    Type* Object::__meta_type()
    {
        static int id = -1;
        if (id == -1) {
            id = Type::Register(CreateInstance, nullptr, _T("JxCoreLib::Object"), typeid(Object), sizeof(Object));
        }
        return Type::GetType(id);
    }

    Type* Object::get_type() const
    {
        return __meta_type();
    }

    void Object::AddManagedChild(Object* child)
    {
        if (child == nullptr)
        {
            return;
        }
        if (this->managed_child_ == nullptr)
        {
            this->managed_child_ = new std::vector<Object*>;
        }
        auto list = this->managed_child_;
        for (auto it = list->rbegin(); it != list->rend(); it++)
        {
            if ((*it) == child)
            {
                return;
            }
        }
        list->push_back(child);
        child->parent_ = this;
    }

    void Object::RemoveManagedChild(Object* child)
    {
        if (child == nullptr || this->managed_child_ == nullptr)
        {
            return;
        }
        auto list = this->managed_child_;
        for (auto it = list->rbegin(); it != list->rend(); it++)
        {
            if ((*it) == child)
            {
                list->erase((++it).base());
                child->parent_ = nullptr;
                return;
            }
        }
    }

    void Object::SetManagedParent(Object* parent)
    {
        if (parent == nullptr)
        {
            //remove
            if (this->parent_ != nullptr)
            {
                this->parent_->RemoveManagedChild(this);
            }
        }
        else
        {
            //add
            if (this->parent_ == parent)
            {
                return;
            }
            if (this->parent_ != nullptr)
            {
                this->parent_->RemoveManagedChild(this);
            }
            parent->AddManagedChild(this);
        }
    }

    Object::~Object()
    {
        if (this->managed_child_ != nullptr)
        {
            for (auto it = this->managed_child_->begin(); it != this->managed_child_->end(); it++)
            {
                delete* it;
            }
            delete this->managed_child_;
        }
    }

    string Object::ToString() const
    {
        return this->get_type()->get_name();
    }

}

std::string std::to_string(JxCoreLib::Object* obj)
{
    return obj->ToString();
}
