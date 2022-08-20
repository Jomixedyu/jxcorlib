#include "DataSerializer.h"


namespace JxCoreLib::Serialization
{
    bool FileStream::IsEOF()
    {
        return (bool)feof((FILE*)this->file_);
    }
    FileStream::FileStream(std::string_view filename)
    {
        this->file_ = (void*)fopen(filename.data(), "rw+");
    }
    int32_t FileStream::WriteByte(uint8_t value)
    {
        return this->WriteBytes(&value, 0, 1);
    }
    int32_t FileStream::WriteBytes(uint8_t* arr, int32_t offset, int32_t count)
    {
        return (int32_t)fwrite(arr + offset, sizeof(uint8_t), count, (FILE*)this->file_);
    }
    int32_t FileStream::ReadByte(uint8_t* out_byte)
    {
        return this->ReadBytes(out_byte, 0, 1);
    }
    int32_t FileStream::ReadBytes(uint8_t* arr, int32_t offset, int32_t count)
    {
        auto len = fread(arr + offset, sizeof(uint8_t), count, (FILE*)this->file_);
        return len == EOF ? -1 : len;
    }
    void FileStream::Flush()
    {
        fflush((FILE*)this->file_);
    }
    FileStream::~FileStream()
    {
        if (this->file_)
        {
            fclose((FILE*)this->file_);
            this->file_ = nullptr;
        }
    }
    FileStream::FileStream(FileStream&& r)
    {
        this->file_ = r.file_;
        r.file_ = nullptr;
    }


    Stream& ReadWriteStream(Stream& stream, bool is_ser, uint8_t& out)
    {
        stream.WriteReadBytes((uint8_t*)(&out), 0, sizeof(out), !is_ser);
        return stream;
    }
    Stream& ReadWriteStream(Stream& stream, bool is_ser, int16_t& out)
    {
        stream.WriteReadBytes((uint8_t*)(&out), 0, sizeof(out), !is_ser);
        return stream;
    }
    Stream& ReadWriteStream(Stream& stream, bool is_ser, int32_t& out)
    {
        stream.WriteReadBytes((uint8_t*)(&out), 0, sizeof(out), !is_ser);
        return stream;
    }
    Stream& ReadWriteStream(Stream& stream, bool is_ser, int64_t& out)
    {
        stream.WriteReadBytes((uint8_t*)(&out), 0, sizeof(out), !is_ser);
        return stream;
    }
    Stream& ReadWriteStream(Stream& stream, bool is_ser, float& out)
    {
        stream.WriteReadBytes((uint8_t*)(&out), 0, sizeof(out), !is_ser);
        return stream;
    }
    Stream& ReadWriteStream(Stream& stream, bool is_ser, double& out)
    {
        stream.WriteReadBytes((uint8_t*)(&out), 0, sizeof(out), !is_ser);
        return stream;
    }
    Stream& ReadWriteStream(Stream& stream, bool is_ser, std::string& out)
    {
        int32_t len = out.length();
        ReadWriteStream(stream, is_ser, len);
        if (is_ser)
        {
            stream.WriteBytes((uint8_t*)out.c_str(), 0, out.size());
        }
        else
        {
            out.reserve(len);
            stream.ReadBytes((uint8_t*)out.c_str(), 0, len);
        }
        return stream;
    }
}

