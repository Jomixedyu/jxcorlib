#pragma once
#include <unordered_map>
#include <string>
#include <string_view>


struct index_string_block
{
    void* bytes;
    size_t size;
};

struct __index_string_manager final
{
    static auto GetIndexStringMap()
    {
        auto static map = new std::unordered_map<size_t, index_string_block>;
        //96kb
        map->reserve(4096);
        return map;
    }
};

template<typename char_t>
struct basic_index_string final
{
private:
    using index_t = size_t;
    constexpr static index_t none = 0;
private:
    static size_t ELFHash(const char_t* str, size_t size)
    {
        static const size_t TotalBits = sizeof(size_t) * 8;
        static const size_t ThreeQuarters = (TotalBits * 3) / 4;
        static const size_t OneEighth = TotalBits / 8;
        static const size_t HighBits = ((size_t)-1) << (TotalBits - OneEighth);
        size_t hash = 0;
        size_t magic = 0;
        for (size_t i = 0; i < size; ++i)
        {
            const size_t ch = str[i];
            hash = (hash << OneEighth) + ch;
            if ((magic = hash & HighBits) != 0)
            {
                hash ^= (magic >> ThreeQuarters);
                hash &= ~magic;
            }
        }
        return hash;
    }
public:
    basic_index_string() : index(none) {}
    basic_index_string(std::basic_string_view<char_t> view)
    {
        if (view.length() == 0)
        {
            return;
        }

        auto hash = ELFHash(view.data(), view.length());

        auto map = __index_string_manager::GetIndexStringMap();

        auto it = map->find(hash);
        if (it == map->end())
        {
            index_string_block block;

            const size_t view_len = view.length();
            const size_t block_len = view_len + 1;

            block.bytes = new char_t[block_len];
            ::memcpy_s(block.bytes, block_len, view.data(), view_len);

            reinterpret_cast<char_t*>(block.bytes)[view_len] = 0;
            block.size = block_len * sizeof(char_t);

            map->emplace(hash, block);
        }
        else
        {
            const size_t block_len = it->second.size / sizeof(char_t);
            const size_t str_len = block_len - 1;

            bool collision;
            do
            {
                collision = false;
                if (str_len == view.length() && ::memcmp(it->second.bytes, view.data(), str_len * sizeof(char_t)))
                {
                    collision = true;
                    hash = ELFHash(hash);
                }

            } while (collision);
        }
        index = hash;
    }

    static const std::basic_string_view<char_t> get_string(index_t index)
    {
        auto map = __index_string_manager::GetIndexStringMap();
        auto it = map->find(index);
        if (it == map->end())
        {
            return {};
        }
        return std::basic_string_view(reinterpret_cast<char_t*>(it->second.bytes), it->second.size - 1);
    }

    std::basic_string<char_t> to_string() const
    {
        if (index == index_t{})
        {
            return {};
        }
        auto block = __index_string_manager::GetIndexStringMap()->find(index)->second;
        return reinterpret_cast<char_t*>(block.bytes);
    }

    bool operator==(const basic_index_string& str)
    {
        return str.index == this->index;
    }

    bool empty() const { return index == none; }

    index_t index;
};

using index_string = basic_index_string<char>;
