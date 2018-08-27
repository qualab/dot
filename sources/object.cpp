// DOT public declaration

#include <dot/object.h>
#include <iostream>
#include <utility>

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
}

// Unicode signature: Владимир Керимов
