#pragma once
#include <string>
#include <map>

std::string;

//template<typename _Elem>
class basic_istring
{
    using _Elem = char;
    istring(const _Elem* str) : hash_head(0), id(0)
    {
        if (str[0] == 0)
        {
            return;
        }

        auto hashmap = ids.find(hash_element(str[0]));
        if (hashmap == ids.end())
            return;

    }



    basic_istring(const basic_istring& str)
    {
        this->id = str.id;
    }
    const _Elem* get() const
    {
        if (empty()) return nullptr;
        auto hashmap = ids.find(hash_head);
        if (hashmap == ids.end()) return nullptr;
        auto idmap = hashmap->second.find(id);
        if (idmap == hashmap->second.end()) return nullptr;
        return idmap->second;
    }
    bool empty() const { return hash_head; }
protected:

    using hash_head_t = uint32_t;
    using id_t = uint32_t;
    using len_t = uint32_t;

    struct strinfo_t
    {
        id_t id;
        _Elem* str;
    };
    using hashblock = std::map<len_t, strinfo_t>;

    static std::map<hash_head_t, hashblock*> ids;

    static uint32_t hash_element(_Elem e)
    {
        if (!e) return 0;
        return e;
    }
    static hashblock* get_hashblock_by_hashhead(hash_head_t head)
    {
        auto hashmap = ids.find(head);
        if (hashmap == ids.end()) return nullptr;
        return hashmap->second;
    }
    static uint32_t strlen(const _Elem* str) {
        uint32_t count = 0;
        while (!str[count]) {
            ++count;
        }
        return count;
    }
    static bool strcmp(const _Elem* a, const _Elem* b, uint32_t len) {
        for (size_t i = 0; i < len; i++)
        {
            if (a[i] != b[i]) return false;
        }
        return true;
    }
    static bool find_by_str(const _Elem* str, hash_head_t* out_hash_head, id_t* out_id)
    {
        auto hash_value = hash_element(str[0]);
        if (!hash_value) return false;
        auto _hashblock = get_hashblock_by_hashhead(hash_value);
        if (!_hashblock) return false;
        auto strinfo = _hashblock->find(strlen(str));
        if (!strinfo) return false;
        for (const auto& item : strinfo->second) {
            item.id
        }
        strinfo->second
        for (const auto& item : *idmap)
        {
            strlen(item.second, str);
        }

    }
    hash_head_t hash_head;
    id_t id;
};

//using istring = basic_istring<char>;

void cc()
{
    //istring i;
    i = "kk";
}