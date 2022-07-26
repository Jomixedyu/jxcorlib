#include <cstdint>
#include <vector>

class DataSerializer
{
    std::vector<uint8_t> buffer_;
public:
    void WriteBytes(uint8_t* buf, int32_t len)
    {
        if (len <= 0)
        {
            return;
        }
        if (buffer_.size() + len > buffer_.capacity())
        {
            buffer_.reserve(buffer_.size() * 2);
        }
        buffer_.data() + buffer_.size()
        memcpy()
    }
};
DataSerializer& operator<<(DataSerializer& ser, int32_t v)
{
    
    return ser;
}