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
    const char* const object::data::null_string = "null";

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

    bool object::operator == (const object& another) const
    {
        return m_data == another.m_data ||
            m_data && another.m_data && m_data->equals(*another.m_data);
    }

    bool object::operator != (const object& another) const
    {
        return !(*this == another);
    }

    bool object::operator <= (const object& another) const
    {
        return !(another < *this);
    }

    bool object::operator >= (const object& another) const
    {
        return !(*this < another);
    }

    bool object::operator < (const object& another) const
    {
        return !m_data && another.m_data ||
            m_data && another.m_data && m_data->less(*another.m_data);
    }

    bool object::operator > (const object& another) const
    {
        return another < *this;
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

    std::ostream& operator << (std::ostream& stream, const object& source)
    {
        if (source.m_data)
        {
            return stream << *source.m_data;
        }
        else
        {
            return stream << object::data::null_string;
        }
    }

    std::istream& operator >> (std::istream& stream, object& /*destination*/)
    {
        // TODO: initialize data by incoming byte stream
        return stream;
    }

    object::data::data() noexcept
    {
    }

    object::data::~data() noexcept
    {
    }

    void object::data::write(std::ostream& stream) const
    {
        // unknown data output by default
        stream << "<data: " << who().name() << ">";
    }

    void object::data::read(std::istream&)
    {
        // exception of unreadable data throws by default
        // must be overloaded before read data from stream
        throw fail::unreadable_data("Unable to read data of the object from byte stream.");
    }

    bool object::data::operator == (const data& another) const
    {
        return equals(another);
    }

    bool object::data::operator != (const data& another) const
    {
        return !equals(another);
    }

    bool object::data::operator <= (const data& another) const
    {
        return !another.less(*this);
    }

    bool object::data::operator >= (const data& another) const
    {
        return !less(another);
    }

    bool object::data::operator < (const data& another) const
    {
        return less(another);
    }

    bool object::data::operator > (const data& another) const
    {
        return another.less(*this);
    }

    bool object::data::equals(const data& another) const noexcept
    {
        return this == &another; // compare address by default, override if required
    }

    bool object::data::less(const data& another) const noexcept
    {
        return this < &another; // compare address by default, override if required
    }

    const class_id& object::data::id() noexcept
    {
        static const class_id object_data_id("object::data");
        return object_data_id;
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
