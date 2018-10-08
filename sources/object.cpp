// DOT public declaration

#include <dot/object.h>
#include <dot/scalar.h>
#include <dot/copyable.h>
#include <dot/fail.h>
#include <iostream>
#include <utility>
#include <string>

namespace dot
{
    object::object() noexcept
        : m_data(nullptr)
    {
    }

    object::~object() noexcept
    {
        reset();
    }

    void object::reset() noexcept
    {
        if (m_data)
        {
            m_data->~data();
            m_data = nullptr;
        }
    }

    bool object::is_null() const noexcept
    {
        return m_data == nullptr;
    }

    bool object::is_not_null() const noexcept
    {
        return !is_null();
    }

    object::object(const object& another)
        : m_data(nullptr)
    {
        another.assign_to(*this);
    }

    object& object::operator = (const object& another)
    {
        another.assign_to(*this);
        return *this;
    }

    object::object(object&& temporary)
        : m_data(nullptr)
    {
        temporary.assign_to(*this);
    }

    object& object::operator = (object&& temporary)
    {
        temporary.assign_to(*this);
        return *this;
    }

    void object::assign_to(object& target) const&
    {
        target.reset();
        if (m_data)
            target.m_data = m_data->copy_to(target.m_buffer);
    }

    void object::assign_to(object& target) &&
    {
        target.reset();
        if (m_data)
            target.m_data = m_data->move_to(target.m_buffer);
    }

    const object::data& object::get_data() const
    {
        if (!m_data)
            throw fail::null_reference("Object has no data but data reference occured.");
        return *m_data;
    }

    const class_id& object::id() noexcept
    {
        static const class_id object_id("object");
        return object_id;
    }

    const class_id& object::who() const noexcept
    {
        return object::id();
    }

    std::ostream& operator << (std::ostream& stream, const object& source)
    {
        stream << source.who().name() << " = { ";
        if (source.m_data)
        {
            stream << source.m_data->who().name() << " = " << *source.m_data;
        }
        else
        {
            stream << "null";
        }
        return stream << " }";
    }

    std::istream& operator >> (std::istream& stream, object& /*destination*/)
    {
        // TODO: read object with data
        return stream;
    }

    object::data::data() noexcept
    {
    }

    object::data::~data() noexcept
    {
    }

    const class_id& object::data::id() noexcept
    {
        static const class_id object_data_id("object::data");
        return object_data_id;
    }

    const class_id& object::data::who() const noexcept
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
