// DOT public declaration

#include <dot/object.h>
#include <dot/scalar.h>
#include <dot/copyable.h>
#include <iostream>
#include <utility>
#include <string>

namespace dot
{
    object::object()
        : m_data(nullptr)
    {
    }

    object::~object()
    {
        reset();
    }

    void object::reset()
    {
        if (m_data)
        {
            m_data->~data();
            m_data = nullptr;
        }
    }

    bool object::is_null() const
    {
        return m_data == nullptr;
    }

    bool object::is_not_null() const
    {
        return !is_null();
    }

    object::object(const object& another)
        : m_data(nullptr)
    {
        *this = another;
    }

    object& object::operator = (const object& another)
    {
        reset();
        if (another.m_data)
            m_data = another.m_data->copy_to(m_buffer); 
        return *this;
    }

    object::object(object&& another)
        : m_data(nullptr)
    {
        *this = std::move(another);
    }

    object& object::operator = (object&& another)
    {
        reset();
        if (another.m_data)
            m_data = another.m_data->move_to(m_buffer);
        return *this;
    }

    const class_id& object::id()
    {
        static const class_id object_id("object");
        return object_id;
    }

    const class_id& object::who() const
    {
        return object::id();
    }

    std::ostream& operator << (std::ostream& stream, const object& value)
    {
        stream << value.who().name() << ": {";
        if (value.m_data)
        {
            stream << value.m_data->who().name() << ": " << *value.m_data;
        }
        else
        {
            stream << "null";
        }
        return stream << '}';
    }

    std::istream& operator >> (std::istream& stream, object&)
    {
        // TODO: reading from stream any type of data
        return stream;
    }

    object::data::data()
    {
    }

    object::data::~data()
    {
    }

    const class_id& object::data::id()
    {
        static const class_id object_data_id("object::data");
        return object_data_id;
    }

    const class_id& object::data::who() const
    {
        return object::data::id();
    }

    std::ostream& operator << (std::ostream& stream, const object::data& value)
    {
        value.write(stream);
        return stream;
    }

    std::istream& operator >> (std::istream& stream, object::data& value)
    {
        value.read(stream);
        return stream;
    }

    template<> void object::set_as(const int64& value) { initialize<scalar<int64>::data>(value); }
    template<> void object::set_as(const int32& value) { initialize<scalar<int32>::data>(value); }
    template<> void object::set_as(const int16& value) { initialize<scalar<int16>::data>(value); }
    template<> void object::set_as(const int8&  value) { initialize<scalar<int8 >::data>(value); }

    template<> void object::set_as(const uint64& value) { initialize<scalar<uint64>::data>(value); }
    template<> void object::set_as(const uint32& value) { initialize<scalar<uint32>::data>(value); }
    template<> void object::set_as(const uint16& value) { initialize<scalar<uint16>::data>(value); }
    template<> void object::set_as(const uint8&  value) { initialize<scalar<uint8 >::data>(value); }

    template<> void object::set_as(const double& value) { initialize<scalar<double>::data>(value); }
    template<> void object::set_as(const float&  value) { initialize<scalar<float >::data>(value); }

    template<> void object::set_as(const bool& value) { initialize<scalar<bool>::data>(value); }
    template<> void object::set_as(const char& value) { initialize<scalar<char>::data>(value); }

    template<> void object::set_as(const char* const& value) { initialize<copyable<std::string>::data>(value); }
    template<> void object::set_as(const std::string& value) { initialize<copyable<std::string>::data>(value); }
}

// Unicode signature: Владимир Керимов
