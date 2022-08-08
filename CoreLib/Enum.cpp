#include "Enum.h"

namespace JxCoreLib
{
    void Enum::InitDefinitions(const char* datas, DataMap** defs)
    {
        if (*defs) return;
        *defs = new DataMap;
        auto enums = StringUtil::Split(datas, u8char(","));
        for (string& item : enums) {
            auto kv = StringUtil::Split(item, u8char("="));
            string& name_ = kv[0];
            string& enum_ = kv[1];
            StringUtil::TrimSelf(name_);
            StringUtil::TrimSelf(enum_);
            (*defs)->insert({ name_ , (uint32_t)std::stoul(enum_) });
        }
    }
    uint32_t Enum::StaticParse(Type* type, string_view name)
    {
        return uint32_t();
    }
}