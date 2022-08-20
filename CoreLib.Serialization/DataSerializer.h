#include <cstdint>
#include <vector>
#include <iostream>
#include <string>
#include <string_view>

namespace JxCoreLib::Serialization
{
    class Stream
    {
    protected:
        int32_t position_;
    public:
        int32_t get_position() { return position_; }
    public:
        virtual bool IsEOF() = 0;
        virtual int32_t WriteByte(uint8_t value) = 0;
        virtual int32_t WriteBytes(uint8_t* arr, int32_t offset, int32_t count) = 0;
        virtual int32_t ReadByte(uint8_t* out_byte) = 0;
        virtual int32_t ReadBytes(uint8_t* arr, int32_t offset, int32_t count) = 0;
        int32_t WriteReadBytes(uint8_t* arr, int32_t offset, int32_t count, bool isread)
        {
            if (isread)
                return ReadBytes(arr, offset, count);
            else
                return WriteBytes(arr, offset, count);
        }
        virtual void Flush() = 0;
        virtual ~Stream() { }
    };

    class FileStream : Stream
    {
    protected:
        void* file_;
    public:
        FileStream(std::string_view filename);
        virtual bool IsEOF() = 0;
        virtual int32_t WriteByte(uint8_t value) override;
        virtual int32_t WriteBytes(uint8_t* arr, int32_t offset, int32_t count)  override;
        virtual int32_t ReadByte(uint8_t* out_byte) override;
        virtual int32_t ReadBytes(uint8_t* arr, int32_t offset, int32_t count)  override;
        virtual void Flush() override;
        virtual ~FileStream() override;
        FileStream(const FileStream&) = delete;
        FileStream(FileStream&& r);
    };

    Stream& ReadWriteStream(Stream& stream, bool is_ser, uint8_t& out);
    Stream& ReadWriteStream(Stream& stream, bool is_ser, int16_t& out);
    Stream& ReadWriteStream(Stream& stream, bool is_ser, int32_t& out);
    Stream& ReadWriteStream(Stream& stream, bool is_ser, int64_t& out);
    Stream& ReadWriteStream(Stream& stream, bool is_ser, float& out);
    Stream& ReadWriteStream(Stream& stream, bool is_ser, double& out);
    Stream& ReadWriteStream(Stream& stream, bool is_ser, std::string& out);

}