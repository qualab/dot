// DOT public declaration

#include <dot/object.h>
#include <iostream>
#include <utility>

namespace dot
{
    class_name_type object::class_name = "object";
    class_name_type object::data::class_name = "object::data";

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

    class_name_type object::who() const
    {
        return object::class_name;
    }

    std::ostream& operator << (std::ostream& stream, const object& value)
    {
        stream << value.who() << ": {";
        if (value.m_data)
        {
            stream << value.m_data->who() << ": " << *value.m_data;
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

    class_name_type object::data::who() const
    {
        return object::data::class_name;
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
}

// Unicode signature: Владимир Керимов
