#ifndef _CORELIB_MANAGEDVECTOR_H
#define _CORELIB_MANAGEDVECTOR_H

#include "Object.h"
#include "Type.h"
#include <vector>

namespace JxCoreLib
{
    template<cltype_concept T>
    class ManagedVector : public Object, public std::vector<T>
    {
        //TODO: Template Macro
    public:
        using vector_type = std::vector<T>;
        using vector_type::vector_type;

        T take_at(const size_t& pos)
        {
            T t = this->at(pos);
            this->erase(this->begin() + pos);
            return t;
        }
        void remove_at_and_delete(const size_t& pos)
        {
            auto it = (this->begin() + pos);
            delete (*it);
            this->erase(it);
        }
        void remove_and_delete(const T& value)
        {
            auto it = std::find(this->begin(), this->end(), value);
            delete (*it);
            this->erase(it);
        }
        void clear_and_delete()
        {
            this->delete_all();
            this->clear();
        }
    private:
        void delete_all()
        {
            for (const auto& item : *this)
            {
                delete item;
            }
        }
    public:
        virtual ~ManagedVector()
        {
            this->delete_all();
        }
    };

}

#endif